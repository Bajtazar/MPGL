#pragma once

#include "RenderWindowException.hpp"

namespace ge {

    class RenderWindowGladException : public RenderWindowException {
    public:
        explicit RenderWindowGladException(std::string title) noexcept : RenderWindowException(title) { createMessage(); }
    private:
        virtual void createMessage(void) noexcept {
            using namespace std::literals;
            message = "Cannot initialize GLAD when widnow '"s + windowName +"' is being initialized";
        }
    };

}
