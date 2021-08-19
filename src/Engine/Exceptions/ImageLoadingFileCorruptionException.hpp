#pragma once

#include "ImageLoadingException.hpp"

namespace ge {

    class ImageLoadingFileCorruptionException : public ImageLoadingException {
    public:
        explicit ImageLoadingFileCorruptionException(const std::string& fileName) noexcept : ImageLoadingException{fileName, "File " + fileName + " is corrupted\n"} {}
    };

}
