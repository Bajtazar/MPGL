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

#include "Texture.hpp"

#include <algorithm>
#include <vector>
#include <map>

namespace mpgl {

    /**
     * Storage for textures. Wraps the std::map to utilize its
     * log(N) access time. Allows to set the default texture
     * which is being returned when there is no matches with
     * given key
     */
    class TexturePack {
    public:
        typedef std::string                         Key;
        typedef Texture                             Value;
        typedef std::map<Key, Value>                TextureMap;
        typedef typename TextureMap::value_type     value_type;
        typedef typename TextureMap::size_type      size_type;

        using iterator
            = typename TextureMap::iterator;
        using const_iterator
            = typename TextureMap::const_iterator;
        using reverse_iterator
            = typename TextureMap::reverse_iterator;
        using const_reverse_iterator
            = typename TextureMap::const_reverse_iterator;

        /**
         * Construct a new Texture Pack object from a given
         * range and allows to set the default texture
         *
         * @tparam Range the type of copyied range
         * @param range the copyied range with keys and textures
         * @param defaultTexture the default texture
         */
        template <std::ranges::input_range Range>
            requires SameRangeType<Range, value_type>
        explicit TexturePack(Range&& range,
            Value const& defaultTexture = Value::defaultTexture());

        /**
         * Construct a new Texture Pack object from a given
         * iterators range and allows to set the default texture
         *
         * @tparam Iter the range iterator type
         * @tparam Sent the range sentinel type
         * @param iter the iterator of the copyied range with
         * keys and textures
         * @param sent the sentinel of the copyied range with
         * keys and textures
         * @param defaultTexture the default texture
         */
        template <std::input_iterator Iter,
            std::sentinel_for<Iter> Sent>
                requires SameIterType<Iter, value_type>
        explicit TexturePack(Iter iter, Sent const& sent,
            Value const& defaultTexture = Value::defaultTexture());

        /**
         * Construct a new empty Texture Pack object with
         * given default texture
         *
         * @param defaultTexture the default texture
         */
        explicit TexturePack(Value const& defaultTexture
            = Value::defaultTexture());

        TexturePack(TexturePack const&) = default;
        TexturePack(TexturePack&&) = default;

        TexturePack& operator=(TexturePack const&) = delete;
        TexturePack& operator=(TexturePack&&) = delete;

        /**
         * Returns the iterator to the begining of the texture map
         *
         * @return iterator to the begining of the texture map
         */
        [[nodiscard]] iterator begin(void) noexcept
            { return textureMap.begin(); }

        /**
         * Returns the iterator to the end of the texture map
         *
         * @return iterator to the end of the texture map
         */
        [[nodiscard]] iterator end(void) noexcept
            { return textureMap.end(); }

        /**
         * Returns the constant iterator to the begining of
         * the texture map
         *
         * @return constant iterator to the begining of the
         * texture map
         */
        [[nodiscard]] const_iterator begin(void) const noexcept
            { return textureMap.begin(); }

        /**
         * Returns the constant iterator to the end of
         * the texture map
         *
         * @return constant iterator to the end of the
         * texture map
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return textureMap.end(); }

        /**
         * Returns the constant iterator to the begining of
         * the texture map
         *
         * @return constant iterator to the begining of the
         * texture map
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return textureMap.cbegin(); }

        /**
         * Returns the constant iterator to the end of
         * the texture map
         *
         * @return constant iterator to the end of the
         * texture map
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return textureMap.cend(); }

        /**
         * Returns the reverse iterator to the end of
         * the texture map
         *
         * @return reverse iterator to the end of the
         * texture map
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept
            { return textureMap.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the texture map
         *
         * @return reverse iterator to the begining of the
         * texture map
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept
            { return textureMap.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the texture map
         *
         * @return constant reverse iterator to the end of the
         * texture map
         */
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept
                { return textureMap.rbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the texture map
         *
         * @return constant reverse iterator to the begining of the
         * texture map
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept
                { return textureMap.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the texture map
         *
         * @return constant reverse iterator to the end of the
         * texture map
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept
                { return textureMap.crbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the texture map
         *
         * @return constant reverse iterator to the begining of the
         * texture map
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept
                { return textureMap.crend(); }

        /**
         * Returns the constant reference to the texture associated
         * to the given key. Returns default texture when the key
         * does not match any texture.
         *
         * @param key the texture name
         * @return constant reference to texture or constant
         * reference to default texture
         */
        [[nodiscard]] Value const&
            operator[] (Key const& key) const noexcept;

        /**
         * Returns the number of stored textures without the
         * default one
         *
         * @return the number of stored textures
         */
        [[nodiscard]] size_type size(void) const noexcept
            { return textureMap.size(); }

        /**
         * Returns whether textures are stored other than
         * the default one
         *
         * @return whether textures are stored
         */
        [[nodiscard]] bool empty(void) const noexcept
            { return textureMap.empty(); }

        /**
         * Returns the reference to the underlying texture map
         *
         * @return reference to the underlying texture map
         */
        [[nodiscard]] TextureMap& underlying(void) noexcept
            { return textureMap; }

        /**
         * Returns the constaint reference to the underlying
         * texture map
         *
         * @return constant reference to the underlying
         * texture map
         */
        [[nodiscard]] TextureMap const&
            underlying(void) const noexcept
                { return textureMap; }

        /**
         * Returns the constant reference to the default texture
         *
         * @return the constant reference to the default texture
         */
        [[nodiscard]] Value const&
            defaultTexture(void) const noexcept
                { return defaultValue; }

        /**
         * Destroy the Texture Pack object
         */
        ~TexturePack(void) noexcept = default;
    private:
        TextureMap                                  textureMap;
        Value const                                 defaultValue;
    };

    template <std::input_iterator Iter,
        std::sentinel_for<Iter> Sent>
            requires SameIterType<Iter, TexturePack::value_type>
    TexturePack::TexturePack(Iter iter, Sent const& sent,
        Value const& defaultTexture)
            : textureMap{iter, sent},
            defaultValue{defaultTexture} {}

    template <std::ranges::input_range Range>
        requires SameRangeType<Range, TexturePack::value_type>
    TexturePack::TexturePack(Range&& range,
        Value const& defaultTexture)
            : TexturePack{std::ranges::begin(range),
                std::ranges::end(range), defaultTexture} {}

}
