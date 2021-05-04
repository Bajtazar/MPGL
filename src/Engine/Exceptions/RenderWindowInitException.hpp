#pragma once

#include "RenderWindowException.hpp"

namespace ge {

    class RenderWindowInitException : public RenderWindowException {
    public:
        explicit RenderWindowInitException(std::string title) noexcept : RenderWindowException(title) { createMessage(); }
    private:
        virtual void createMessage(void) noexcept {
            using namespace std::literals;
            message = "Cannot initliazie GLEW under '"s + windowName + "'\n";
        }
    };

}
