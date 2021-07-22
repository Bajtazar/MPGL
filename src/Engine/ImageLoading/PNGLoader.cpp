#include "PNGLoader.hpp"

#include "../Exceptions/ImageLoadingFileException.hpp"
#include "../Exceptions/ImageLoadingInvalidTypeException.hpp"

namespace ge {

    const std::string PNGLoader::Tag{"png"};

    PNGLoader::PNGLoader(const std::string& fileName) : LoaderInterface(std::move(fileName)) {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileException{this->fileName};
        readHeader(file);
        readImage(file);
    }

    void PNGLoader::readHeader(std::ifstream& file) {
        if (readType<uint64_t>(file) != 0x0A1A0D0A504E4789)
            throw ImageLoadingInvalidTypeException{fileName};
    }

    void PNGLoader::readImage(std::ifstream& file) noexcept {

    }

}
