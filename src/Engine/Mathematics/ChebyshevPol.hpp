#pragma once

#include <algorithm>
#include <array>

namespace ge {

    template <uint8_t Precision, uint8_t Size>
        requires (Size >= Precision)
    constexpr std::array<double, Precision> cutArray(
            std::array<double, Size> const& array) noexcept;

    template <uint8_t Precision>
        requires (Precision != 0)
    class ChebyshevPol {
    protected:
        typedef std::array<double, Precision>   CoefficientArray;

        constexpr ChebyshevPol(CoefficientArray&& coefficients) noexcept
            : coefficients{coefficients} {}

        constexpr double calculate(double point) const noexcept;
    private:
        CoefficientArray                        coefficients;

        constexpr double higherPrecision(double point) const noexcept;
    };

    template <uint8_t Precision>
        requires (Precision != 0)
    constexpr double ChebyshevPol<Precision>::higherPrecision(double point) const noexcept {
        double prelast = 1., last = point, sum = 0.;
        for (uint8_t i = 2; i != Precision; ++i) {
            double current = 2. * point * last - prelast;
            prelast = last;
            last = current;
            sum += coefficients[i] * current;
        }
        return sum;
    }

    template <uint8_t Precision>
        requires (Precision != 0)
    constexpr double ChebyshevPol<Precision>::calculate(double point) const noexcept {
        double sum = coefficients[0];
        if constexpr (Precision >= 2)
            sum += point * coefficients[1];
        if constexpr (Precision > 2)
            sum += higherPrecision(point);
        return sum;
    }

    template <uint8_t Precision, uint8_t Size>
        requires (Size >= Precision)
    constexpr std::array<double, Precision>
        cutArray(std::array<double, Size> const& array) noexcept
    {
        std::array<double, Precision> result;
        std::copy(array.begin(), array.begin() + Precision, result.begin());
        return result;
    }
}
