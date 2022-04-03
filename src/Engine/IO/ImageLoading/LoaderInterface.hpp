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

#include <fstream>

#include "../../Collections/Image.hpp"
#include "../Readers.hpp"

namespace mpgl {

    /**
     * Base class for the image loaders
     */
    class LoaderInterface {
    public:
        typedef std::size_t                         size_type;

        /**
         * Returns the constant reference to the loaded image
         *
         * @return the constant reference to the loaded image
         */
        [[nodiscard]] Image const& getImage(void) const noexcept
            { return pixels; }

        /**
         * Returns the width of the image
         *
         * @return the width of the image
         */
        [[nodiscard]] size_type getWidth(void) const noexcept
            { return pixels.getWidth(); }

        /**
         * Returns the height of the image
         *
         * @return the height of the image
         */
        [[nodiscard]] size_type getHeight(void) const noexcept
            { return pixels.getHeight(); }

        /**
         * Virtual destructor. Destroys the Loader Interface object
         */
        virtual ~LoaderInterface(void) noexcept = default;
    protected:
        Image                                       pixels;
        std::string                                 filePath;

        /**
         * Constructs a new Loader Interface object from the given
         * file path
         *
         * @param filePath the file path
         */
        explicit LoaderInterface(std::string const& filePath)
            : filePath{filePath} {}
    };

}
