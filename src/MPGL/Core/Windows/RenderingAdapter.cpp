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
#include <MPGL/Core/Windows/RenderingAdapter.hpp>

namespace mpgl {

    void RenderingAdapter::onScreenTransformation(
        Vector2u const& oldDimensions) noexcept
    {
        reference.get().onScreenTransformation(oldDimensions);
    }

    void RenderingAdapter::onMouseRelease(
        MouseButton const& button) noexcept
    {
        reference.get().onMouseRelease(button);
    }

    void RenderingAdapter::onWindowClose(void) noexcept {
        reference.get().onWindowClose();
    }

    void RenderingAdapter::onMouseMotion(
        Vector2f const& position) noexcept
    {
        reference.get().onMouseMotion(position);
    }

    void RenderingAdapter::onMousePress(
        MouseButton const& button) noexcept
    {
        reference.get().onMousePress(button);
    }

    void RenderingAdapter::onKeyRelease(Key const& key) noexcept {
        reference.get().onKeyRelease(key);
    }

    void RenderingAdapter::onTextWrite(
        std::string const& unicodeString) noexcept
    {
        reference.get().onTextWrite(unicodeString);
    }

    void RenderingAdapter::onKeyPress(Key const& key) noexcept {
        reference.get().onKeyPress(key);
    }

    void RenderingAdapter::onScroll(Vector2f const& scroll) noexcept {
        reference.get().onScroll(scroll);
    }

    void RenderingAdapter::onTick(
        std::chrono::milliseconds const& delta) noexcept
    {
        reference.get().render(cleaning, background);
        reference.get().onTick(delta);
    }

}
