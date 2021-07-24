#pragma once

#include "LoaderInterface.hpp"
#include "../Utility/FunctionalWrapper.hpp"

#include <map>
#include <functional>

namespace ge {

    class PNGLoader : public LoaderInterface {
    public:
        explicit PNGLoader(const std::string& fileName);

        static const std::string Tag;

        ~PNGLoader(void) noexcept = default;
    private:
        void readImage(std::ifstream& file);

        struct ChunkInterface {
            explicit ChunkInterface(void) = default;
            virtual void operator() (std::size_t length, std::istream& data, PNGLoader&) = 0;
            virtual ~ChunkInterface(void) = default;
        };

        struct IHDRChunk : ChunkInterface {
            explicit IHDRChunk(void) = default;
            virtual void operator() (std::size_t length, std::istream& data, PNGLoader&) final;
            ~IHDRChunk(void) = default;
        };

        static std::map<std::string, std::function<std::unique_ptr<ChunkInterface> (void)>> chunkParsers;
    };

}
