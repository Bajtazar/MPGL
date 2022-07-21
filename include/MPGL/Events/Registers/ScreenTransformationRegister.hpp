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

#include <MPGL/Events/Types/ScreenTransformationEvent.hpp>
#include <MPGL/Events/Registers/RegisterInterface.hpp>

#include <vector>
#include <memory>

namespace mpgl {

    /**
     * Register that holds the screen transformation events
     */
    struct ScreenTransformationRegister
        : public RegisterInterface<ScreenTransformationEvent>
    {
        /**
         * Constructs a new Screen Transformation Register object
         */
        explicit ScreenTransformationRegister(
            void) noexcept = default;

        ScreenTransformationRegister(
            ScreenTransformationRegister&&) noexcept = default;

        ScreenTransformationRegister& operator=(
            ScreenTransformationRegister&&) noexcept = default;

        /**
         * Calls the screen transformation event on the underlying
         * objects
         *
         * @param layout the reference to the layout
         * @param oldDimensions the constant reference to window's
         * old dimensions
         */
        void onEvent(
            Layout& layout,
            Vector2u const& oldDimensions) noexcept;

        /**
         * Calls the screen transformation event on the underlying
         * objects and distributes the layouts between them
         *
         * @param layouts the reference to the layout vector
         * @param oldDimensions the constant reference to window's
         * old dimensions
         */
        void onEvent(
            std::vector<std::unique_ptr<Layout>>& layouts,
            Vector2u const& oldDimensions) noexcept;

        /**
         * Destroys the Screen Transformation Register object
         */
        ~ScreenTransformationRegister(void) noexcept = default;
    };

}
