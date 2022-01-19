#pragma once

#include <type_traits>

#include "Drawable.hpp"
#include "WindowInterface.hpp"
#include "Shaders/ShaderLibrary.hpp"

namespace ge {

    class Window : virtual private WindowInterface {
    public:
        typedef std::shared_ptr<Drawable>           DrawablePtr;
        typedef std::vector<DrawablePtr>            Drawables;

        explicit Window(Vector2u const& dimmensions,
            std::string const& title,
            Options const& options = Options{});

        Window(Window const& window) noexcept = delete;
        Window(Window&& window) noexcept = delete;

        Window& operator= (const Window& window) noexcept = delete;
        Window& operator= (Window&& window) noexcept = delete;

        using WindowInterface::getWindowDimmensions;
        using WindowInterface::getWindowTitle;
        using WindowInterface::openWindow;
        using WindowInterface::closeWindow;

        void setContextWindow(void) noexcept;

        template <std::derived_from<EventBase> Tp>
        void pushEvent(std::shared_ptr<Tp> const& event);

        template <std::derived_from<EventBase> Tp, typename... Args>
            requires std::constructible_from<Tp, Args...>
        void emplaceEvent(Args&&... args);

        template <std::derived_from<Drawable> T>
        void pushDrawable(std::shared_ptr<T> const& drawable) noexcept;
        template <std::derived_from<Drawable> T>
        void pushDrawable(std::shared_ptr<T>&& drawable) noexcept;

        template <std::derived_from<Drawable> T, typename... Args>
            requires std::constructible_from<T, Args...>
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

        int32 windowLoop(Color const& background = Color::Black) noexcept;

        ShaderLibrary& getShaderLib(void) noexcept
            { return shaders; }

        ~Window(void) noexcept;
    private:
        typedef std::chrono::microseconds           Duration;
        typedef std::chrono::steady_clock           ThreadClock;
        typedef ThreadClock::time_point             TimePoint;

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
        requires std::constructible_from<T, Args...>
    void Window::emplaceDrawable(Args&&... args) noexcept {
        auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
        events.addIfDerived(ptr);
        drawables.push_back(std::move(ptr));
    }

    template <std::derived_from<Drawable> T>
    void Window::pushDrawable(std::shared_ptr<T> const& drawable) noexcept {
        events.addIfDerived(drawable);
        drawables.push_back(std::static_pointer_cast<Drawable>(drawable));
    }

    template <std::derived_from<Drawable> T>
    void Window::pushDrawable(std::shared_ptr<T>&& drawable) noexcept {
        events.addIfDerived(drawable);
        drawables.push_back(std::static_pointer_cast<Drawable>(std::move(drawable)));
    }

    template <std::derived_from<EventBase> Tp>
    void Window::pushEvent(std::shared_ptr<Tp> const& event) {
        events.addIfDerived(event);
    }

    template <std::derived_from<EventBase> Tp, typename... Args>
        requires std::constructible_from<Tp, Args...>
    void Window::emplaceEvent(Args&&... args) {
        events.addIfDerived(std::make_shared<Tp>(
            std::forward<Args>(args)...));
    }

}
