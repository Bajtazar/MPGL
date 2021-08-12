#pragma once

#include "../Collections/SafeIterator.hpp"
#include "LoaderInterface.hpp"

namespace ge {

    class BMPLoader : public LoaderInterface {
    public:
        explicit BMPLoader(const std::string& fileName);

        static const std::string Tag;

        ~BMPLoader(void) noexcept = default;
    private:
        typedef std::istreambuf_iterator<char>      StreamBuf;
        typedef SafeIterator<StreamBuf>             FileIter;

        void readHeader(FileIter& file);
        void readImage(FileIter& file);
    };

}
