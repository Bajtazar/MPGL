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

#include <MPGL/Mathematics/FastFunctions.hpp>
#include <MPGL/Utility/BitReversion.hpp>
#include <MPGL/Traits/Concepts.hpp>

#include <math.h>
#include <numeric>
#include <complex>
#include <climits>
#include <algorithm>

namespace mpgl {

    /**
     * Calculates the Discrete Fourier Transformation using
     * the fast algorithms (Cooley-Tukey and Bluestein)
     */
    class FFT {
    public:
        typedef std::complex<float64>               Complex;
        typedef std::size_t                         size_type;

        /**
         * Construcs a new FFT object
         */
        explicit FFT(void) noexcept = default;

        /**
         * Performs the Discrete Fourier Transformation on
         * the given range
         *
         * @tparam Range the range type
         * @param range the universal reference to the range object
         */
        template <std::ranges::random_access_range Range>
            requires (std::ranges::sized_range<Range>
                && InstanceOf<std::ranges::range_value_t<Range>,
                    std::complex>)
        void operator() (Range&& range) const noexcept;
    private:
        typedef std::vector<Complex>                ComplexVector;

        /**
         * Calculates the size of the Bluestein's convolution
         *
         * @param number the size of the range
         * @return the size of the Bluestein's convolution
         */
        static size_type convolutionSize(size_type number) noexcept;

        /**
         * Performs the Fast Fourier Transformation on the
         * given random access range
         *
         * @tparam Range the range type
         * @param range the universal reference to the range object
         */
        template <std::ranges::random_access_range Range>
        static void randomRangeFFT(Range&& range) noexcept;

        /**
         * Generates the lookup table for the FFT algorithm
         *
         * @param size the size of the lookup table
         * @return the lookup table
         */
        static ComplexVector generateLookupTable(
            size_type size) noexcept;

        /**
         * Generates the left side sequence of the Bluestein's
         * circular convolution
         *
         * @tparam Range the range type
         * @param range the universal reference to the range object
         * @param lookupTable the constant reference to the
         * lookup table
         * @param size the size of the sequence
         * @return the convolution's left side sequence
         */
        template <std::ranges::random_access_range Range>
        static ComplexVector generateLeftSequence(
            Range&& range,
            ComplexVector const& lookupTable,
            size_type size) noexcept;

        /**
         * Generates the right side sequence of the Bluestein's
         * circular convolution
         *
         * @param lookupTable the constant reference to the
         * lookup table
         * @param size the size of the sequence
         * @return the convolution's right side sequence
         */
        static ComplexVector generateRightSequence(
            ComplexVector const& lookupTable,
            size_type size) noexcept;

        /**
         * Perfroms the circular convolution on the given sequences
         *
         * @param leftSequence the left sequence
         * @param rightSequence the right seqeunce
         * @return the convolution result
         */
        static ComplexVector convolve(
            ComplexVector leftSequence,
            ComplexVector rightSequence) noexcept;

        /**
         * Performs the Cooley-Tukey FFT algorithm on the given
         * range. The size of the range must be the power of two
         *
         * @tparam Range the type of range
         * @param range the universal reference to the range object
         * @param sign the sign of the coefficients. If the sign
         * is negative, then performs the inverse discrete
         * fourier transformation
         */
        template <std::ranges::random_access_range Range>
        static void cooleyTukey(
            Range&& range,
            float64 sign = -1.) noexcept;

        /**
         * Performs the Bluestein FFT algorithm on the given
         * range. The size should not be the power of two
         *
         * @tparam Range the type of range
         * @param range the universal reference to the range object
         */
        template <std::ranges::random_access_range Range>
        static void bluestein(Range&& range) noexcept;

        /**
         * Stores the lookup table for the ranges with the
         * compile time known size
         *
         * @tparam Id the id of the base part
         * @tparam Inverse indicates whether the invertse
         * transformation coefficents should be calculated
         */
        template <size_type Id, bool Inverse>
        struct CooleyTukeyBasePart {
            typedef std::array<Complex, 1 << Id>    Table;

            static Table const                      lookupTable;

            /**
             * Generates the lookup table for the given base part
             *
             * @return the lookup table
             */
            static Table generateLookupTable(void) noexcept;
        };

        /**
         * Constructs a tuple containing all the Cooley-Tukey
         * Base Parts for the given size
         *
         * @tparam Size the transformation size
         * @tparam Inverse indicates whether the invertse
         * transformation coefficents should be calculated
         * @return the range with the parts
         */
        template <size_type Size, bool Inverse>
        static constexpr decltype(auto) constructor(void) noexcept {
            if constexpr (Size == 0)
                return std::tuple<CooleyTukeyBasePart<0, Inverse>>{};
            else
                return std::tuple_cat(
                    std::tuple<CooleyTukeyBasePart<Size, Inverse>>{},
                        constructor<Size - 1, Inverse>());
        }

        /**
         * Declaration of the helper class
         *
         * @tparam Up the type placeholder
         */
        template <typename Up>
        struct CooleyTukeyTupleHelper;

        /**
         * Builds the Cooley-Tokey helper class from the given
         * base parts tuple
         *
         * @tparam Args the base parts types
         */
        template <typename... Args>
        struct CooleyTukeyTupleHelper<std::tuple<Args...>>
            : public Args... {};

        /**
         * Base for the Cooley-Tukey algorithm performed on the
         * fixed size ranges
         *
         * @tparam Size the transformation size
         * @tparam Inverse indicates whether the invertse
         * transformation coefficents should be calculated
         */
        template <size_type Size, bool Inverse = false>
        class CooleyTukeyBase : private CooleyTukeyTupleHelper<
            decltype(constructor<::mpgl::log2N<size_type, Size>(),
                Inverse>())>
        {
        public:
            /**
             * Calculates the Discrete Fourier Transformation of
             * the given range using the Cooley-Tukey's algorithm
             * when the range's size is known at compilation
             * time
             *
             * @tparam Range the type of range
             * @param range the universal reference to the range
             * object
             */
            template <std::ranges::random_access_range Range>
            void operator() (Range&& range) const noexcept;
        private:
            /**
             * Performs one stage of the main loop
             *
             * @tparam Index the index of the stage
             * @tparam Range the type of range
             * @param range the universal reference to the range
             */
            template <std::size_t Index,
                std::ranges::random_access_range Range>
            void innerLoop(Range&& range) const noexcept;

            /**
             * Casts the underlying range type to the valid one
             *
             * @tparam Tp the range's value type
             * @param value the range's value
             * @param lookup the constant reference to the lookup
             * table
             * @return the casted value
             */
            template <typename Tp>
            inline Tp cast(
                Tp const& value,
                Complex const& lookup) const noexcept;

            /**
             * Returns the binary logarithmed size of the
             * range
             *
             * @return the binary logarithmed size of the
             * range
             */
            static constexpr size_type log2N()
                { return ::mpgl::log2N<size_type, Size>(); }
        };

        /**
         * Base for the Bluestein's algorithm performed on the
         * fixed size ranges
         *
         * @tparam Size the transformation size
         */
        template <size_type Size>
        class BluesteinBase {
        public:
            /**
             * Calculates the Discrete Fourier Transformation of
             * the given range using the Bluestein's algorithm
             * when the range's size is known at compilation
             * time
             *
             * @tparam Range the type of range
             * @param range the universal reference to the range
             * object
             */
            template <std::ranges::random_access_range Range>
            void operator() (Range&& range) const noexcept;
        private:
            /**
             * Returns the size of the Bluestein's convolution
             *
             * @return the size of the Bluestein's convolution
             */
            static constexpr size_type convoluteLen(void) noexcept;

            typedef std::array<Complex,
                convoluteLen()>                     ConvolutionArray;
            typedef std::array<Complex, Size>       Table;

            /**
             * Generates the right side sequence of the Bluestein's
             * circular convolution
             *
             * @tparam Range the range type
             * @param range the universal reference to the range
             * object
             * @return the convolution's left side sequence
             */
            template <std::ranges::random_access_range Range>
            ConvolutionArray generateLeftConvolution(
                Range&& range) const noexcept;

            /**
             * Generates the right side sequence of the Bluestein's
             * circular convolution
             *
             * @return the convolution's right side sequence
             */
            static ConvolutionArray generateRightConvolution(
                void) noexcept;

            /**
             * Generates the lookup table for the FFT algorithm
             *
             * @return the lookup table
             */
            static Table generateLookupTable(void) noexcept;

            static Table const                          lookupTable;
            static ConvolutionArray const               rightConvolution;
        };

        template <size_type Size>
        class ConstexprSizeFFT : public std::conditional_t<
            IsPowerOf2<size_type, Size>,
            CooleyTukeyBase<Size>, BluesteinBase<Size>> {};
    };

    constexpr inline FFT                            fft{};

}

#include <MPGL/Mathematics/Transforms/FFT.tpp>
