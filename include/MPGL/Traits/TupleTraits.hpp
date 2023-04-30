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

#include <MPGL/Traits/Concepts.hpp>

#include <tuple>

namespace mpgl {

    /**
     * Helper metastruct. Helps in creation of mutlidimensional
     * tensor-like tuples
     */
    class TupleHelperFn {
    private:
        /**
         * Meta method. Allows to get the tuple type containing
         * the given number of types
         *
         * @tparam Tp the tuple base type
         * @tparam I the index sequence
         * @return the tuple consisting of the given number of
         * base types
         */
        template <PureType Tp, std::size_t... I>
        static constexpr auto tupleConstructor(
            std::index_sequence<I...>) noexcept
                -> decltype(std::tuple{(I, std::declval<Tp>())...});
    public:
        /**
         * Meta method. Allows to get the tuple type containing
         * the given number of types
         *
         * @tparam Tp the tuple base type
         * @tparam Dim the tuple dimensions
         * @return the tuple consisting of the given number of
         * base types
         */
        template <PureType Tp, std::size_t Dim>
            requires (Dim != 0)
        static constexpr auto tensorConstructor(void) noexcept
            -> decltype(tupleConstructor<Tp>(
                std::make_index_sequence<Dim>{}));

        /**
         * Meta method. Allows to get the tensor-like tuple
         * constructed from the given number of types in
         * earch dimension
         *
         * @tparam Tp the tuple base type
         * @tparam Dim the first-dimension tuple dimensions
         * @tparam the rest dimensions
         * @return the tensor-like tuple
         */
        template <PureType Tp, std::size_t Dim,
            std::size_t... Dimensions>
                requires (Dim != 0 && sizeof...(Dimensions) > 0)
        static constexpr auto tensorConstructor(void) noexcept
            -> decltype(tensorConstructor<decltype(
            tensorConstructor<Tp, Dimensions...>()), Dim>());
    };

    /**
     * Creates a tuple with reversed arguments
     *
     * @tparam Args the arguments types
     * @param args the arguments
     * @return the tuple with reversed arguments
     */
    template <typename... Args>
    constexpr auto tupleReverser(Args&&... args) noexcept;

    /**
     * The convenient for shortcut the uniform tuple [one
     * dimensional tuple]
     *
     * @tparam Tp the tuple's base type
     * @tparam Size the tuple size
     */
    template <PureType Tp, std::size_t Size>
    using UniformTuple = decltype(TupleHelperFn::tensorConstructor<Tp,
        Size>());

    /**
     * The convenient shortcut for the tensor tuple
     * [mutlidimensional tuple]
     *
     * @tparam Tp the tuple's base type
     * @tparam Dimensions the tuple dimensions
     */
    template <PureType Tp, std::size_t... Dimensions>
    using TensorTuple = decltype(TupleHelperFn::tensorConstructor<Tp,
        Dimensions...>());

    /**
     * The convenient shortcut for the reversed tuple
     *
     * @tparam Args the tuple arguments [to be reversed]
     */
    template <typename... Args>
    using ReversedTuple = decltype(tupleReverser<Args...>(
        std::declval<Args>()...));

}

#include <MPGL/Traits/TupleTraits.tpp>
