#include "Context.hpp"

#include <stdlib.h>

namespace ge {

    thread_local Context GraphicalObject::context{};

    Context::Context(void) noexcept {
        if (!glfwInit())
            std::terminate();
    }

    Context::~Context(void) noexcept {
        glfwTerminate();
    }

}