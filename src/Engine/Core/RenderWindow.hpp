#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace ge {

    class RenderWindow{
    public:
        explicit RenderWindow(int32_t width, int32_t height, std::string title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr) noexcept(false);
        RenderWindow(const RenderWindow& window) noexcept = delete;
        RenderWindow(RenderWindow&& window) noexcept;

        RenderWindow& operator= (const RenderWindow& window) noexcept = delete;
        RenderWindow& operator= (RenderWindow&& window) noexcept;

        operator bool() const noexcept { return glfwWindowShouldClose(windowHandler); }

        int32_t getWindowWidth(void) const noexcept { return width; }
        int32_t getWindowHeight(void) const noexcept { return height; }
        const std::string& getWindowTitle(void) const noexcept { return name; }

        void draw(void) noexcept;
        void clear(void) noexcept;

        ~RenderWindow(void) noexcept { glfwTerminate(); }
    private:
        int32_t width;
        int32_t height;
        std::string name;
        GLFWwindow* windowHandler;
    };

}
