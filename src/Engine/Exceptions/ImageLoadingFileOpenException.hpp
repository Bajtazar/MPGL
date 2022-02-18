#pragma once

#include "ImageLoadingException.hpp"

namespace mpgl {

    class ImageLoadingFileOpenException : ImageLoadingException {
    public:
        explicit ImageLoadingFileOpenException(const std::string& fileName) noexcept : ImageLoadingException{fileName, "Cannot open " + fileName + " file\n"} {}
    };

}
