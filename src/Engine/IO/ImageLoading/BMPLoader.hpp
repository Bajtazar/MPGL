#pragma once

#include "../../Iterators/SafeIterator.hpp"
#include "../../Utility/Security.hpp"
#include "LoaderInterface.hpp"

namespace mpgl {

    template <security::SecurityPolicy Policy = Secured>
    class BMPLoader : public LoaderInterface {
    public:
        explicit BMPLoader(const std::string& fileName);
        explicit BMPLoader(Policy policy, const std::string& fileName);

        static const std::string Tag;

        ~BMPLoader(void) noexcept = default;
    private:
        typedef std::istreambuf_iterator<char>      StreamBuf;
        typedef std::conditional_t<security::isSecurePolicy<Policy>,
            SafeIterator<StreamBuf>, StreamBuf>     FileIter;

        FileIter setPolicy(std::istream& file);
        void readHeader(FileIter& file);
        void readImage(FileIter& file);
    };

    template class BMPLoader<Secured>;
    template class BMPLoader<Unsecured>;

}
