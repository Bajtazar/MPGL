#pragma once

#include "../Angular.hpp"

#include <array>

namespace mpgl {

    class Tetragon : public Angular {
    public:
        Tetragon(Color const& color = {});
        // parallelogram
        Tetragon(Vector2f const& firstVertex,
                Vector2f const& secondVertex,
                Vector2f const& thirdVertex,
                Color const& color = {});
        // rectangle parallel to the x and y axis
        Tetragon(Vector2f const& firstVertex,
                Vector2f const& dimensions,
                Color const& color = {});
        Tetragon(Tetragon const& tetragon);
        Tetragon(Tetragon&& tetragon) noexcept;

        Tetragon& operator= (Tetragon const& tetragon);
        Tetragon& operator= (Tetragon&& tetragon) noexcept;

        void draw(void) const noexcept final;

        ~Tetragon(void) noexcept;
    private:
        typedef std::array<uint32, 6>               Indexes;

        static constexpr const Indexes              indexes{0, 1, 2, 0, 3, 2};

        uint32                                      elementArrayBuffer;

        void moveTetragon(Tetragon& tetragon) noexcept;
        void generateBuffers(void) noexcept;
        void bindBuffers(void) const noexcept override;
        void unbindBuffers(void) const noexcept override;
    };

}
