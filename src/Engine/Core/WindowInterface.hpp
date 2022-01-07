#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Transformations/Transformable2D.hpp"
#include "../Mathematics/Vector.hpp"
#include "../Events/EventBus.hpp"
#include "Context/Context.hpp"
#include "Color.hpp"

namespace ge {

    class WindowInterface : protected GraphicalObject {
    public:
        // OpenGL Options
        struct Options {
            uint16 openGLMajorVersion;
            uint16 openGLMinorVersion;
            bool floating;
            bool maximised;
            bool resizable;
            Options(uint16 major=3,uint16 minor=3,bool floating=false,bool maximised=false,bool resizable=true) noexcept;
        };
        // Interface cannot be copied or moved
        explicit WindowInterface(void) noexcept = delete;
        WindowInterface(const WindowInterface& windowInterface) noexcept = delete;
        WindowInterface(WindowInterface&& windowInterface) noexcept = delete;
        WindowInterface& operator= (const WindowInterface& window) noexcept = delete;
        WindowInterface& operator= (WindowInterface&& window) noexcept = delete;

        friend void framebufferCallback(GLFWwindow* window, int32 width, int32 height) noexcept;
        friend void keyCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) noexcept;
        friend void textCallback(GLFWwindow* window, uint32 character) noexcept;
        friend void mousePosCallback(GLFWwindow* window, float64 xpos, float64 ypos) noexcept;
        friend void mouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) noexcept;

        virtual ~WindowInterface(void) noexcept;
    protected:
        explicit WindowInterface(Vector2u dimmensions, std::string title, Options options = Options(), GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
        explicit WindowInterface(GLFWwindow* window) noexcept : window(window) {}
        bool shouldWindowClose(void) const noexcept  { return glfwWindowShouldClose(window); }
        void clear(const Color& color) const noexcept;
        void draw(void) const noexcept;
        void setContextWindow(void) noexcept;

        const Vector2u& getWindowDimmensions(void) const noexcept { return dimmensions; }
        const std::string& getWindowTitle(void) const noexcept { return name; }

        EventBus                        events;
    private:
        Vector2u                       dimmensions;
        std::string                     name;
        GLFWwindow*                     window;

        void setWindowOptions(const Options&) const noexcept;
        void setCallbacks(void) noexcept;
        void setDimmensions(Vector2u const& dimm) noexcept;
    };

}
