#pragma once

#include <concepts>

namespace ge {

    struct Execution {};

    struct Parallel : Execution {};

    struct Sequenced : Execution {};

    namespace execution {

        inline constexpr Parallel parallel{};
        inline constexpr Sequenced sequenced{};

        template <class T>
        concept ExecutionPolicy = std::derived_from<T, Execution>;

        template <ExecutionPolicy T>
        constexpr bool isParallelPolicy = std::derived_from<T, Parallel>;

        template <ExecutionPolicy T>
        constexpr bool isSequencedPolicy = std::derived_from<T, Sequenced>;

    }

}