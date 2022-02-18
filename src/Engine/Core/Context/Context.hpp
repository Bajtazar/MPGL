#pragma once

#include "../../Mathematics/Vector.hpp"
#include "ShadersContext.hpp"
#include "Options.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace mpgl {

    class WindowInterface;

    struct Context {
        explicit Context(void) noexcept;

        ShadersContext                  shaders;
        Vector2u                        windowDimensions;
        Options                         windowOptions;

        friend void errorCallback(int32 error,
            char const* message) noexcept;

        ~Context(void) noexcept;
    };

    class GraphicalObject {
    public:
        static Context                  context;

        static bool isCommunicationThread(void) noexcept
            { return communicationThread; }

        friend class WindowInterface;
    private:
        static_assert((context, true));

        static void setCommunicationThread(
            Vector2u const& dimensions,
            Options const& options) noexcept;
        static void resetCommunicationThread(void) noexcept
            { communicationThread = false; }

        thread_local static bool        communicationThread;
    };

}
