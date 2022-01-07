#pragma once

#include "../TestsFramework/Tests.hpp"
#include "../../src/Engine/Traits/Types.hpp"
#include "../../src/Engine/Collections/CircularList.hpp"

#include <array>

BeginConstexprTest(CircularListConstructor)
    ge::CircularList<int> list;
EndConstexprTest

BeginConstexprTest(CircularListAddOneElement)
    ge::CircularList<int> list;
    list.push(4);
EndConstexprTest

BeginConstexprTest(CircularListAddTwoElements)
    ge::CircularList<int> list;
    list.push(4);
    list.push(5);
EndConstexprTest

BeginConstexprTest(CircularListAddMultipleElements)
    ge::CircularList<int> list;
    for (ge::uint8 i = 0;i < 100; ++i)
        list.push(i);
EndConstexprTest

BeginConstexprTest(CircularListRangeConstructor)
    std::array vec{1, 2, 3, 4, 5};
    ge::CircularList<int> list{vec};
EndConstexprTest

BeginConstexprTest(CircularListCopyConstructor)
    ge::CircularList<int> list;
    for (ge::uint8 i = 0;i < 100; ++i)
        list.push(i);
    auto cpy = list;
    auto i1 = list.attachment(), i2 = cpy.attachment();
    auto end1 = i1, end2 = i2;
    for (++i1, ++i2; i1 != end1 && i2 != end2; ++i1, ++i2)
        ConstexprEqual(*i1, *i2)
EndConstexprTest

BeginConstexprTest(CircularListMoveConstructor)
    ge::CircularList<int> list;
    for (ge::uint8 i = 0;i < 100; ++i)
        list.push(i);
    auto cpy = std::move(list);
    ConstexprAssert(list.empty())
    auto iter = ++cpy.attachment();
    for (int i = 0;i < 100; ++i, ++iter)
        ConstexprEqual(i, *iter)
EndConstexprTest

BeginConstexprTest(CircularListCopyAsignment)
    ge::CircularList<int> list, cpy;
    for (ge::uint8 i = 0;i < 100; ++i)
        list.push(i);
    cpy = list;
    auto i1 = list.attachment(), i2 = cpy.attachment();
    auto end1 = i1, end2 = i2;
    for (++i1, ++i2; i1 != end1 && i2 != end2; ++i1, ++i2)
        ConstexprEqual(*i1, *i2)
EndConstexprTest

BeginConstexprTest(CircularListMoveAsignment)
    ge::CircularList<int> list, cpy;
    for (ge::uint8 i = 0;i < 100; ++i)
        list.push(i);
    cpy = std::move(list);
    ConstexprAssert(list.empty())
    auto iter = ++cpy.attachment();
    for (int i = 0;i < 100; ++i, ++iter)
        ConstexprEqual(i, *iter)
EndConstexprTest

BeginConstexprTest(CircularListRemoveOneElement)
    ge::CircularList<int> list;
    list.push(1);
    ConstexprEqual(list.pop(), 1)
    ConstexprAssert(list.empty())
EndConstexprTest

BeginConstexprTest(CircularListRemoveMultipleElements)
    ge::CircularList<int> list;
    for (ge::uint8 i = 0;i < 100; ++i)
        list.push(i);
    ConstexprEqual(list.pop(), 99)
    ConstexprEqual(list.pop(), 98)
    auto iter = ++list.attachment();
    for (int i = 0;i < 98; ++i, ++iter)
        ConstexprEqual(i, *iter)
    ConstexprAssert(*iter == 0)
EndConstexprTest
