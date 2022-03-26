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

#include "Canva.hpp"

#include <istream>
#include <ostream>

namespace mpgl {

    #pragma pack(push, 1)

    /**
     * Represents a RGBA pixel
     */
    struct Pixel {
        typedef uint8                           Subpixel;

        /**
         * Constructs a new pixel object from the given
         * subpixels
         *
         * @param red the red subpixel
         * @param green the green subpixel
         * @param blue the blue subpixel
         * @param alpha the alpha value
         */
        constexpr Pixel(
            Subpixel red,
            Subpixel green,
            Subpixel blue,
            Subpixel alpha) noexcept
                : red{red}, green{green},
                blue{blue}, alpha{alpha} {}

        /**
         * Constructs a new pixel object
         */
        constexpr Pixel(void) noexcept
            : red{0x00}, green{0x00}, blue{0x00}, alpha{0xFF} {}

        Subpixel                                red;
        Subpixel                                green;
        Subpixel                                blue;
        Subpixel                                alpha;

        /**
         * Returns a reference to element with the given index
         *
         * @param index the element's index
         * @return the reference to element
         */
        [[nodiscard]] constexpr Subpixel&
            operator[] (std::size_t index) noexcept;

        /**
         * Returns a constant reference to element
         * with the given index
         *
         * @param index the element's index
         * @return the constant reference to element
         */
        [[nodiscard]] constexpr Subpixel const& operator[] (
            std::size_t index) const noexcept;

        /**
         * Pixel manipulators, used to load the pixel from
         * the binary streams
         */
        struct Manip {
            /**
             * Loads the RGB data from a binary istream
             *
             * @param is the reference to the istream
             * @param pixel the reference to the pixel
             * @return the reference to the istream
             */
            static std::istream& RGB(
                std::istream& is,
                Pixel& pixel) noexcept;

            /**
             * Saves the RGB data from to binary ostream
             *
             * @param os the reference to the ostream
             * @param pixel the constant reference to the pixel
             * @return the reference to the ostream
             */
            static std::ostream& RGB(
                std::ostream& os,
                Pixel const& pixel) noexcept;

            /**
             * Loads the RGB data from the iterator
             *
             * @tparam Iter the iterator type
             * @param iter the reference to the iterator
             * @param pixel the reference to the pixel
             */
            template <std::input_iterator Iter>
            static void RGB(Iter& iter, Pixel& pixel) noexcept;
        };
    };

    #pragma pack(pop)

    template class Canva<Pixel>;

    typedef Canva<Pixel>                        Image;

    [[nodiscard]] constexpr Pixel::Subpixel&
        Pixel::operator[] (std::size_t index) noexcept
    {
        switch (index) {
            case 0:
                return red;
            case 1:
                return green;
            case 2:
                return blue;
            case 3:
                return alpha;
        }
        return red;
    }

    [[nodiscard]] constexpr const Pixel::Subpixel&
        Pixel::operator[] (std::size_t index) const noexcept
    {
        switch (index) {
            case 0:
                return red;
            case 1:
                return green;
            case 2:
                return blue;
            case 3:
                return alpha;
        }
        return red;
    }

    template <std::input_iterator Iter>
    void Pixel::Manip::RGB(Iter& iter, Pixel& pixel) noexcept {
        pixel.blue = *iter++;
        pixel.green = *iter++;
        pixel.red = *iter++;
    }

}
