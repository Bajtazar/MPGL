#pragma once

#include <inttypes.h>
#include <array>

namespace ge {

    template <uint16_t Precision>
        requires (Precision != 0)
    class ChebyshevPol {
    protected:
        typedef std::array<double, Precision>   CoefficientArray;

        constexpr ChebyshevPol(CoefficientArray&& coefficients) noexcept
            : coefficients{coefficients} {}

        constexpr double calculate(double point) noexcept;
    private:
        CoefficientArray                        coefficients;

        constexpr double higherPrecision(double point) const noexcept;
    };

    template <uint16_t Precision>
        requires (Precision != 0)
    constexpr double ChebyshevPol<Precision>::higherPrecision(double point) const noexcept {
        double prelast = 1., last = point, sum = 0.;
        for (uint16_t i = 2; i != Precision; ++i) {
            double current = 2 * point * last - prelast;
            prelast = last;
            last = current;
            sum += coefficients[i] * current;
        }
        return sum;
    }

    template <uint16_t Precision>
        requires (Precision != 0)
    constexpr double ChebyshevPol<Precision>::calculate(double point) noexcept {
        double sum;
        if constexpr (Precision == 1)
            sum = coefficients[0];
        else if constexpr (Precision == 2)
            sum = point * coefficients[1] + coefficients[0];
        if constexpr(Precision > 2)
            sum += higherPrecision(point);
        return sum;
    }

}
