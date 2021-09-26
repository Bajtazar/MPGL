#pragma once

#include "../Mathematics/Vector.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ge {

    struct Context {
        explicit Context(void) noexcept;

        Vector2ui                       windowDimmensions;

        ~Context(void) noexcept;
    };

    struct GraphicalObject {
        thread_local static Context     context;
    };

}
