#include "JPEGLoader.hpp"

#include "../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../Utility/FunctionalWrapper.hpp"

namespace ge {

    const std::string JPEGLoader::Tag{"jpeg"};

    JPEGLoader::JPEGLoader(const std::string& fileName) : LoaderInterface{std::move(fileName)} {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->fileName};
        parseChunks(file);
    }

    void JPEGLoader::parseChunks(std::ifstream& file) {
        if (readType<uint16_t>(file) != 0xD8FF)
            throw ImageLoadingInvalidTypeException{fileName};
        for (uint16_t signature = readType<uint16_t, true>(file); 0xFFD9 != signature && !file.bad();
                signature = readType<uint16_t, true>(file)) {
            if (auto iter = chunkParser.find(signature); iter != chunkParser.end())
                std::invoke(*std::invoke(iter->second, std::ref(*this)), file);
            else
                ignoreNBytes(readType<uint16_t, true>(file) - 2, file);
        }
    }

    void JPEGLoader::StartOfScanChunk::operator() (std::istream& data) {
        ignoreNBytes(3, data);
        while (true) {
            if (readType<uint8_t>(data) == 0xFF) {
                if (auto point = readType<uint8_t>(data); point != 0x00 && !(point >= 0xD0 && point <= 0xD7)) {
                    data.putback(0xFF);
                    data.putback(point);
                    return;
                }
            }
        }
    }

    const std::map<uint16_t, std::function<std::unique_ptr<JPEGLoader::ChunkInterface> (JPEGLoader&)>> JPEGLoader::chunkParser {
        {0xFFDA, FunctionalWrapper<JPEGLoader::StartOfScanChunk, JPEGLoader::ChunkInterface>{}}
    };

}
