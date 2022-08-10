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

namespace mpgl {

    template <std::derived_from<Camera> CameraTp>
    void Window::addCameraEventIfDerived(
        std::shared_ptr<CameraTp> const& camera) noexcept
    {
        if constexpr (Event<CameraTp>)
            eventManager->push(camera);
    }

    template <std::derived_from<Camera> CameraTp>
    void Window::windowLoop(
        Matrix4f const& projection,
        Color const& background,
        std::shared_ptr<CameraTp> cameraPtr) noexcept
    {
        addCameraEventIfDerived(cameraPtr);
        openWindow();
        while (!shouldWindowClose()) {
            clear(background);
            eventManager->onTick();
            setVPMatrix(cameraPtr);
            draw3DDrawables();
            draw2DDrawables();
            draw();
        }
    }

}
