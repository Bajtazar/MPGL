#pragma once

#include "RenderWindowException.hpp"

namespace ge {

    class RenderWindowInitException : public RenderWindowException {
    public:
        explicit RenderWindowInitException(std::string title) noexcept : RenderWindowException(title) { createMessage(); }
    private:
        virtual void createMessage(void) noexcept {
            using namespace std::literals;
            message = "Cannot link GLEW library when window '"s + windowName + "' is being initialized\n";
        }
    };

}
