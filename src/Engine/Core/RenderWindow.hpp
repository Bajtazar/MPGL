#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Color.hpp"

namespace ge {

    class RenderWindow {
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
        // RenderWindow methods;
        explicit RenderWindow(int32_t width, int32_t height, std::string title, Options options = Options(), GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr) noexcept(false);
        RenderWindow(const RenderWindow& window) noexcept = delete;
        RenderWindow(RenderWindow&& window) noexcept;

        RenderWindow& operator= (const RenderWindow& window) noexcept = delete;
        RenderWindow& operator= (RenderWindow&& window) noexcept;

        operator bool() const noexcept { return glfwWindowShouldClose(windowHandler); }

        int32_t getWindowWidth(void) const noexcept { return width; }
        int32_t getWindowHeight(void) const noexcept { return height; }
        const std::string& getWindowTitle(void) const noexcept { return name; }

        void draw(void) noexcept;
        void clear(const Color& = Color::literals::Black) noexcept;

        ~RenderWindow(void) noexcept { glfwTerminate(); }

        friend void framebufferCallback(GLFWwindow* window, int width, int height) noexcept;
    private:
        int32_t width;
        int32_t height;
        std::string name;
        GLFWwindow* windowHandler;
        void setWindowOptions(const Options&) const noexcept;
        void setCallbacks(void) noexcept;
    };

}
