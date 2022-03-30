/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

#include "../TestsFramework/Tests.hpp"
#include "../../src/Engine/Traits/Types.hpp"
#include "../../src/Engine/Collections/CircularList.hpp"

#include <array>

BeginConstexprTest(CircularListConstructor)
    mpgl::CircularList<int> list;
EndConstexprTest

BeginConstexprTest(CircularListAddOneElement)
    mpgl::CircularList<int> list;
    list.push(4);
EndConstexprTest

BeginConstexprTest(CircularListAddTwoElements)
    mpgl::CircularList<int> list;
    list.push(4);
    list.push(5);
EndConstexprTest

BeginConstexprTest(CircularListAddMultipleElements)
    mpgl::CircularList<int> list;
    for (mpgl::uint8 i = 0;i < 100; ++i)
        list.push(i);
EndConstexprTest

BeginConstexprTest(CircularListRangeConstructor)
    std::array vec{1, 2, 3, 4, 5};
    mpgl::CircularList<int> list{vec};
EndConstexprTest

BeginConstexprTest(CircularListCopyConstructor)
    mpgl::CircularList<int> list;
    for (mpgl::uint8 i = 0;i < 100; ++i)
        list.push(i);
    auto cpy = list;
    auto i1 = list.attachment(), i2 = cpy.attachment();
    auto end1 = i1, end2 = i2;
    for (++i1, ++i2; i1 != end1 && i2 != end2; ++i1, ++i2)
        ConstexprEqual(*i1, *i2)
EndConstexprTest

BeginConstexprTest(CircularListMoveConstructor)
    mpgl::CircularList<int> list;
    for (mpgl::uint8 i = 0;i < 100; ++i)
        list.push(i);
    auto cpy = std::move(list);
    ConstexprAssert(list.empty())
    auto iter = ++cpy.attachment();
    for (int i = 0;i < 100; ++i, ++iter)
        ConstexprEqual(i, *iter)
EndConstexprTest

BeginConstexprTest(CircularListCopyAsignment)
    mpgl::CircularList<int> list, cpy;
    for (mpgl::uint8 i = 0;i < 100; ++i)
        list.push(i);
    cpy = list;
    auto i1 = list.attachment(), i2 = cpy.attachment();
    auto end1 = i1, end2 = i2;
    for (++i1, ++i2; i1 != end1 && i2 != end2; ++i1, ++i2)
        ConstexprEqual(*i1, *i2)
EndConstexprTest

BeginConstexprTest(CircularListMoveAsignment)
    mpgl::CircularList<int> list, cpy;
    for (mpgl::uint8 i = 0;i < 100; ++i)
        list.push(i);
    cpy = std::move(list);
    ConstexprAssert(list.empty())
    auto iter = ++cpy.attachment();
    for (int i = 0;i < 100; ++i, ++iter)
        ConstexprEqual(i, *iter)
EndConstexprTest

BeginConstexprTest(CircularListRemoveOneElement)
    mpgl::CircularList<int> list;
    list.push(1);
    ConstexprEqual(list.pop(), 1)
    ConstexprAssert(list.empty())
EndConstexprTest

BeginConstexprTest(CircularListRemoveMultipleElements)
    mpgl::CircularList<int> list;
    for (mpgl::uint8 i = 0;i < 100; ++i)
        list.push(i);
    ConstexprEqual(list.pop(), 99)
    ConstexprEqual(list.pop(), 98)
    auto iter = ++list.attachment();
    for (int i = 0;i < 98; ++i, ++iter)
        ConstexprEqual(i, *iter)
    ConstexprAssert(*iter == 0)
EndConstexprTest

BeginConstexprTest(CircularListRemoveFromEmptyList)
    mpgl::CircularList<int> list;
    ConstexprOnThrow((void)list.pop(),
        mpgl::CircularListEmptyException)
EndConstexprTest
