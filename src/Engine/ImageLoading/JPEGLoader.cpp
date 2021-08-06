#include "JPEGLoader.hpp"

#include "../Exceptions/ImageLoadingFileCorruptionException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../Exceptions/NotSupportedException.hpp"
#include "../Utility/FunctionalWrapper.hpp"
#include "../Utility/Ranges.hpp"

#include <algorithm>
#include <bitset>
#include <array>

namespace ge {

    const std::string JPEGLoader::Tag{"jpeg"};

    JPEGLoader::JPEGLoader(const std::string& fileName) : LoaderInterface{std::move(fileName)}, endOfImage{false} {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->fileName};
        parseChunks(file);
    }

    void JPEGLoader::parseNextChunk(std::istream& file, uint16_t signature) {
        if (signature == 0xFFD9) {
            endOfImage = true;
            return;
        }
        if (auto iter = chunkParser.find(signature); iter != chunkParser.end())
            parsingQueue.push(iter->second);
        else
            parsingQueue.push(emptyChunk);
    }

    void JPEGLoader::parseChunks(std::ifstream& file) {
        if (readType<uint16_t>(file) != 0xD8FF)
            throw ImageLoadingInvalidTypeException{fileName};
        while (!endOfImage) { // while not end
            parseNextChunk(file, readType<uint16_t, true>(file));
            while (!parsingQueue.empty()) {
                auto chunk = std::move(parsingQueue.front());
                parsingQueue.pop();
                std::invoke(*std::invoke(chunk, std::ref(*this)), file);
            }
        };
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
        loader.huffmanTables.emplace_back(HuffmanTree<uint16_t>{symbolsLengths, characters}, header, loader.fileName);
    }

    void JPEGLoader::DQTChunk::operator() (std::istream& data) {
        uint16_t length = readType<uint16_t, true>(data) - 3;
        uint8_t header = readType<uint8_t>(data);
        QuantizationTable table;
        table.number = 0xF & header;
        table.precision = (0xF0 & header) >> 4;
        table.information.resize(length);
        std::ranges::for_each(table.information, [&data](uint8_t& quant)
            { data.get(reinterpret_cast<char&>(quant)); });
        loader.quantizationTables.push_back(std::move(table));
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
        for (uint8_t i = 0;i != components; ++i)
            loader.componentsTable.emplace_back(readType<uint8_t>(data),
                readType<uint8_t>(data), readType<uint8_t>(data));
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

    void JPEGLoader::EmptyChunk::operator() (std::istream& data) {
        ignoreNBytes(readType<uint16_t, true>(data) - 2, data);
    }

    JPEGLoader::HuffmanTable::HuffmanTable(HuffmanTree<uint16_t> tree, uint8_t header, const std::string& fileName)
        : decoder{std::move(tree)}, number{static_cast<uint8_t>(0xF & header)}, type{static_cast<bool>((0x10 & header) >> 4)}
    {
        if (0xe0 & header)
            throw ImageLoadingFileCorruptionException{fileName};
    }

    JPEGLoader::Component::Component(uint8_t tableNumber, uint8_t samplings, uint8_t id) noexcept
        : id{id}, verticalSampling{static_cast<uint8_t>(0xF & samplings)},
            horizontalSampling{static_cast<uint8_t>((0xF0 & samplings) >> 4)},tableNumber{tableNumber} {}

    const JPEGLoader::ChunkParser JPEGLoader::emptyChunk = FunctionalWrapper<JPEGLoader::EmptyChunk, JPEGLoader::ChunkInterface>{};

    const std::map<uint16_t, std::function<std::unique_ptr<JPEGLoader::ChunkInterface> (JPEGLoader&)>> JPEGLoader::chunkParser {
        {0xFFC4, FunctionalWrapper<JPEGLoader::DHTChunk, JPEGLoader::ChunkInterface>{}},
        {0xFFDB, FunctionalWrapper<JPEGLoader::DQTChunk, JPEGLoader::ChunkInterface>{}},
        {0xFFC0, FunctionalWrapper<JPEGLoader::SOF0Chunk, JPEGLoader::ChunkInterface>{}},
        {0xFFDA, FunctionalWrapper<JPEGLoader::SOSChunk, JPEGLoader::ChunkInterface>{}}
    };

}
