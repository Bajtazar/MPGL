#include "Context.hpp"

#include <stdlib.h>
#include <iostream>

namespace ge {

    void errorCallback(int32 error, char const* message) noexcept {
        std::cout << "Error: " << error << '\n';
        std::cout << "Description: " << message << '\n';
    }

    Context GraphicalObject::context{};

    thread_local bool GraphicalObject::communicationThread = false;

    Context::Context(void) noexcept {
        if (!glfwInit())
            std::terminate();
        glfwSetErrorCallback(errorCallback);
    }

    Context::~Context(void) noexcept {
        glfwTerminate();
    }

    void GraphicalObject::setCommunicationThread(
        Vector2u const& dimensions,
        Options const& options) noexcept
    {
        context.windowDimmensions = dimensions;
        context.windowOptions = options;
        communicationThread = true;
    }

}
