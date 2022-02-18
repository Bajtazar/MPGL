#pragma once

#include "ImageLoadingException.hpp"

namespace mpgl {

    class ImageLoadingUnsuportedFileType : public ImageLoadingException {
    public:
        explicit ImageLoadingUnsuportedFileType(const std::string& fileName) noexcept : ImageLoadingException{fileName, "File " + fileName + " cannot be oppened - unsuported type"} {}
    };

}