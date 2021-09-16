#pragma once

#include <concepts>

namespace ge {

    namespace dimm {

        struct Dimmension {};

        struct Dimm2 : Dimmension {};

        struct Dimm3 : Dimmension {};

    }

    template <class T>
    concept Dimmension = std::derived_from<T, dimm::Dimmension>;

}
