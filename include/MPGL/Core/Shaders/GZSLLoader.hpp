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

#include <MPGL/Traits/Types.hpp>

#include <string>
#include <map>

namespace mpgl {

    /**
     * The GZSL file format parser
     */
    struct GZSLLoader {
        /**
         * Parses the GZSL file format and returns a map containing
         * the shader names and their codes
         *
         * @param path a path to the GZSL file
         * @return the map containing
         * the shader names and their codes
         */
        [[nodiscard]] std::map<std::string, std::string>
            operator() (std::string const& path) const;
    };

    constexpr inline GZSLLoader                         gzslLoader;

}
