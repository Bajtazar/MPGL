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

#include <concepts>

namespace mpgl {

    /**
     * Base class for execution tags
     */
    struct Execution {};

    /**
     * The parallel execution tag
     */
    struct Parallel : Execution {};

    /**
     * The sequenced execution tag
     */
    struct Sequenced : Execution {};

    namespace execution {

        inline constexpr Parallel                   parallel{};
        inline constexpr Sequenced                  sequenced{};

        /**
         * Checks whether the given type is the execution policy tag
         *
         * @tparam Tp the checked type
         */
        template <class Tp>
        concept ExecutionPolicy = std::derived_from<Tp, Execution>;

        /**
         * Checks whether the given type is the parallel
         * execution policy tag
         *
         * @tparam Tp the checked type
         */
        template <ExecutionPolicy Tp>
        constexpr bool isParallelPolicy = std::derived_from<Tp,
            Parallel>;

        /**
         * Checks whether the given type is the sequenced
         * execution policy tag
         *
         * @tparam Tp the checked type
         */
        template <ExecutionPolicy Tp>
        constexpr bool isSequencedPolicy = std::derived_from<Tp,
            Sequenced>;

    }

}
