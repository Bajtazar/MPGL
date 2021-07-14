#pragma once

#include "LoaderInterface.hpp"
#include "../Exceptions/ImageLoadingFileException.hpp"

#include <functional>

namespace ge {

    class BMPLoader : public LoaderInterface {
    public:
        explicit BMPLoader(const std::string& fileName);

        static const std::string Tag;

        ~BMPLoader(void) noexcept = default;
    private:
        virtual void readHeader(std::ifstream& file) final;
        virtual void readImage(std::ifstream& file) noexcept final;
    };

}
