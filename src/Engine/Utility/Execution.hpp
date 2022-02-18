#pragma once

#include <concepts>

namespace mpgl {

    struct Execution {};

    struct Parallel : Execution {};

    struct Sequenced : Execution {};

    namespace execution {

        inline constexpr Parallel parallel{};
        inline constexpr Sequenced sequenced{};

        template <class Tp>
        concept ExecutionPolicy = std::derived_from<Tp,
            Execution>;

        template <ExecutionPolicy Tp>
        constexpr bool isParallelPolicy = std::derived_from<Tp,
            Parallel>;

        template <ExecutionPolicy Tp>
        constexpr bool isSequencedPolicy = std::derived_from<Tp,
            Sequenced>;

    }

}