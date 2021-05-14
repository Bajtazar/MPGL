#pragma once

#include <string>

#include "WindowInterface.hpp"
#include "Figures/ShaderLibrary.hpp"

namespace ge {

    class RenderWindow : virtual private WindowInterface {
    public:
        // RenderWindow methods;
        explicit RenderWindow(const Vector2i& dimmensions, std::string title, Options options = Options(), GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr) noexcept(false);
        RenderWindow(const RenderWindow& window) noexcept = delete;
        RenderWindow(RenderWindow&& window) noexcept = delete;

        RenderWindow& operator= (const RenderWindow& window) noexcept = delete;
        RenderWindow& operator= (RenderWindow&& window) noexcept = delete;

        operator bool() const noexcept { return shouldWindowClose(); }

        const Vector2i& getWindowBaseDimmensions(void) const noexcept { return WindowInterface::getWindowBaseDimmensions(); }
        const Vector2i& getWindowDimmensions(void) const noexcept { return WindowInterface::getWindowDimmensions(); }
        const std::string& getWindowTitle(void) const noexcept { return WindowInterface::getWindowTitle(); }

        void draw(void) noexcept;
        void clear(const Color& = Color::literals::Black) noexcept;
        ShaderLibrary& getShaderLib(void) noexcept { return shaders; }

        ~RenderWindow(void) noexcept = default;
    private:
        ShaderLibrary shaders;
    };

}
