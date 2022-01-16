#pragma once

#include "../../Mathematics/Vector.hpp"
#include "ShadersContext.hpp"
#include "Options.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ge {

    struct Context {
        explicit Context(void) noexcept;

        ShadersContext                  shaders;
        Vector2u                        windowDimmensions;
        Options                         windowOptions;
        uint32                          objects;

        friend void errorCallback(int32 error,
            char const* message) noexcept;

        ~Context(void) noexcept;
    };

    struct GraphicalObject {
        explicit GraphicalObject(void) noexcept;

        ~GraphicalObject(void) noexcept;

        thread_local static Context    context;
    };

}
