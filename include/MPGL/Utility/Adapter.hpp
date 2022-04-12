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

#include <memory>

#include <MPGL/Core/Context/Context.hpp>
#include <MPGL/Traits/Concepts.hpp>

namespace mpgl {

    /**
     * Holds a value normalized by the window dimensions and
     * perform rescaling operations during access
     *
     * @tparam Tp the underlying value type
     */
    template <Adaptable Tp>
    class Adapter : private GraphicalObject {
    public:
        typedef typename Tp::value_type value_type;

        /**
         * Construct a new Adapter object. Normalizes
         * the given range
         *
         * @param range the given range constant reference
         */
        Adapter(Tp const& range) noexcept;

        /**
         * Construct a new Adapter object. Normalizes
         * the given range
         *
         * @param range the given range rvalue
         */
        Adapter(Tp&& range) noexcept;

        Adapter(Adapter const&) = default;
        Adapter(Adapter&&) noexcept = default;

        Adapter& operator=(Adapter const&) = default;
        Adapter& operator=(Adapter&&) noexcept = default;

        /**
         * Assigns the given value to the adapter object
         *
         * @param value the given value contant reference
         */
        void operator= (Tp const& value) noexcept;

        /**
         * Assigns the given value to the adapter object
         *
         * @param value the given value rvalue
         */
        void operator= (Tp&& value) noexcept;

        /**
         * Rescales and returns the unnormialized value
         *
         * @return the unnormalized value
         */
        operator Tp() const noexcept;

        /**
         * Returns a reference to the underlying value
         *
         * @return the reference to the underlying value
         */
        Tp& get(void) noexcept
            { return range; }

        /**
         * Returns a constant reference to the underlying value
         *
         * @return the constant reference to the underlying value
         */
        Tp const& get(void) const noexcept
            { return range; }

        /**
         * Destroy the Adapter object
         */
        ~Adapter(void) noexcept = default;
    private:
        Tp                              range;
    };

}

#include <MPGL/Utility/Adapter.tpp>
