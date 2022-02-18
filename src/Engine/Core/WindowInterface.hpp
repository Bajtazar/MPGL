#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Transformations/Transformable2D.hpp"
#include "../Mathematics/Vector.hpp"
#include "../Events/EventBus.hpp"
#include "Context/Context.hpp"
#include "Color.hpp"

namespace mpgl {

    class WindowInterface : protected GraphicalObject {
    public:
        WindowInterface(void) noexcept = delete;

        WindowInterface(
            WindowInterface const& windowInterface) noexcept = delete;
        WindowInterface(
            WindowInterface&& windowInterface) noexcept = delete;

        WindowInterface& operator= (
            WindowInterface const& window) noexcept = delete;
        WindowInterface& operator= (
            WindowInterface&& window) noexcept = delete;

        friend void framebufferCallback(
            GLFWwindow* window, int32 width, int32 height) noexcept;

        friend void keyCallback(
            GLFWwindow* window, int32 key, int32 scancode,
            int32 action, int32 mods) noexcept;

        friend void textCallback(
            GLFWwindow* window, uint32 character) noexcept;

        friend void mousePosCallback(
            GLFWwindow* window, float64 xpos, float64 ypos) noexcept;

        friend void mouseButtonCallback(
            GLFWwindow* window, int32 button,
            int32 action, int32 mods) noexcept;

        friend void windowCloseCallback(
            GLFWwindow* window) noexcept;

        virtual ~WindowInterface(void) noexcept;
    protected:
        explicit WindowInterface(Vector2u dimensions,
            std::string title, Options const& options = Options{});
        explicit WindowInterface(
            GLFWwindow* window) noexcept : window(window) {}

        bool shouldWindowClose(void) const noexcept
            { return glfwWindowShouldClose(window); }

        void clear(Color const& color) const noexcept;
        void draw(void) const noexcept;
        void setContextWindow(void) noexcept;
        void closeWindow(void) noexcept;
        void openWindow(void) noexcept;

        Vector2u const& getWindowDimensions(void) const noexcept
            { return dimensions; }
        std::string const& getWindowTitle(void) const noexcept
            { return name; }

        EventBus                        events;
    private:
        Vector2u                        dimensions;
        Options                         options;
        std::string                     name;
        GLFWwindow*                     window;

        void setWindowOptions(void) const noexcept;
        void setCallbacks(void) noexcept;
        void setDimensions(Vector2u const& dimm) noexcept;
    };

}
