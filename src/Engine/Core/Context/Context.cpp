#include "Context.hpp"

#include <stdlib.h>
#include <iostream>

namespace ge {

    void errorCallback(int32 error, char const* message) noexcept {
        std::cout << "Error: " << error << '\n';
        std::cout << "Description: " << message << '\n';
    }

    thread_local Context GraphicalObject::context{};

    GraphicalObject::GraphicalObject(void) noexcept {
        // forces context initialization
        ++context.objects;
    }

    GraphicalObject::~GraphicalObject(void) noexcept {
        --context.objects;
    }

    Context::Context(void) noexcept {
        if (!glfwInit())
            std::terminate();
        glfwSetErrorCallback(errorCallback);
    }

    Context::~Context(void) noexcept {
        glfwTerminate();
    }

}