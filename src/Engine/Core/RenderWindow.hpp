#pragma once

#include <type_traits>

#include "Drawable.hpp"
#include "WindowInterface.hpp"
#include "Figures/ShaderLibrary.hpp"

namespace ge {

    class RenderWindow : virtual private WindowInterface {
    public:
        typedef std::shared_ptr<Drawable>           DrawablePtr;
        typedef std::vector<DrawablePtr>            Drawables;

        explicit RenderWindow(Vector2ui const& dimmensions,
            std::string const& title, Options options = Options(),
            GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

        RenderWindow(RenderWindow const& window) noexcept = delete;
        RenderWindow(RenderWindow&& window) noexcept = delete;

        RenderWindow& operator= (const RenderWindow& window) noexcept = delete;
        RenderWindow& operator= (RenderWindow&& window) noexcept = delete;

        using WindowInterface::getWindowDimmensions;
        using WindowInterface::getWindowTitle;
        using WindowInterface::setContextWindow;

        template <std::derived_from<Drawable> T>
        void pushDrawable(std::shared_ptr<T> const& drawable) noexcept;
        template <std::derived_from<Drawable> T>
        void pushDrawable(std::shared_ptr<T>&& drawable) noexcept;

        template <std::derived_from<Drawable> T, typename... Args>
            requires std::is_constructible_v<T, Args...>
        void emplaceDrawable(Args&&... args) noexcept;

        bool setFPSLimit(std::size_t fpsLimit) noexcept;
        void setTickrate(std::size_t ticks) noexcept;

        DrawablePtr const& operator[] (std::size_t index) const noexcept
            { return drawables[index]; }
        DrawablePtr& operator[] (std::size_t index) noexcept
            { return drawables[index]; }

        using iterator = typename Drawables::iterator;
        using const_iterator = typename Drawables::const_iterator;
        using reverse_iterator = typename Drawables::reverse_iterator;
        using const_reverse_iterator = typename Drawables::const_reverse_iterator;

        iterator begin(void) noexcept
            { return drawables.begin(); }
        iterator end(void) noexcept
            { return drawables.end(); }
        const_iterator begin(void) const noexcept
            { return drawables.begin(); }
        const_iterator end(void) const noexcept
            { return drawables.end(); }

        const_iterator cbegin(void) const noexcept
            { return drawables.cbegin(); }
        const_iterator cend(void) const noexcept
            { return drawables.cend(); }

        reverse_iterator rbegin(void) noexcept
            { return drawables.rbegin(); }
        reverse_iterator rend(void) noexcept
            { return drawables.rend(); }

        const_reverse_iterator crbegin(void) const noexcept
            { return drawables.crbegin(); }
        const_reverse_iterator crend(void) const noexcept
            { return drawables.crend(); }

        int32_t windowLoop(Color const& background = Color::literals::Black) noexcept;

        ShaderLibrary& getShaderLib(void) noexcept
            { return shaders; }

        ~RenderWindow(void) noexcept = default;
    private:
        typedef std::chrono::microseconds           Duration;
        typedef std::chrono::steady_clock           ThreadClock;
        typedef ThreadClock::time_point             TimePoint;

        void setDrawablesShaders(void) noexcept;
        void copyDrawablesToGPU(void) const noexcept;
        void drawDrawables(void) const noexcept;
        void draw(void) noexcept;
        void clear(Color const&) noexcept;

        ShaderLibrary                               shaders;
        Drawables                                   drawables;
        Duration                                    sleepTime;
        TimePoint                                   lastTime;
    };

    template <std::derived_from<Drawable> T, typename... Args>
        requires std::is_constructible_v<T, Args...>
    void RenderWindow::emplaceDrawable(Args&&... args) noexcept {
        auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
        events.addIfDerived(ptr);
        drawables.push_back(std::move(ptr));
    }

    template <std::derived_from<Drawable> T>
    void RenderWindow::pushDrawable(std::shared_ptr<T> const& drawable) noexcept {
        events.addIfDerived(drawable);
        drawables.push_back(std::static_pointer_cast<Drawable>(drawable));
    }

    template <std::derived_from<Drawable> T>
    void RenderWindow::pushDrawable(std::shared_ptr<T>&& drawable) noexcept {
        events.addIfDerived(drawable);
        drawables.push_back(std::static_pointer_cast<Drawable>(std::move(drawable)));
    }

}
