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

#include <MPGL/Mathematics/Tensors/Matrix.hpp>
#include <MPGL/Core/Context/Context.hpp>

namespace mpgl {

    /**
     * Interface for the camera classes. Calculates a view
     * matrix of the camera
     */
    class Camera : protected GraphicalObject {
    public:
        /**
         * Constructs a new camera object
         */
        explicit Camera(void) noexcept = default;

        /**
         * Pure virtual method. Has to be overloaded. Calculates
         * the camera's view matrix
         *
         * @return the camera's view matrix
         */
        [[nodiscard]] virtual Matrix4f operator() (
            void) const noexcept = 0;

        /**
         * Destroys the camera object
         */
        virtual ~Camera(void) noexcept = default;
    protected:
        Camera(Camera const&) noexcept = default;
        Camera(Camera&&) noexcept = default;

        Camera& operator=(Camera const&) noexcept = default;
        Camera& operator=(Camera&&) noexcept = default;
    };

}
