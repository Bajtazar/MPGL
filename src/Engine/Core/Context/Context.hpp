#pragma once

#include "../../Mathematics/Vector.hpp"
#include "ShadersContext.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ge {

    struct Context {
        explicit Context(void) noexcept;

        Vector2ui                       windowDimmensions;
        ShadersContext                  shaders;

        ~Context(void) noexcept;
    };

    struct GraphicalObject {
        thread_local static Context     context;
    };

}
