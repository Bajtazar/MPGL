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

#include <MPGL/Core/Text/Subfont.hpp>

#include <memory>

namespace mpgl {

    /**
     * Contains all of the subfonts consisting for the font.
     * Can be copyied without unefficient memory duplication
     */
    class Font {
    public:
        /**
         * All available types of subfonts
         */
        enum class Type : uint8 {
            /// The regular subfont
            Regular                           = 0x01,
            /// The italic subfont
            Italic                            = 0x02,
            /// The bold subfont
            Bold                              = 0x04,
            /// The italic subfont
            BoldItalic                        = 0x08
        };

        /**
         * Construct a new Font object. Loads all subfonts
         * founded in the font directory
         *
         * @param fontName the font's name
         * @param fontDirectory  the font's directory
         */
        explicit Font(
            std::string const& fontName,
            std::string fontDirectory = {});

        Font(Font const& font) = default;
        Font(Font&& font) noexcept = default;

        Font& operator=(Font const& font) = default;
        Font& operator=(Font&& font) noexcept = default;

        /**
         * Returns the font's mask [bit mask storing
         * information about stored subfonts]
         *
         * @return the font's mask
         */
        uint8 getMask(void) const noexcept
            { return pointer->mask; }

        /**
         * Returns the reference to a subfont of the given type.
         * When the given subfont type is not available returns
         * the reference to the regular subfont
         *
         * @param type the type of the subfont
         * @return reference to the subfont
         */
        Subfont& operator() (Type const& type);

        /**
         * Returns the constant reference to a subfont of the
         * given type. When the given subfont type is not available
         * returns the constant reference to the regular subfont
         *
         * @param type the type of the subfont
         * @return reference to the subfont
         */
        Subfont const& operator() (Type const& type) const;

        /**
         *  Destroy the Font object
         */
        ~Font(void) noexcept = default;
    private:
        typedef std::map<Type, Subfont>         SubfontsMap;
        typedef std::vector<std::string>        Files;
        typedef std::map<Type const,
            std::string const>                  TypeMap;

        /**
         * Stores the basic font's data. Allows for easy
         * font copying
         */
        struct Container {
            SubfontsMap                         subfonts;
            std::string                         fontName;
            uint8                               mask;

            explicit Container(std::string const& fontName);
        };

        typedef std::shared_ptr<Container>      ContainerPtr;

        ContainerPtr                            pointer;

        /**
         * Finds the given subfont in the given files and loads
         * it when it is possible
         *
         * @param files the searched files
         * @param signatures the subfont signature
         * @param type the subfont type signature
         * @param flag the subfont type flag
         */
        void findSubfont(
            Files& files,
            Files& signatures,
            std::string const& type,
            Type const& flag);

        /**
         * Adds the given subfont to the font collection
         *
         * @param position the position of subfont name in files
         * @param files the searched files
         * @param signatures the subfont signature
         * @param flag the subfont type flag
         */
        void addSubfont(
            std::size_t position,
            Files& files,
            Files& signatures,
            Type const& flag);

        static const TypeMap                typeVector;
    };

    uint8 operator& (Font::Type const& left, uint8 right) noexcept;

    uint8 operator& (uint8 left, Font::Type const& right) noexcept;

}
