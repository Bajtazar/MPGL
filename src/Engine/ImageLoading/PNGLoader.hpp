#pragma once

#include "LoaderInterface.hpp"

namespace ge {

    class PNGLoader : public LoaderInterface {
    public:
        explicit PNGLoader(const std::string& fileName);

        static const std::string Tag;

        ~PNGLoader(void) noexcept = default;
    private:
        virtual void readHeader(std::ifstream& file) final;
        virtual void readImage(std::ifstream& file) noexcept final;
    };

}
