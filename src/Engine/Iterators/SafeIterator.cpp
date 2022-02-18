#include "SafeIterator.hpp"

namespace mpgl {

    SafeIterator<std::istreambuf_iterator<char>,
        std::istreambuf_iterator<char>>::value_type
    SafeIterator<std::istreambuf_iterator<char>,
        std::istreambuf_iterator<char>>::operator* (void) const
    {
        if (iter == sentinel)
            throw SafeIteratorOutOfRangeException{};
        return *iter;
    }

}
