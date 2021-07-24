#pragma once

#include "LoaderInterface.hpp"

namespace ge {

    class BMPLoader : public LoaderInterface {
    public:
        explicit BMPLoader(const std::string& fileName);

        static const std::string Tag;

        ~BMPLoader(void) noexcept = default;
    private:
        void readHeader(std::ifstream& file);
        void readImage(std::ifstream& file) noexcept;
    };

}
