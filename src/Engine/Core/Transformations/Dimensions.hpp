#pragma once

#include <concepts>

namespace ge {

    namespace dim {

        struct Dimension {};

        struct Dim2 : Dimension {};

        struct Dim3 : Dimension {};

    }

    template <class T>
    concept Dimension = std::derived_from<T, dim::Dimension>;

}
