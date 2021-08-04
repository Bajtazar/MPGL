#include "JPEGLoader.hpp"

#include "../Exceptions/ImageLoadingFileCorruptionException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../Utility/FunctionalWrapper.hpp"
#include "../Utility/Ranges.hpp"

#include <algorithm>
#include <bitset>
#include <array>

namespace ge {

    const std::string JPEGLoader::Tag{"jpeg"};

    JPEGLoader::JPEGLoader(const std::string& fileName) : LoaderInterface{std::move(fileName)}, huffmanTrees{0} {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->fileName};
        parseChunks(file);
    }

    void JPEGLoader::invokeParser(uint16_t signature, std::istream& file) {
        if (auto iter = chunkParser.find(signature); iter != chunkParser.end())
            std::invoke(*std::invoke(iter->second, std::ref(*this)), file);
        else
            ignoreNBytes(readType<uint16_t, true>(file) - 2, file);
    }

    void JPEGLoader::parseChunks(std::ifstream& file) {
        if (readType<uint16_t>(file) != 0xD8FF)
            throw ImageLoadingInvalidTypeException{fileName};
        for (uint16_t signature = readType<uint16_t, true>(file); 0xFFD9 != signature && !file.eof();
                signature = readType<uint16_t, true>(file))
            invokeParser(signature, file);
    }

    void JPEGLoader::DHTChunk::operator() (std::istream& data) {
        uint16_t length = readType<uint16_t, true>(data) - 19;
        uint8_t ht = readType<uint8_t>(data); // parse
        std::array<uint8_t, 16> symbolsLengths;
        std::ranges::for_each(symbolsLengths, [&data](auto& symbol) { data.get(reinterpret_cast<char&>(symbol)); });
        if (ge::accumulate(symbolsLengths, 0u) != length)
            throw ImageLoadingFileCorruptionException{loader.fileName};
        std::vector<uint8_t> characters(length, 0);
        std::ranges::for_each(characters, [&data](auto& symbol){ data.get(reinterpret_cast<char&>(symbol)); });
        HuffmanTree<> tree{symbolsLengths, characters}; //// debug this
    }

    void JPEGLoader::StartOfScanChunk::operator() (std::istream& data) {
        ignoreNBytes(3, data);
        while (!data.eof()) {
            if (readType<uint8_t>(data) == 0xFF) {
                if (auto point = readType<uint8_t>(data); point != 0x00 && !(point >= 0xD0 && point <= 0xD7)) {
                    if (point != 0xD9)
                        loader.invokeParser(0xFF00 | point, data);
                    return;
                }
            }
        }
    }

    const std::map<uint16_t, std::function<std::unique_ptr<JPEGLoader::ChunkInterface> (JPEGLoader&)>> JPEGLoader::chunkParser {
        {0xFFC4, FunctionalWrapper<JPEGLoader::DHTChunk, JPEGLoader::ChunkInterface>{}},
        {0xFFDA, FunctionalWrapper<JPEGLoader::StartOfScanChunk, JPEGLoader::ChunkInterface>{}}
    };

}
