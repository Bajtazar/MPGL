#pragma once

#include "../Shaders/Shadeable.hpp"
#include "../Transformations/Transformable2D.hpp"

namespace ge {

    class Figure : public Shadeable, public Transformable2D {
    public:
        virtual void copyToGPU(void) const noexcept = 0;
        virtual void draw(void) const noexcept = 0;

        virtual void translate(
            Vector2f const& shift) noexcept = 0;
        virtual void scale(Vector2f const& center,
            float32 factor) noexcept = 0;
        virtual void rotate(Vector2f const& center,
            float32 angle) noexcept = 0;
        virtual void rotate(Vector2f const& center,
            Matrix2f const& rotation) noexcept = 0;
        virtual void onScreenTransformation(
            Vector2u const& oldDimmensions) noexcept = 0;

        virtual ~Figure(void) noexcept;
    protected:
        explicit Figure(std::string const& programName);
        explicit Figure(std::string const& programName,
            Executable exec);
        explicit Figure(void) noexcept;

        explicit Figure(Figure const& shape);
        explicit Figure(Figure&& shape) noexcept;

        Figure& operator=(Figure const& shape);
        Figure& operator=(Figure&& shape) noexcept;

        uint32                              vertexBuffer;
        uint32                              vertexArrayObject;
    private:
        void init(void) noexcept;
    };

}
