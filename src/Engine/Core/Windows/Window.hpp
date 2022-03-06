/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

#include "WindowPlatform.hpp"

namespace mpgl {

    class Window : public WindowPlatform {
    public:
        explicit Window(Vector2u const& dimensions,
            std::string const& title,
            Options const& options = Options{});

        Window(Window const& window) noexcept = delete;
        Window(Window&& window) noexcept = delete;

        Window& operator= (const Window& window) noexcept = delete;
        Window& operator= (Window&& window) noexcept = delete;

        void setContextWindow(void) noexcept;

        bool setFPSLimit(std::size_t fpsLimit) noexcept;
        void setTickrate(std::size_t ticks) noexcept;

        int32 windowLoop(Color const& background = Color::Black) noexcept;

        [[nodiscard]] ShaderLibrary& getShaderLib(void) noexcept
            { return shaders; }

        ~Window(void) noexcept;
    private:
        typedef std::chrono::microseconds           Duration;
        typedef std::chrono::steady_clock           ThreadClock;
        typedef ThreadClock::time_point             TimePoint;

        void drawDrawables(void) const noexcept;
        void draw(void) noexcept;
        void clear(Color const&) noexcept;

        ShaderLibrary                               shaders;
        Duration                                    sleepTime;
        TimePoint                                   lastTime;
    };


}
