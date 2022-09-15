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

#include <MPGL/Core/Transformations/Transformable.hpp>
#include <MPGL/Mathematics/Systems.hpp>
#include <MPGL/Core/DimensionTraits.hpp>

#include <algorithm>
#include <vector>

namespace mpgl {

    namespace details {

        /**
         * Checks whether the transformation event extension
         * can be applied to the given type drawable collection
         *
         * @tparam Base the checked type
         */
        template <typename Base>
        concept TransDrawable = InstanceOf<Base, Drawable> &&
            InstanceOf<Base, Transformable>;

    }

    /**
     * The container that simplifies the common operations
     * on the drawable type. Allows to use the desired
     * collection underneath
     *
     * @tparam Base the drawable type
     * @tparam Range the collection type
     */
    template <InstanceOf<Drawable> Base,
        std::ranges::input_range Range = std::vector<Base>>
    struct DrawableCollection :
        public Drawable<DimensionOfT<Drawable, Base>>,
        public Range
    {
        using Range::Range;

        DrawableCollection(
            DrawableCollection const& base) noexcept = default;
        DrawableCollection(
            DrawableCollection&& base) noexcept = default;

        DrawableCollection& operator=(
            DrawableCollection const& base) noexcept = default;
        DrawableCollection& operator=(
            DrawableCollection&& base) noexcept = default;

        using iterator
            = typename Range::iterator;
        using const_iterator
            = typename Range::const_iterator;
        using reverse_iterator
            = typename Range::reverse_iterator;
        using const_reverse_iterator
            = typename Range::const_reverse_iterator;

        using value_type = typename Range::value_type;
        using size_type = typename Range::size_type;

        /**
         * Draws all of the contained drawables on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Draws all of the contained drawables on the screen
         * with the indices between the given range
         *
         * @param begin the first index
         * @param end the last index
         */
        void draw(
            std::size_t begin,
            std::size_t end) const noexcept
                requires std::ranges::random_access_range<Range>;

        /**
         * Destroys the Drawable Collection object
         */
        ~DrawableCollection(void) noexcept = default;
    };

    /**
     * The container that simplifies the common operations
     * on the drawable type. Allows to use the desired
     * collection underneath. Extends the default usage by
     * providing an aditional Transformable support
     *
     * @tparam Base the drawable type
     * @tparam Range the collection type
     */
    template <details::TransDrawable Base,
        std::ranges::input_range Range>
    struct DrawableCollection<Base, Range> :
        public Drawable<DimensionOfT<Drawable, Base>>,
        public Range,
        public Transformable<DimensionOfT<Drawable, Base>>
    {
        using Range::Range;

        using Dim = DimensionOfT<Drawable, Base>;

        DrawableCollection(
            DrawableCollection const& base) noexcept = default;
        DrawableCollection(
            DrawableCollection&& base) noexcept = default;

        DrawableCollection& operator=(
            DrawableCollection const& base) noexcept = default;
        DrawableCollection& operator=(
            DrawableCollection&& base) noexcept = default;

        using iterator
            = typename Range::iterator;
        using const_iterator
            = typename Range::const_iterator;
        using reverse_iterator
            = typename Range::reverse_iterator;
        using const_reverse_iterator
            = typename Range::const_reverse_iterator;

        using value_type = typename Range::value_type;
        using size_type = typename Range::size_type;

        /**
         * Draws all of the contained drawables on the screen
         */
        void draw(void) const noexcept final;

        /**
         * Draws all of the contained drawables on the screen
         * with the indices between the given range
         *
         * @param begin the first index
         * @param end the last index
         */
        void draw(
            std::size_t begin,
            std::size_t end) const noexcept
                requires std::ranges::random_access_range<Range>;

        /**
         * Performes transforation on the contained drawables
         *
         * @param transformator the constant reference to the
         * transforming object
         */
        void transform(
            Transformation<Dim> const& transformator) noexcept final;

        /**
         * Destroys the Drawable Collection object
         */
        ~DrawableCollection(void) noexcept = default;
    };

}

#include <MPGL/Core/DrawableCollection.tpp>
