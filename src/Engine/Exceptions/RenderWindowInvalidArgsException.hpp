#pragma once

#include "RenderWindowException.hpp"

namespace ge {

    class RenderWindowInvalidArgsException : public RenderWindowException {
    public:
        explicit RenderWindowInvalidArgsException(std::string title) noexcept : RenderWindowException(title) { createMessage(); }

        ~RenderWindowInvalidArgsException(void) noexcept = default;
    private:
        virtual void createMessage(void) noexcept {
            using namespace std::literals;
            message = "Invalid arguments at RenderWindow '"s + windowName + "'\n";
        }
    };

}
