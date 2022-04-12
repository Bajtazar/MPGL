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

#include <MPGL/Utility/BitReversion.hpp>
#include <MPGL/Utility/FixedRange.hpp>
#include <MPGL/Mathematics/Matrix.hpp>
#include <MPGL/Traits/Concepts.hpp>
#include <MPGL/Mathematics/FFT.hpp>

#include <climits>
#include <array>

namespace mpgl {

    /**
     * Calculates the Inverse Discrete Cosine Transformation using
     * the FFT based algorithm
     */
    class IFCT {
    public:
        typedef std::complex<float64>               Complex;
        typedef std::size_t                         size_type;

        /**
         * Constructs a new IFCT object
         */
        constexpr explicit IFCT(void) noexcept = default;

        /**
         * Performs the Inverse Discrete Cosine Transformation on
         * the given range
         *
         * @tparam Range the range type
         * @param range the universal reference to the range object
         */
        template <std::ranges::random_access_range Range>
            requires (std::ranges::sized_range<Range>
                && Arithmetic<std::ranges::range_value_t<Range>>)
        void operator() (Range&& range) const;

        /**
         * Performs the two dimensional Inverse Discrete Cosine
         * Transformation on the given matrix
         *
         * @tparam Tp the matrix's element type
         * @tparam Rows the matrix's rows size
         * @param matrix the reference to the matrix object
         */
        template <Arithmetic Tp, size_type Rows>
        void operator() (
            Matrix<Tp, Rows, Rows>& matrix) const;
    private:
        typedef std::vector<Complex>                ComplexVector;

        /**
         * The default preprocessing kernel
         */
        struct DefaultKernel {
            /**
             * Constructs a new default kernel object
             */
            constexpr explicit DefaultKernel(
                void) noexcept = default;

            /**
             * Perfroms the preprocessing on the given range
             *
             * @tparam Range the range type
             * @param range the universal reference to the range
             * object
             * @return the preprocessed range
             */
            template <std::ranges::random_access_range Range>
            ComplexVector operator() (Range&& range) const;
        };

        /**
         * The preprocessing kernel for fixed ranges
         *
         * @tparam Size the range size
         */
        template <size_type Size>
        class FixedSizeKernel {
        public:
            /**
             * Constructs a new Fixed Size Kernel object
             */
            constexpr explicit FixedSizeKernel(
                void) noexcept = default;

             /**
             * Perfroms the preprocessing on the given range
             *
             * @tparam Range the range type
             * @param range the universal reference to the range
             * object
             * @return the preprocessed range
             */
            template <std::ranges::random_access_range Range>
            ComplexVector operator() (Range&& range) const;
        private:
            typedef std::array<Complex, Size>       ComplexArray;

            /**
             * Generates the lookup table
             *
             * @return the lookup table
             */
            static ComplexArray generateLookupTable(void);

            static ComplexArray const               lookupTable;
        };

        /**
         * The adpater for the fixed size kernel
         */
        struct FixedKernel {
            /**
             * Constructs a new Fixed Kernel object
             */
            constexpr explicit FixedKernel(void) noexcept = default;

             /**
             * Perfroms the preprocessing on the given range
             *
             * @tparam Range the range type
             * @param range the universal reference to the range
             * object
             * @return the preprocessed range
             */
            template <std::ranges::random_access_range Range>
            ComplexVector operator() (Range&& range) const;
        };

        /**
         * Performs the postprocessing on the given range
         *
         * @tparam Range the range type
         * @param range the universal reference to the range
         * object
         * @param vector the constant reference to the processed
         * vector object
         */
        template <std::ranges::random_access_range Range>
        void postprocessing(
            Range&& range,
            ComplexVector const& vector) const;

        /**
         * Perfroms the preprocessing on the given range. Chooses
         * the correct preprocessing kernel
         *
         * @tparam Range the range type
         * @param range the universal reference to the range
         * object
         * @return the preprocessed range
         */
        template <std::ranges::random_access_range Range>
        ComplexVector preprocessing(Range&& range) const;

        [[no_unique_address]] DefaultKernel         defaultKernel;
        [[no_unique_address]] FixedKernel           fixedKernel;
    };

    inline constexpr IFCT                           ifct;

}

#include <MPGL/Mathematics/IFCT.tpp>
