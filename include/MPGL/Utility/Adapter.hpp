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
     * Wrapps the float32 mathematical vector. For the 2D
     * objects maps the vector from the current screen dimensions
     * to the normalized OpenGl's [-1, 1]^2 space. For other
     * dimensions does not perform any transformations or operations
     * allowing to specialize its instance when other space is more
     * handy to operate within. Even when the adapter is not
     * specialized it is recommended to use the get() method for the
     * performance and safety reasons
     *
     * @tparam Size the size of the vector
     */
    template <std::size_t Size>
        requires (Size > 1u)
    class Adapter {
    public:
        using value_type                  = Vector<float32, Size>;
        using reference                   = value_type&;
        using const_reference             = value_type const&;

        /**
         * Constructs a new Adapter object
         *
         * @param value the constant reference to the adapter's value
         */
        constexpr Adapter(const_reference value) noexcept;

        /**
         * Constructs a new Adapter object
         *
         * @param value the rvalue reference to the adapter's value
         */
        constexpr Adapter(value_type&& value) noexcept;

        constexpr Adapter(Adapter const&) noexcept = default;
        constexpr Adapter(Adapter&&) noexcept = default;

        constexpr Adapter& operator=(Adapter const&) noexcept = default;
        constexpr Adapter& operator=(Adapter&&) noexcept = default;

        /**
         * Assigns the value to the adapter object
         *
         * @param value the constant reference to the assigned
         * value object
         */
        constexpr Adapter& operator=(const_reference value) noexcept;

        /**
         * Assigns the value to the adapter object
         *
         * @param value the rvalue reference to the assigned
         * value object
         */
        constexpr Adapter& operator=(value_type&& value) noexcept;

        /**
         * Returns the copy of the wrapped object
         *
         * @return the copy of the wrapped object
         */
        [[nodiscard]] constexpr operator value_type() const noexcept
            { return value; }

        /**
         * Casts the wrapped object onto the given type
         *
         * @tparam Tp the casted vector's type
         * @return the casted vector
         */
        template <Arithmetic Tp>
            requires std::convertible_to<float32, Tp>
        [[nodiscard]] operator Vector2<Tp>() const noexcept
            { return value_type(*this); }

        /**
         * Returns a reference to the wrapped object
         *
         * @return the reference to the wrapped object
         */
        [[nodiscard]] constexpr reference get(void) noexcept
            { return value; }

        /**
         * Returns constant reference to the wrapped object
         *
         * @return the constant reference to the wrapped objecy
         */
        [[nodiscard]] constexpr const_reference get(
            void) const noexcept
                { return value; }

        /**
         * Destroys the Adapter object
         */
        constexpr ~Adapter(void) noexcept = default;
    private:
        value_type                          value;
    };

    /**
     * Holds a vector mapped from the window dimensions to the
     * OpenGl's [-1, 1]^2 space and performs rescaling operations
     * during access
     */
    template <>
    class Adapter<2> : private GraphicalObject {
    public:
        using value_type                  = Vector2f;
        using reference                   = value_type&;
        using const_reference             = value_type const&;

        /**
         * Constructs a new Adapter object. Normalizes
         * the given value
         *
         * @param value the constant reference to the adapter's value
         */
        Adapter(const_reference value) noexcept;

        /**
         * Constructs a new Adapter object. Normalizes
         * the given value
         *
         * @param value the rvalue reference to the adapter's value
         */
        Adapter(value_type&& value) noexcept;

        constexpr Adapter(Adapter const&) noexcept = default;
        constexpr Adapter(Adapter&&) noexcept = default;

        constexpr Adapter& operator=(Adapter const&) noexcept = default;
        constexpr Adapter& operator=(Adapter&&) noexcept = default;

        /**
         * Assigns the value to the adapter object. Normalizes
         * the given range
         *
         * @param value the constant reference to the assigned
         * value object
         */
        Adapter& operator=(const_reference value) noexcept;

        /**
         * Assigns the value to the adapter object. Normalizes
         * the given range
         *
         * @param value the rvalue reference to the assigned
         * value object
         */
        Adapter& operator=(value_type&& value) noexcept;

        /**
         * Rescales and returns the denormalized value
         *
         * @return the denormalized value
         */
        [[nodiscard]] operator value_type() const noexcept;

        /**
         * Rescales, converts and returns the denormalized value
         *
         * @return the denormalized value
         */
        template <Arithmetic Tp>
            requires std::convertible_to<float32, Tp>
        [[nodiscard]] operator Vector2<Tp>() const noexcept
            { return value_type(*this); }

        /**
         * Returns a reference to the wrapped object
         *
         * @return the reference to the wrapped object
         */
        [[nodiscard]] constexpr reference get(void) noexcept
            { return value; }

        /**
         * Returns constant reference to the wrapped object
         *
         * @return the constant reference to the wrapped objecy
         */
        [[nodiscard]] constexpr const_reference get(
            void) const noexcept
                { return value; }

        /**
         * Destroys the Adapter object
         */
        constexpr ~Adapter(void) noexcept = default;
    private:
        value_type                          value;

        /**
         * Casts the position vector from the pixel space into
         * the OpenGL's normalized space
         *
         * @param value a constant reference to the pixel space vector
         * @return the OpenGL's normalized space vector
         */
        static value_type cast(const_reference value) noexcept;
    };

    typedef Adapter<2>                      Adapter2D;
    typedef Adapter<3>                      Adapter3D;

}

#include <MPGL/Utility/Adapter.tpp>
