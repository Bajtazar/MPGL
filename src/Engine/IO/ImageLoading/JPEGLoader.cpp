/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#include "JPEGLoader.hpp"

#include "../../Exceptions/ImageLoadingFileCorruptionException.hpp"
#include "../../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../../Exceptions/SecurityUnknownPolicyException.hpp"
#include "../../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../../Exceptions/NotSupportedException.hpp"
#include "../../Utility/FunctionalWrapper.hpp"
#include "../../Utility/ZigZacRange.hpp"
#include "../../Utility/Ranges.hpp"
#include "../FileIO.hpp"

#include <algorithm>
#include <bitset>
#include <array>

namespace mpgl {

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::Path const JPEGLoader<Policy>::Tag{"jpeg"};

    template <security::SecurityPolicy Policy>
    IDCT<> const JPEGLoader<Policy>::ifct{};

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::JPEGLoader(
        [[maybe_unused]] Policy policy,
        Path const& filePath)
            : LoaderInterface{filePath}, endOfImage{false}
    {
        if (auto file = FileIO::readFileToVec(this->filePath)) {
            try {
                setPolicy(*file);
                decodeImage();
            } catch (std::out_of_range const&) {
                throw ImageLoadingFileCorruptionException{this->filePath};
            } catch (HuffmanTreeException const&) {
                throw ImageLoadingFileCorruptionException{this->filePath};
            }
        } else
            throw ImageLoadingFileOpenException{this->filePath};
    }

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::JPEGLoader(Path const& filePath)
        : JPEGLoader{Policy{}, filePath} {}

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::setPolicy(FileBuffer const& file) {
        if constexpr (security::isSecurePolicy<Policy>)
            return parseChunks(FileIter{file.begin(), file.end()});
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return parseChunks(FileIter{file.begin()});
        else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::parseNextChunk(
        FileIter& file,
        uint16 signature)
    {
        if (signature == 0xFFD9) {
            endOfImage = true;
            return;
        }
        auto iter = chunkParser.find(signature);
        parsingQueue.push(iter != chunkParser.end() ?
            iter->second : emptyChunk);
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::parseChunks(FileIter file) {
        if (readType<uint16, true>(file) != 0xFFD8)
            throw ImageLoadingInvalidTypeException{filePath};
        while (!endOfImage) {
            parseNextChunk(file, readType<uint16, true>(file));
            while (!parsingQueue.empty()) {
                auto chunk = std::move(parsingQueue.front());
                parsingQueue.pop();
                (*(chunk)(std::ref(*this)))(file);
            }
        };
    }

    template <security::SecurityPolicy Policy>
    Matrix8<int16> JPEGLoader<Policy>::readMatrix(
        Iter& iter,
        uint8 id,
        int16& coeff)
    {
        uint8 code = huffmanTables.at(false).at(id)->decoder(iter);
        uint16 bits = readRNBits<uint16>(code, iter);
        coeff += decodeNumber(code, bits);
        std::array<int16, 64> data{};
        data.front() = coeff
            * quantizationTables.at(id)->information.at(0);
        decodeMatrix(data, huffmanTables.at(true).at(id),
            quantizationTables.at(id), iter);
        return ifct(ZigZacRange<8>::fromZigZac(data));
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::decodeMatrix(
        QuantizationData& data,
        HuffmanTablePtr const& table,
        QuantizationTablePtr const& quant,
        Iter& iter)
    {
        uint16 bits;
        for (uint8 length = 1, code; code = table->decoder(iter);
            ++length)
        {
            if (code > 15) {
                length += code >> 4;
                code &= 0xF;
            }
            bits = readRNBits<uint16>(code, iter);
            if (length >= 64)
                return;
            data[length] = decodeNumber(code, bits)
                * quant->information.at(length);
        }
    }

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::Iter JPEGLoader<Policy>::getDecodeIterator(
        void) noexcept
    {
        if constexpr (security::isSecurePolicy<Policy>)
            return Iter{SafeIter{imageData.begin(), imageData.end()}};
        else
            return Iter{SafeIter{imageData.begin()}};
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::decodeImage(void) {
        Iter iter{getDecodeIterator()};
        Channels channels;
        channels.resize(componentsTable.size(), 0);
        for (size_type i = 0; i < getBoundry(pixels.getWidth()); ++i) {
            for (size_type j = 0; j < getBoundry(pixels.getHeight());
                ++j)
            {
                decodeImageBlock(iter, i, j, channels);
            }
        }
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::decodeImageBlock(
        Iter& iter,
        size_type y,
        size_type x,
        Channels& channels)
    {
        MatricesMap matrices;
        auto coef = channels.begin();
        for (const auto& [id, component] : componentsTable)
            matrices[id] = std::move(readMatrix(iter,
                component->tableNumber, *coef++));
        drawYCbCrOnImage(matrices, y, x);
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::convertYCbCrToRGB(
        PixelMatrix<int16> const& yCbCr,
        PixelMatrix<uint8>& rgb,
        uint8 x,
        uint8 y) noexcept
    {
        int16 red = (float64) std::get<2>(yCbCr)[x][y]
            * 1.402 + std::get<0>(yCbCr)[x][y];
        int16 blue = (float64) std::get<1>(yCbCr)[x][y]
            * 1.772 + std::get<0>(yCbCr)[x][y];
        int16 green = (float64) (std::get<0>(yCbCr)[x][y]
            - 0.114 * blue - 0.299 * red) * 1.703577;
        std::get<0>(rgb)[x][y]
            = adjustPixelColor(red + 128);
        std::get<1>(rgb)[x][y]
            = adjustPixelColor(green + 128);
        std::get<2>(rgb)[x][y]
            = adjustPixelColor(blue + 128);
    }

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::PixelMatrix<uint8>
        JPEGLoader<Policy>::convertYCbCrToRGB(
            PixelMatrix<int16> const& yCbCr) noexcept
    {
        PixelMatrix<uint8> rgbColors;
        for (uint8 i = 0; i < 8; ++i)
            for (uint8 j = 0; j < 8; ++j)
                convertYCbCrToRGB(yCbCr, rgbColors, i, j);
        return rgbColors;
    }

    template <security::SecurityPolicy Policy>
    uint8 JPEGLoader<Policy>::DHTChunk::readChunk(FileIter& data) {
        uint16 length = readType<uint16, true>(data) - 19;
        uint8 header = readType<uint8>(data);
        std::ranges::for_each(symbolsLengths | std::views::drop(1),
            [&data](auto& symbol) { symbol = *data; ++data; });
        if (mpgl::accumulate(symbolsLengths, 0u) != length)
            throw ImageLoadingFileCorruptionException{
                this->loader.filePath};
        characters.resize(length);
        std::ranges::for_each(characters,
            [&data](auto& symbol){ symbol = *data; ++data; });
        return header;
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::DHTChunk::operator() (FileIter& data) {
        uint8 header = readChunk(data);
        if (0xE0 & header)
            throw ImageLoadingFileCorruptionException{
                this->loader.filePath};
        this->loader.huffmanTables[static_cast<bool>(
            (0x10 & header) >> 4)].emplace(
                static_cast<uint8>(0xF & header),
                std::make_unique<HuffmanTable>(HuffmanTree<uint16>{
                    symbolsLengths, characters}));
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::DQTChunk::operator() (FileIter& data) {
        uint16 length = readType<uint16, true>(data) - 3;
        uint8 header = readType<uint8>(data);
        QuantizationTable table;
        if (table.precision = header >> 4)
            throw NotSupportedException{
                "Only 8-pixels quantization tables are supported."};
        table.information.resize(length);
        std::ranges::for_each(table.information, [&data](uint8& quant)
            { quant = *data; ++data; });
        this->loader.quantizationTables.emplace(
            0xF & header, std::make_unique<QuantizationTable>(
                std::move(table)));
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::SOF0Chunk::parseComponents(
        FileIter& data,
        uint16 length)
    {
        uint8 components = readType<uint8>(data);
        if (length / 3 != components)
            throw ImageLoadingFileCorruptionException{
                this->loader.filePath};
        for (uint8 i = 0;i != components; ++i){
            uint8 head = readType<uint8>(data);
            this->loader.componentsTable.emplace(head,
                std::make_unique<Component>(
                    readType<uint8>(data), readType<uint8>(data)));
        }
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::SOF0Chunk::operator() (FileIter& data) {
        uint16 length = readType<uint16, true>(data) - 8;
        if (readType<uint8>(data) != 8)
            throw NotSupportedException{
                "Other JPEG data precisions than 8 are not supported."};
        uint16 height = readType<uint16, true>(data);
        this->loader.pixels.resize(height,
            readType<uint16, true>(data));
        parseComponents(data, length);
    }

    template <security::SecurityPolicy Policy>
    bool JPEGLoader<Policy>::SOSChunk::iterable(
        FileIter& data) const noexcept
    {
        if constexpr (security::isSecurePolicy<Policy>)
            return data.isSafe();
        else
            return data != FileIter{};
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::SOSChunk::operator() (FileIter& data) {
        uint16 length = readType<uint16, true>(data) - 2;
        std::advance(data, length); // progressive jpegs are not used
        while (iterable(data)) {
            auto byte = readType<uint8>(data);
            if (byte == 0xFF)
                if (uint8 header = readType<uint8>(data))
                    return this->loader.parseNextChunk(data,
                        0xFF00 | header);
            this->loader.imageData.push_back(byte);
        }
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::drawBlockOnImage(
        Matrix8<uint8> const& red,
        Matrix8<uint8> const& green,
        Matrix8<uint8> const& blue,
        Vector2<size_type> const& coordinates,
        Vector2<size_type> const& bases)
    {
        auto const& [x, y] = coordinates;
        auto const& [xBase, yBase] = bases;
        pixels[pixels.getHeight() - 1 - y][x].red
            = red[y - yBase][x - xBase];
        pixels[pixels.getHeight() - 1 - y][x].green
            = green[y - yBase][x - xBase];
        pixels[pixels.getHeight() - 1 - y][x].blue
            = blue[y - yBase][x - xBase];
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::drawYCbCrOnImage(
        MatricesMap& matrices,
        size_type row,
        size_type column)
    {
        auto [redMatrix, greenMatrix, blueMatrix] = convertYCbCrToRGB(
            {matrices.at(1), matrices.at(2), matrices.at(3)});
        for (size_type iBase = row * 8, i = iBase;
            i < iBase + 8 && i < pixels.getHeight(); ++i)
        {
            for (size_type jBase = column * 8, j = jBase;
                j < jBase + 8 && j < pixels.getWidth(); ++j)
            {
                drawBlockOnImage(redMatrix, greenMatrix, blueMatrix,
                    {i, j}, {iBase, jBase});
            }
        }
    }

    template <security::SecurityPolicy Policy>
    int32 JPEGLoader<Policy>::decodeNumber(
        uint8 code,
        uint16 bits) noexcept
    {
        uint16 coeff = 1 << code - 1;
        return bits >= coeff ? bits : (int32) bits - (2 * coeff - 1);
    }

    template <security::SecurityPolicy Policy>
    std::size_t JPEGLoader<Policy>::getBoundry(
        size_type boundry) noexcept
    {
        return (boundry >> 3) + (boundry % 8 ? 1 : 0);
    }

    template <security::SecurityPolicy Policy>
    uint8 JPEGLoader<Policy>::adjustPixelColor(int16 color) noexcept {
        return std::clamp(color, int16(0), int16(0xff));
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::EmptyChunk::operator() (FileIter& data) {
        std::advance(data, readType<uint16, true>(data) - 2);
    }

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::HuffmanTable::HuffmanTable(
        HuffmanTree<uint16> tree)
            : decoder{std::move(tree)} {}

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::Component::Component(
        uint8 tableNumber,
        uint8 samplings) noexcept
            : verticalSampling{static_cast<uint8>(0xF & samplings)},
            horizontalSampling{static_cast<uint8>(samplings >> 4)},
            tableNumber{tableNumber} {}

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::ChunkParser const
        JPEGLoader<Policy>::emptyChunk
            = FunctionalWrapper<JPEGLoader<Policy>::EmptyChunk,
                JPEGLoader<Policy>::ChunkInterface>{};

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::ParserMap const
        JPEGLoader<Policy>::chunkParser
    {
        {0xFFC4, FunctionalWrapper<JPEGLoader::DHTChunk,
            JPEGLoader::ChunkInterface>{}},
        {0xFFDB, FunctionalWrapper<JPEGLoader::DQTChunk,
            JPEGLoader::ChunkInterface>{}},
        {0xFFC0, FunctionalWrapper<JPEGLoader::SOF0Chunk,
            JPEGLoader::ChunkInterface>{}},
        {0xFFDA, FunctionalWrapper<JPEGLoader::SOSChunk,
            JPEGLoader::ChunkInterface>{}}
    };

}
