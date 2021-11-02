#pragma once

#include "../Shape.hpp"

#include <array>

namespace ge {

    class Tetragon : public Shape {
    public:
        Tetragon(Color const& color) noexcept;
        // parallelogram
        Tetragon(Vector2f const& firstVertex,      Vector2f const& secondVertex,
                 Vector2f const& thirdVertex,      Color const& color = {}) noexcept;
        // rectangle parallel to the x and y axis
        Tetragon(Vector2f const& firstVertex,      Vector2f const& dimmensions,
                 Color const& color = {}) noexcept;
        Tetragon(Tetragon const& rectangle) noexcept;
        Tetragon(Tetragon&& rectangle) noexcept;

        Tetragon& operator= (Tetragon const& rectangle) noexcept;
        Tetragon& operator= (Tetragon&& rectangle) noexcept;

        void draw(void) const noexcept final;

        ~Tetragon(void) noexcept;
    private:
        typedef std::array<uint32_t, 6>             Indexes;

        static constexpr const Indexes              indexes{0, 1, 2, 0, 3, 2};

        uint32_t                                    elementArrayBuffer;

        void bindBuffers(void) const noexcept override;
        void unbindBuffers(void) const noexcept override;
    };

}
