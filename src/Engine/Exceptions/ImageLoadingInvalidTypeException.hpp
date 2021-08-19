#pragma once

#include "ImageLoadingException.hpp"

namespace ge {

    class ImageLoadingInvalidTypeException : ImageLoadingException {
    public:
        explicit ImageLoadingInvalidTypeException(const std::string& fileName) noexcept : ImageLoadingException{fileName, "File " + fileName + " contains invalid data"} {}
    };

}