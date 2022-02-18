#include "Context.hpp"

#include <stdlib.h>
#include <iostream>

namespace mpgl {

    void errorCallback(int32 error, char const* message) noexcept {
        std::cout << "Error: " << error << '\n';
        std::cout << "Description: " << message << '\n';
    }

    Context GraphicalObject::context{};

    GraphicalObject::AtomicThreadID GraphicalObject::communicationID{};

    Context::Context(void) noexcept {
        if (!glfwInit())
            std::terminate();
        glfwSetErrorCallback(errorCallback);
    }

    Context::~Context(void) noexcept {
        glfwTerminate();
    }

    bool GraphicalObject::isCommunicationThread(void) noexcept {
        return communicationID.load(std::memory_order_relaxed)
            == std::this_thread::get_id();
    }

    void GraphicalObject::setCommunicationThread(
        Vector2u const& dimensions,
        Options const& options) noexcept
    {
        context.windowDimensions = dimensions;
        context.windowOptions = options;
        communicationID.store(std::this_thread::get_id(),
            std::memory_order_relaxed);
    }

}
