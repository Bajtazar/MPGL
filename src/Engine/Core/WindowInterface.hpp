#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Transformations/Transformable2D.hpp"
#include "../Mathematics/Vector.hpp"
#include "../Events/EventBus.hpp"
#include "Context.hpp"
#include "Color.hpp"

namespace ge {

    class WindowInterface : private GraphicalObject {
    public:
        // OpenGL Options
        struct Options {
            uint16_t openGLMajorVersion;
            uint16_t openGLMinorVersion;
            bool floating;
            bool maximised;
            bool resizable;
            Options(uint16_t major=3,uint16_t minor=3,bool floating=false,bool maximised=false,bool resizable=true) noexcept;
        };
        // Interface cannot be copied or moved
        explicit WindowInterface(void) noexcept = delete;
        WindowInterface(const WindowInterface& windowInterface) noexcept = delete;
        WindowInterface(WindowInterface&& windowInterface) noexcept = delete;
        WindowInterface& operator= (const WindowInterface& window) noexcept = delete;
        WindowInterface& operator= (WindowInterface&& window) noexcept = delete;

        friend void framebufferCallback(GLFWwindow* window, int32_t width, int32_t height) noexcept;
        friend void keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
        friend void textCallback(GLFWwindow* window, uint32_t character);
        friend void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
        friend void mouseButtonCallback(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);

        virtual ~WindowInterface(void) noexcept = default;
    protected:
        explicit WindowInterface(Vector2ui dimmensions, std::string title, Options options = Options(), GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
        explicit WindowInterface(GLFWwindow* window) noexcept : window(window) {}
        bool shouldWindowClose(void) const noexcept  { return glfwWindowShouldClose(window); }
        void clear(const Color& color) const noexcept;
        void draw(void) const noexcept;
        void setContextWindow(void) noexcept;

        const Vector2ui& getWindowDimmensions(void) const noexcept { return dimmensions; }
        const std::string& getWindowTitle(void) const noexcept { return name; }

        EventBus                        events;
    private:
        Vector2ui                       dimmensions;
        std::string                     name;
        GLFWwindow*                     window;

        void setWindowOptions(const Options&) const noexcept;
        void setCallbacks(void) noexcept;
        void setDimmensions(Vector2ui const& dimm) noexcept;
    };

}
