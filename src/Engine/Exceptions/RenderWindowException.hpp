#pragma once

#include <exception>
#include <string>

namespace ge{

    class RenderWindowException : public std::exception {
    public:
        explicit RenderWindowException(std::string windowName) noexcept : windowName(windowName) {}
        virtual const char* what (void) const noexcept{ return message.c_str(); }
        const std::string& getMessage(void) const noexcept { return message; }
        const std::string& getWindowName(void) const noexcept { return windowName; }

        virtual ~RenderWindowException(void) noexcept = default;
    protected:
        std::string windowName;
        std::string message;
        virtual void createMessage(void) noexcept = 0;
    };

}
