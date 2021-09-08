#include "BMPLoader.hpp"

#include "../../Exceptions/ImageLoadingFileOpenException.hpp"
#include "../../Exceptions/SecurityUnknownPolicyException.hpp"
#include "../../Exceptions/ImageLoadingInvalidTypeException.hpp"
#include "../../Exceptions/ImageLoadingFileCorruptionException.hpp"

namespace ge {

    template <security::SecurityPolicy Policy>
    const std::string BMPLoader<Policy>::Tag{"bmp"};

    template <security::SecurityPolicy Policy>
    BMPLoader<Policy>::BMPLoader([[maybe_unused]] Policy policy, const std::string& fileName)
        : LoaderInterface{std::move(fileName)}
    {
        std::ifstream file{this->fileName.c_str(), std::ios::binary};
        if (!file.good() || !file.is_open())
            throw ImageLoadingFileOpenException{this->fileName};
        FileIter iter = setPolicy(file);
        try {
            readHeader(iter);
            readImage(iter);
        } catch (std::out_of_range&) {
            throw ImageLoadingFileCorruptionException{this->fileName};
        }
    }

    template <security::SecurityPolicy Policy>
    BMPLoader<Policy>::BMPLoader(const std::string& fileName) : BMPLoader{Policy{}, fileName} {}

    template <security::SecurityPolicy Policy>
    BMPLoader<Policy>::FileIter BMPLoader<Policy>::setPolicy(std::istream& file) {
        if constexpr (security::isSecurePolicy<Policy>)
            return FileIter{StreamBuf{file}, StreamBuf{}};
        else if constexpr (security::isUnsecuredPolicy<Policy>)
            return FileIter{file};
        else
            throw SecurityUnknownPolicyException{};
    }

    template <security::SecurityPolicy Policy>
    void BMPLoader<Policy>::readHeader(FileIter& file) {
        if (readType<uint16_t>(file) != 0x4D42)
            throw ImageLoadingInvalidTypeException{fileName};
        readType<uint64_t>(file);   // file size and two reserved fields
        uint32_t offset = readType<uint32_t>(file) - 6 * sizeof(uint32_t) - sizeof(uint16_t);
        readType<uint32_t>(file);   // DIB header
        pixels.resize(readType<uint32_t>(file), readType<uint32_t>(file));
        for (uint32_t i = 0;i < offset; ++i)
            readType<std::byte>(file);
    }

    template <security::SecurityPolicy Policy>
    void BMPLoader<Policy>::readImage(FileIter& file) {
        for (auto row : pixels) {
            for (auto& pixel : row)
                Pixel::Manip::RGB(file, pixel);
            std::advance(file, (4 - pixels.getWidth() % 4) % 4);
        }
    }
}
