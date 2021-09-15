#pragma once

#include <type_traits>

#include "WindowInterface.hpp"
#include "Figures/Drawable.hpp"
#include "Figures/ShaderLibrary.hpp"
#include "Registers/TickRegister.hpp"

#include <chrono>

namespace ge {

    class RenderWindow : virtual private WindowInterface {
    public:
        explicit RenderWindow(const Vector2ui& dimmensions, std::string title, Options options = Options(), GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr) noexcept(false);
        RenderWindow(const RenderWindow& window) noexcept = delete;
        RenderWindow(RenderWindow&& window) noexcept = delete;

        RenderWindow& operator= (const RenderWindow& window) noexcept = delete;
        RenderWindow& operator= (RenderWindow&& window) noexcept = delete;

        operator bool() const noexcept { return shouldWindowClose(); }

        const std::shared_ptr<Vector2ui>& getWindowDimmensions(void) const noexcept { return WindowInterface::getWindowDimmensions(); }
        const std::string& getWindowTitle(void) const noexcept { return WindowInterface::getWindowTitle(); }

        void pushDrawable(const std::shared_ptr<Drawable>& drawable) noexcept;
        void pushDrawable(std::shared_ptr<Drawable>&& drawable) noexcept;
        template <std::derived_from<Drawable> T, typename... Args>
            requires std::is_constructible_v<T, const std::shared_ptr<Vector2ui>&, Args...>
        void emplaceDrawable(Args&&... args) noexcept;

        void setDrawablesShaders(void) noexcept;
        void copyDrawablesToGPU(void) const noexcept;
        void drawDrawables(void) const noexcept;

        bool setFPSLimit(std::size_t fpsLimit) noexcept;

        const std::shared_ptr<Drawable>& operator[] (std::size_t index) const noexcept { return drawables[index]; }
        std::shared_ptr<Drawable>& operator[] (std::size_t index) noexcept { return drawables[index]; }

        using iterator = std::vector<std::shared_ptr<Drawable>>::iterator;
        using const_iterator = std::vector<std::shared_ptr<Drawable>>::const_iterator;
        using reverse_iterator = std::vector<std::shared_ptr<Drawable>>::reverse_iterator;
        using const_reverse_iterator = std::vector<std::shared_ptr<Drawable>>::const_reverse_iterator;

        iterator begin(void) noexcept { return drawables.begin(); }
        iterator end(void) noexcept { return drawables.end(); }
        const_iterator begin(void) const noexcept { return drawables.begin(); }
        const_iterator end(void) const noexcept { return drawables.end(); }

        const_iterator cbegin(void) const noexcept { return drawables.cbegin(); }
        const_iterator cend(void) const noexcept { return drawables.cend(); }

        reverse_iterator rbegin(void) noexcept { return drawables.rbegin(); }
        reverse_iterator rend(void) noexcept { return drawables.rend(); }

        const_reverse_iterator crbegin(void) const noexcept { return drawables.crbegin(); }
        const_reverse_iterator crend(void) const noexcept { return drawables.crend(); }

        int32_t windowLoop(const Color& background = Color::literals::Black) noexcept;

        void draw(void) noexcept;
        void clear(const Color& = Color::literals::Black) noexcept;
        ShaderLibrary& getShaderLib(void) noexcept { return shaders; }

        ~RenderWindow(void) noexcept = default;
    private:
        ShaderLibrary shaders;
        std::vector<std::shared_ptr<Drawable>> drawables;
        std::chrono::microseconds sleepTime;
        std::chrono::steady_clock::time_point lastTime;
    };

    template <std::derived_from<Drawable> T, typename... Args>
        requires std::is_constructible_v<T, const std::shared_ptr<Vector2ui>&, Args...>
    void RenderWindow::emplaceDrawable(Args&&... args) noexcept {
        auto ptr = std::make_shared<T>(WindowInterface::getWindowDimmensions(), std::forward<Args>(args)...);
        if constexpr (std::is_base_of_v<Transformable, T>)
            transformables.push_back(std::static_pointer_cast<Transformable>(ptr));
        drawables.push_back(std::move(ptr));
    }

}
