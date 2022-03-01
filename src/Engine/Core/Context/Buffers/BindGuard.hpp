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

#include "../../../Traits/Concepts.hpp"

namespace mpgl {

    /**
     * Ensures that the given bindable object will be unbinded
     * after the duration of bind guard block. When the object
     * is created then binds the given object and unbinds it
     * upon destruction
     *
     * @tparam Tp the bindable type
     */
    template <Bindable Tp>
    class BindGuard;

    template <Bindable Tp>
    BindGuard(Tp const&) -> BindGuard<Tp>;

    /**
     * Ensures that the given bindable object will be unbinded
     * after the duration of bind guard block. When the object
     * is created then binds the given object and unbinds it
     * upon destruction. Allows to get the result of the binding
     * method
     *
     * @tparam Tp the bindable type which returns move constructible
     * type upon binding
     */
    template <ReturnBindable Tp>
    class BindGuard<Tp> {
    public:
        using BindResult = decltype(std::declval<Tp const>().bind());

        /**
         * Construct a new Bind Guard object. Binds the given
         * object and saves its result
         *
         * @param bindable the constant reference to bindable
         * object
         */
        explicit BindGuard(Tp const& bindable) noexcept
            : bindable{bindable}, result{bindable.bind()} {}

        BindGuard(BindGuard const&) = delete;
        BindGuard(BindGuard&&) = delete;

        BindGuard& operator=(BindGuard const&) = delete;
        BindGuard& operator=(BindGuard&&) = delete;

        /**
         * Returns the reference to the result of binding
         *
         * @return the reference to the result of binding
         */
        [[nodiscard]] BindResult& getResult(void) & noexcept
            { return result; }

        /**
         * Returns the constant reference to the result of binding
         *
         * @return the constaint reference to the result of binding
         */
        [[nodiscard]] BindResult const& getResult(void) const& noexcept
            { return result;}

        /**
         * Returns the rvalue to the result of binding
         *
         * @return the rvalue to the result of binding
         */
        [[nodiscard]] BindResult&& getResult(void) && noexcept
            { return std::move(result); }

        /**
         * Destroy the Bind Guard object. Unbinds the bindable
         * object
         */
        ~BindGuard(void) noexcept
            { bindable.unbind(); }
    private:
        Tp const&                                           bindable;
        BindResult                                          result;
    };

    /**
     * Ensures that the given bindable object will be unbinded
     * after the duration of bind guard block. When the object
     * is created then binds the given object and unbinds it
     * upon destruction
     *
     * @tparam Tp the bindable type which returns nothing
     * (bind method returns void type)
     */
    template <NonReturnBindable Tp>
    class BindGuard<Tp> {
    public:
        /**
         * Construct a new Bind Guard object. Binds the given
         * object
         *
         * @param bindable
         */
        explicit BindGuard(Tp const& bindable) noexcept
            : bindable{bindable} { bindable.bind(); }

        BindGuard(BindGuard const&) = delete;
        BindGuard(BindGuard&&) = delete;

        BindGuard& operator=(BindGuard const&) = delete;
        BindGuard& operator=(BindGuard&&) = delete;

        /**
         * Destroy the Bind Guard object. Unbinds the bindable
         * object
         */
        ~BindGuard(void) noexcept
            { bindable.unbind(); }
    private:
        Tp const&                                            bindable;
    };

}
