#include "JPEGLoader.hpp"

#include "../../Exceptions/ImageLoadingFileCorruptionException.hpp"
#include "../../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../../Exceptions/SecurityUnknownPolicyException.hpp"
#include "../../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../../Exceptions/NotSupportedException.hpp"
#include "../../Utility/FunctionalWrapper.hpp"
#include "../../Utility/ZigZacRange.hpp"
#include "../../Utility/Ranges.hpp"

#include <algorithm>
#include <bitset>
#include <array>

namespace ge {

    template <security::SecurityPolicy Policy>
    const std::string JPEGLoader<Policy>::Tag{"jpeg"};

    template <security::SecurityPolicy Policy>
    const IDCT<> JPEGLoader<Policy>::inverseCosineTransform{};

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::JPEGLoader([[maybe_unused]] Policy policy, const std::string& fileName)
        : LoaderInterface{std::move(fileName)}, endOfImage{false}
    {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->fileName};
        try {
            setPolicy(file);
            decodeImage();
        } catch (std::out_of_range const&) {
            throw ImageLoadingFileCorruptionException{this->fileName};
        } catch (HuffmanTreeException&) {
            throw ImageLoadingFileCorruptionException{this->fileName};
        }
    }

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::JPEGLoader(const std::string& fileName) : JPEGLoader{Policy{}, fileName} {}

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::setPolicy(std::istream& file) {
        if constexpr (security::isSecurePolicy<Policy>)
            return parseChunks(FileIter{StreamBuf{file}, StreamBuf{}});
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return parseChunks(FileIter{file});
        else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::parseNextChunk(FileIter& file, uint16_t signature) {
        if (signature == 0xFFD9) {
            endOfImage = true;
            return;
        }
        auto iter = chunkParser.find(signature);
        parsingQueue.push(iter != chunkParser.end() ? iter->second : emptyChunk);
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::parseChunks(FileIter file) {
        if (readType<uint16_t, true>(file) != 0xFFD8)
            throw ImageLoadingInvalidTypeException{fileName};
        while (!endOfImage) {
            parseNextChunk(file, readType<uint16_t, true>(file));
            while (!parsingQueue.empty()) {
                auto chunk = std::move(parsingQueue.front());
                parsingQueue.pop();
                std::invoke(*std::invoke(chunk, std::ref(*this)), file);
            }
        };
    }

    template <security::SecurityPolicy Policy>
    Matrix8<int16_t> JPEGLoader<Policy>::readMatrix(Iter& iter, uint8_t id, int16_t& coeff) {
        uint8_t code = huffmanTables.at(false).at(id)->decoder.decodeToken(iter);
        uint16_t bits = readRNBits<uint16_t>(code, iter);
        coeff += decodeNumber(code, bits);
        std::array<int16_t, 64> data{};
        data.front() = coeff * quantizationTables.at(id)->information.at(0);
        decodeMatrix(data, huffmanTables.at(true).at(id), quantizationTables.at(id), iter);
        return inverseCosineTransform(ZigZacRange<8>::returnZigZac(data));
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::decodeMatrix(std::array<int16_t, 64>& data, HuffmanTablePtr const& table,
        QuantizationTablePtr const& quant, Iter& iter)
    {
        uint16_t bits;
        for (uint8_t length = 1, code; code = table->decoder.decodeToken(iter); ++length) {
            if (code > 15) {
                length += code >> 4;
                code &= 0xF;
            }
            bits = readRNBits<uint16_t>(code, iter);
            if (length >= 64)
                break;
            data[length] = decodeNumber(code, bits) * quant->information.at(length);
        }
    }

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::Iter JPEGLoader<Policy>::getDecodeIterator(void) noexcept {
        if constexpr (security::isSecurePolicy<Policy>)
            return Iter{SafeIter{imageData.begin(), imageData.end()}};
        else
            return Iter{SafeIter{imageData.begin()}};
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::decodeImage(void) {
        Iter iter{getDecodeIterator()};
        std::vector<int16_t> channels;
        channels.resize(componentsTable.size(), 0);
        for (auto i : std::views::iota(std::size_t(0), getBoundry(pixels.getWidth()))) {
            for (auto j : std::views::iota(std::size_t(0), getBoundry(pixels.getHeight()))) {
                MatricesMap matrices;
                auto coef = channels.begin();
                for (const auto& [id, component] : componentsTable)
                    matrices[id] = std::move(readMatrix(iter, component->tableNumber, *coef++));
                drawYCbCrOnImage(matrices, i, j);
            }
        }
    }

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::PixelMatrix<uint8_t> JPEGLoader<Policy>::convertYCbCrToRGB(PixelMatrix<int16_t> yCbCr) noexcept {
        PixelMatrix<uint8_t> rgbColors;
        for (uint8_t i = 0; i < 8; ++i) {
            for (uint8_t j = 0; j < 8; ++j) {
                int16_t red = (double) std::get<2>(yCbCr)[i][j] * 1.402 + std::get<0>(yCbCr)[i][j];
                int16_t blue = (double) std::get<1>(yCbCr)[i][j] * 1.772 + std::get<0>(yCbCr)[i][j];
                int16_t green = (double) (std::get<0>(yCbCr)[i][j] - 0.114 * blue - 0.299 * red) * 1.703577;
                std::get<0>(rgbColors)[i][j] = adjustPixelColor(red + 128);
                std::get<1>(rgbColors)[i][j] = adjustPixelColor(green + 128);
                std::get<2>(rgbColors)[i][j] = adjustPixelColor(blue + 128);
            }
        }
        return rgbColors;
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::DHTChunk::operator() (FileIter& data) {
        uint16_t length = readType<uint16_t, true>(data) - 19;
        uint8_t header = readType<uint8_t>(data); // parse
        std::array<uint8_t, 17> symbolsLengths{};
        std::ranges::for_each(symbolsLengths | std::views::drop(1), [&data](auto& symbol)
            { symbol = *data; ++data; });
        if (ge::accumulate(symbolsLengths, 0u) != length)
            throw ImageLoadingFileCorruptionException{this->loader.fileName};
        std::vector<uint8_t> characters(length, 0);
        std::ranges::for_each(characters, [&data](auto& symbol){ symbol = *data; ++data; });
        if (0xE0 & header)
            throw ImageLoadingFileCorruptionException{this->loader.fileName};
        this->loader.huffmanTables[static_cast<bool>((0x10 & header) >> 4)].emplace(static_cast<uint8_t>(0xF & header),
            std::make_unique<HuffmanTable>(HuffmanTree<uint16_t>{symbolsLengths, characters}));
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::DQTChunk::operator() (FileIter& data) {
        uint16_t length = readType<uint16_t, true>(data) - 3;
        uint8_t header = readType<uint8_t>(data);
        QuantizationTable table;
        if (table.precision = header >> 4)
            throw NotSupportedException{"Only 8-pixels quantization tables are supported."};
        table.information.resize(length);
        std::ranges::for_each(table.information, [&data](uint8_t& quant)
            { quant = *data; ++data; });
        this->loader.quantizationTables.emplace(
            0xF & header, std::make_unique<QuantizationTable>(std::move(table)));
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::SOF0Chunk::operator() (FileIter& data) {
        uint16_t length = readType<uint16_t, true>(data) - 8;
        if (readType<uint8_t>(data) != 8)
            throw NotSupportedException{"Other JPEG data precisions than 8 are not supported."};
        uint16_t height = readType<uint16_t, true>(data);
        this->loader.pixels.resize(height, readType<uint16_t, true>(data));
        uint8_t components = readType<uint8_t>(data);
        if (length / 3 != components)
            throw ImageLoadingFileCorruptionException{this->loader.fileName};
        for (uint8_t i = 0;i != components; ++i){
            uint8_t head = readType<uint8_t>(data);
            this->loader.componentsTable.emplace(head,
                std::make_unique<Component>(readType<uint8_t>(data), readType<uint8_t>(data)));
        }
    }

    template <security::SecurityPolicy Policy>
    bool JPEGLoader<Policy>::SOSChunk::iterable(FileIter& data) const noexcept {
        if constexpr (security::isSecurePolicy<Policy>)
            return data.isSafe();
        else
            return data != FileIter{};
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::SOSChunk::operator() (FileIter& data) {
        uint16_t length = readType<uint16_t, true>(data) - 2;
        std::advance(data, length); // progressive jpegs are not used
        while (iterable(data)) {
            auto byte = readType<uint8_t>(data);
            if (byte == 0xFF)
                if (uint8_t header = readType<uint8_t>(data))
                    return this->loader.parseNextChunk(data, 0xFF00 | header);
            this->loader.imageData.push_back(byte);
        }
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::drawYCbCrOnImage(MatricesMap& matrices, std::size_t row, std::size_t column) {
        auto [redMatrix, greenMatrix, blueMatrix] = convertYCbCrToRGB({matrices.at(1), matrices.at(2), matrices.at(3)});
        for (std::size_t iBase = row * 8, i = iBase; i < iBase + 8 && i < pixels.getHeight(); ++i) {
            for (std::size_t jBase = column * 8, j = jBase; j < jBase + 8 && j < pixels.getWidth(); ++j) {
                pixels[pixels.getHeight() - 1 - i][j].red = redMatrix[i - iBase][j - jBase];
                pixels[pixels.getHeight() - 1 - i][j].green = greenMatrix[i - iBase][j - jBase];
                pixels[pixels.getHeight() - 1 - i][j].blue = blueMatrix[i - iBase][j - jBase];
            }
        }
    }

    template <security::SecurityPolicy Policy>
    int32_t JPEGLoader<Policy>::decodeNumber(uint8_t code, uint16_t bits) noexcept {
        uint16_t coeff = 1 << code - 1;
        return bits >= coeff ? bits : (int32_t) bits - (2 * coeff - 1);
    }

    template <security::SecurityPolicy Policy>
    std::size_t JPEGLoader<Policy>::getBoundry(std::size_t boundry) noexcept {
        return (boundry >> 3) + (boundry % 8 ? 1 : 0);
    }

    template <security::SecurityPolicy Policy>
    uint8_t JPEGLoader<Policy>::adjustPixelColor(int16_t color) noexcept {
        return color > 0xff ? 0xff : (color < 0 ? 0 : color);
    }

    template <security::SecurityPolicy Policy>
    void JPEGLoader<Policy>::EmptyChunk::operator() (FileIter& data) {
        std::advance(data, readType<uint16_t, true>(data) - 2);
    }

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::HuffmanTable::HuffmanTable(HuffmanTree<uint16_t> tree) : decoder{std::move(tree)} {}

    template <security::SecurityPolicy Policy>
    JPEGLoader<Policy>::Component::Component(uint8_t tableNumber, uint8_t samplings) noexcept
        : verticalSampling{static_cast<uint8_t>(0xF & samplings)},
            horizontalSampling{static_cast<uint8_t>(samplings >> 4)},tableNumber{tableNumber} {}

    template <security::SecurityPolicy Policy>
    const JPEGLoader<Policy>::ChunkParser JPEGLoader<Policy>::emptyChunk = FunctionalWrapper<JPEGLoader<Policy>::EmptyChunk, JPEGLoader<Policy>::ChunkInterface>{};

    template <security::SecurityPolicy Policy>
    const std::map<uint16_t, typename JPEGLoader<Policy>::ChunkParser> JPEGLoader<Policy>::chunkParser {
        {0xFFC4, FunctionalWrapper<JPEGLoader::DHTChunk, JPEGLoader::ChunkInterface>{}},
        {0xFFDB, FunctionalWrapper<JPEGLoader::DQTChunk, JPEGLoader::ChunkInterface>{}},
        {0xFFC0, FunctionalWrapper<JPEGLoader::SOF0Chunk, JPEGLoader::ChunkInterface>{}},
        {0xFFDA, FunctionalWrapper<JPEGLoader::SOSChunk, JPEGLoader::ChunkInterface>{}}
    };

}
