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
        template <typename T>
        T decode(std::ifstream& file) const noexcept;

        virtual void readHeader(std::ifstream& file) final;
        virtual void readImage(std::ifstream& file) noexcept final;
    };

    template <typename T>
    T BMPLoader::decode(std::ifstream& file) const noexcept {
        T data;
        file >> data;
        return data;
    }

}
