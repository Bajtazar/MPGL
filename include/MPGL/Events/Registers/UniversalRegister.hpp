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

#include <algorithm>

#include <MPGL/Events/Registers/RegisterInterface.hpp>
#include <MPGL/Events/Event.hpp>

namespace mpgl {

    template <Event Tp, typename Signature, Signature Method>
    class UniversalRegister;

    /**
     * Register that allows to construct the simple register for
     * cutom event
     */
    template <Event Tp, typename... Args,
        void(Tp::*EventMethod)(Args...)>
    struct UniversalRegister<Tp, void(Tp::*)(Args...), EventMethod>
        : public RegisterInterface<Tp>
    {
        /**
         * Constructs a new Universal Register object
         */
        explicit UniversalRegister(void) noexcept = default;

        UniversalRegister(UniversalRegister&&) = default;

        UniversalRegister& operator=(UniversalRegister&&) = default;

        /**
         * Calls the event on the underlying objects with
         * the given arguments
         *
         * @param args the event arguments
         */
        void onEvent(Args&&... args) noexcept;

        /**
         * Destroys the Universal Register object
         */
        ~UniversalRegister(void) noexcept = default;
    };

}

#include <MPGL/Events/Registers/UniversalRegister.tpp>
