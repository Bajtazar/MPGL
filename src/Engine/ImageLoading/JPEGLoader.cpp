#include "JPEGLoader.hpp"

#include "../Exceptions/ImageLoadingFileCorruptionException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../Exceptions/NotSupportedException.hpp"
#include "../Utility/FunctionalWrapper.hpp"
#include "../Utility/ZigZacRange.hpp"
#include "../Utility/Ranges.hpp"

#include <algorithm>
#include <bitset>
#include <array>

namespace ge {

    const std::string JPEGLoader::Tag{"jpeg"};
    const IDCT<> JPEGLoader::inverseCosineTransform{};

    JPEGLoader::JPEGLoader(const std::string& fileName) : LoaderInterface{std::move(fileName)}, endOfImage{false} {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->fileName};
        parseChunks(file);
        decodeImage();
    }

    void JPEGLoader::parseNextChunk(std::istream& file, uint16_t signature) {
        if (signature == 0xFFD9) {
            endOfImage = true;
            return;
        }
        auto iter = chunkParser.find(signature);
        parsingQueue.push(iter != chunkParser.end() ? iter->second : emptyChunk);
    }

    void JPEGLoader::parseChunks(std::ifstream& file) {
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

    Matrix<int16_t, 8> JPEGLoader::readMatrix(Iter& iter, uint8_t id, int16_t& coeff) noexcept {
        uint8_t code = huffmanTables[false][id]->decoder.decodeToken(iter);
        uint16_t bits = readRNBits<uint16_t>(code, iter);
        coeff += decodeNumber(code, bits);
        std::array<int16_t, 64> data{};
        data.front() = coeff * quantizationTables[id]->information[0];
        decodeMatrix(data, huffmanTables[true][id], quantizationTables[id], iter);
        return inverseCosineTransform.invertVector(ZigZacRange::returnZigZac(data));
    }

    void JPEGLoader::decodeMatrix(std::array<int16_t, 64>& data, HuffmanTablePtr const& table,
        QuantizationTablePtr const& quant, Iter& iter) noexcept
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
            data[length] = decodeNumber(code, bits) * quant->information[length];
        }
    }

    void JPEGLoader::decodeImage(void) {
        Iter iter{imageData.begin()};
        std::vector<int16_t> channels;
        channels.resize(componentsTable.size(), 0);
        for (auto i : std::views::iota(static_cast<std::size_t>(0), getBoundry(pixels.getWidth()))) {
            for (auto j : std::views::iota(static_cast<std::size_t>(0), getBoundry(pixels.getHeight()))) {
                MatricesMap matrices;
                auto coef = channels.begin();
                for (const auto& [id, component] : componentsTable)
                    matrices[id] = std::move(readMatrix(iter, component->tableNumber, *coef++));
                drawYCbCrOnImage(matrices, i, j);
            }
        }
    }

    JPEGLoader::PixelMatrix<uint8_t> JPEGLoader::convertYCbCrToRGB(PixelMatrix<int16_t> yCbCr) noexcept {
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

    void JPEGLoader::DHTChunk::operator() (std::istream& data) {
        uint16_t length = readType<uint16_t, true>(data) - 19;
        uint8_t header = readType<uint8_t>(data); // parse
        std::array<uint8_t, 17> symbolsLengths{};
        std::ranges::for_each(symbolsLengths | std::views::drop(1), [&data](auto& symbol)
            { data.get(reinterpret_cast<char&>(symbol)); });
        if (ge::accumulate(symbolsLengths, 0u) != length)
            throw ImageLoadingFileCorruptionException{loader.fileName};
        std::vector<uint8_t> characters(length, 0);
        std::ranges::for_each(characters, [&data](auto& symbol){ data.get(reinterpret_cast<char&>(symbol)); });
        if (0xe0 & header)
            throw ImageLoadingFileCorruptionException{loader.fileName};
        loader.huffmanTables[static_cast<bool>((0x10 & header) >> 4)].emplace(static_cast<uint8_t>(0xF & header),
            std::make_unique<HuffmanTable>(HuffmanTree<uint16_t>{symbolsLengths, characters}));
    }

    void JPEGLoader::DQTChunk::operator() (std::istream& data) {
        uint16_t length = readType<uint16_t, true>(data) - 3;
        uint8_t header = readType<uint8_t>(data);
        QuantizationTable table;
        if (table.precision = header >> 4)
            throw NotSupportedException{"Only 8-pixels quantization tables are supported."};
        table.information.resize(length);
        std::ranges::for_each(table.information, [&data](uint8_t& quant)
            { data.get(reinterpret_cast<char&>(quant)); });
        loader.quantizationTables.emplace(
            0xF & header, std::make_unique<QuantizationTable>(std::move(table)));
    }

    void JPEGLoader::SOF0Chunk::operator() (std::istream& data) {
        uint16_t length = readType<uint16_t, true>(data) - 8;
        if (readType<uint8_t>(data) != 8)
            throw NotSupportedException{"Other JPEG data precisions than 8 are not supported."};
        uint16_t height = readType<uint16_t, true>(data);
        loader.pixels.resize(height, readType<uint16_t, true>(data));
        uint8_t components = readType<uint8_t>(data);
        if (length / 3 != components)
            throw ImageLoadingFileCorruptionException{loader.fileName};
        for (uint8_t i = 0;i != components; ++i){
            uint8_t head = readType<uint8_t>(data);
            loader.componentsTable.emplace(head,
                std::make_unique<Component>(readType<uint8_t>(data), readType<uint8_t>(data)));
        }
    }

    void JPEGLoader::SOSChunk::operator() (std::istream& data) {
        uint16_t length = readType<uint16_t, true>(data) - 2;
        ignoreNBytes(length, data); // progressive jpegs are not used
        while (!data.eof()) {
            auto byte = readType<uint8_t>(data);
            if (byte == 0xFF)
                if (uint8_t header = readType<uint8_t>(data))
                    return loader.parseNextChunk(data, 0xFF00 | header);
            loader.imageData.push_back(byte);
        }
        if (data.eof())
            loader.endOfImage = true;
    }

    void JPEGLoader::drawYCbCrOnImage(MatricesMap& matrices, std::size_t row, std::size_t column) noexcept {
        auto [redMatrix, greenMatrix, blueMatrix] = convertYCbCrToRGB({matrices[1], matrices[2], matrices[3]});
        for (std::size_t iBase = row * 8, i = iBase; i < iBase + 8 && i < pixels.getHeight(); ++i) {
            for (std::size_t jBase = column * 8, j = jBase; j < jBase + 8 && j < pixels.getWidth(); ++j) {
                pixels[pixels.getHeight() - 1 - i][j].red = redMatrix[i - iBase][j - jBase];
                pixels[pixels.getHeight() - 1 - i][j].green = greenMatrix[i - iBase][j - jBase];
                pixels[pixels.getHeight() - 1 - i][j].blue = blueMatrix[i - iBase][j - jBase];
            }
        }
    }

    int32_t JPEGLoader::decodeNumber(uint8_t code, uint16_t bits) noexcept {
        uint16_t coeff = 1 << code - 1;
        return bits >= coeff ? bits : (int32_t) bits - (2 * coeff - 1);
    }

    std::size_t JPEGLoader::getBoundry(std::size_t boundry) noexcept {
        return (boundry >> 3) + (boundry % 8 ? 1 : 0);
    }

    uint8_t JPEGLoader::adjustPixelColor(int16_t color) noexcept {
        return color > 0xff ? 0xff : (color < 0 ? 0 : color);
    }

    void JPEGLoader::EmptyChunk::operator() (std::istream& data) {
        ignoreNBytes(readType<uint16_t, true>(data) - 2, data);
    }

    JPEGLoader::HuffmanTable::HuffmanTable(HuffmanTree<uint16_t> tree) : decoder{std::move(tree)} {}

    JPEGLoader::Component::Component(uint8_t tableNumber, uint8_t samplings) noexcept
        : verticalSampling{static_cast<uint8_t>(0xF & samplings)},
            horizontalSampling{static_cast<uint8_t>(samplings >> 4)},tableNumber{tableNumber} {}

    const JPEGLoader::ChunkParser JPEGLoader::emptyChunk = FunctionalWrapper<JPEGLoader::EmptyChunk, JPEGLoader::ChunkInterface>{};

    const std::map<uint16_t, std::function<std::unique_ptr<JPEGLoader::ChunkInterface> (JPEGLoader&)>> JPEGLoader::chunkParser {
        {0xFFC4, FunctionalWrapper<JPEGLoader::DHTChunk, JPEGLoader::ChunkInterface>{}},
        {0xFFDB, FunctionalWrapper<JPEGLoader::DQTChunk, JPEGLoader::ChunkInterface>{}},
        {0xFFC0, FunctionalWrapper<JPEGLoader::SOF0Chunk, JPEGLoader::ChunkInterface>{}},
        {0xFFDA, FunctionalWrapper<JPEGLoader::SOSChunk, JPEGLoader::ChunkInterface>{}}
    };

}
