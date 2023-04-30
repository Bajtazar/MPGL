/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

#include <MPGL/Platform/Features/Windows/WindowPlatform.hpp>

#include <memory>

namespace mpgl::platform {

    /**
     * Handler that allows to derived from the object containing
     * the window implementation thus enforcing the initialization
     * and destruction order
     */
    class PlatformHandler {
    public:
        using WindowPtr = std::unique_ptr<WindowPlatform>;

        /**
         * Constructs a new platfrom handler object
         *
         * @param windowPtr a rvalue reference to the window's
         * unique pointer
         */
        explicit PlatformHandler(WindowPtr&& windowPtr) noexcept
            : windowImpl{std::move(windowPtr)} {}

        PlatformHandler(PlatformHandler const& window) noexcept = delete;
        PlatformHandler(PlatformHandler&& window) noexcept = default;

        PlatformHandler& operator= (
            PlatformHandler const& window) noexcept = delete;
        PlatformHandler& operator= (
            PlatformHandler&& window) noexcept = default;

        /**
         * Destroys the platfrom handler object
         */
        ~PlatformHandler(void) noexcept = default;
    protected:
        WindowPtr                       windowImpl;
    };

}
