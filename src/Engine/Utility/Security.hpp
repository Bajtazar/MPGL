#pragma once

#include <concepts>

namespace mpgl {

    struct Security {};

    struct Unsecured : Security {};

    struct Secured : Security {};

    namespace security {

        inline constexpr Unsecured unsecured{};
        inline constexpr Secured secured;

        template <class T>
        concept SecurityPolicy = std::derived_from<T, Security>;

        template <SecurityPolicy T>
        constexpr bool isSecurePolicy = std::derived_from<T, Secured>;

        template <SecurityPolicy T>
        constexpr bool isUnsecuredPolicy = std::derived_from<T, Unsecured>;

    }

}
