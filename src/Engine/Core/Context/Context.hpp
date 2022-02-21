#pragma once

#include "../../Mathematics/Vector.hpp"
#include "ShadersContext.hpp"
#include "Options.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <atomic>

namespace mpgl {

    struct Context {
        explicit Context(void) noexcept;

        ShadersContext                              shaders;
        Vector2u                                    windowDimensions;
        Options                                     windowOptions;

        friend void errorCallback(int32 error,
            char const* message) noexcept;

        ~Context(void) noexcept;
    };

    class GraphicalObject {
    public:
        static Context                              context;

        static bool isCommunicationThread(void) noexcept;
    protected:
        static void setCommunicationThread(
            Vector2u const& dimensions,
            Options const& options) noexcept;
    private:
        typedef std::atomic<std::thread::id>        AtomicThreadID;

        static_assert((context, true));

        static AtomicThreadID                       communicationID;
    };

}
