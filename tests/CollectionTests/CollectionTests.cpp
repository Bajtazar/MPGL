#include "CircularListTests.hpp"

Test(CircularListTests) {
    static_assert(CircularListConstructor());
    static_assert(CircularListRangeConstructor());
    static_assert(CircularListAddOneElement());
    static_assert(CircularListAddTwoElements());
    static_assert(CircularListAddMultipleElements());
    static_assert(CircularListCopyConstructor());
    static_assert(CircularListMoveConstructor());
    static_assert(CircularListCopyAsignment());
    static_assert(CircularListMoveAsignment());
    static_assert(CircularListRemoveOneElement());
    static_assert(CircularListRemoveMultipleElements());
}

Test(CircularListRemoveFromEmptyList) {
    mpgl::CircularList<int> list;
    OnThrow(list.pop(), mpgl::CircularListEmptyException)
}
