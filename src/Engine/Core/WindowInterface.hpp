#pragma once


#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Mathematics/Vector.hpp"
#include "Color.hpp"

namespace ge {

    class WindowInterface {
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

        virtual ~WindowInterface(void) noexcept { glfwTerminate(); }
    protected:
        explicit WindowInterface(Vector2i dimmensions, std::string title, Options options = Options(), GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
        explicit WindowInterface(GLFWwindow* window) noexcept : window(window) {}
        bool shouldWindowClose(void) const noexcept  { return glfwWindowShouldClose(window); }
        void clear(const Color& color) const noexcept;
        void draw(void) const noexcept;
        constexpr const Vector2i& getWindowBaseDimmensions(void) const noexcept { return baseDimmensions; }
        constexpr const Vector2i&  getWindowDimmensions(void) const noexcept { return dimmensions; }
        constexpr const std::string& getWindowTitle(void) const noexcept { return name; }
    private:
        Vector2i baseDimmensions;
        Vector2i dimmensions;
        std::string name;
        GLFWwindow* window;
        void setWindowOptions(const Options&) const noexcept;
        void setCallbacks(void) noexcept;
    };

}