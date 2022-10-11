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
#include <MPGL/Platform/Features/Windows/WindowPlatform.hpp>

namespace mpgl::platform {

    WindowPlatform::WindowPlatform(
        Vector2u dimensions,
        std::string title,
        Options const& options)
            : dimensions{std::move(dimensions)},
            options{options}, title{std::move(title)} {}

    void WindowPlatform::setDimensions(
        Vector2u const& dimensions) noexcept
    {
        const_cast<Vector2u&>(context.windowDimensions)
            = this->dimensions = dimensions;
    }

}