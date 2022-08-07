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
#include <MPGL/Core/Layouts/LayoutHolder.hpp>

namespace mpgl {

    LayoutHolder::InlineMemory::InlineMemory(
        InlineMemory&& inlineMemory) noexcept
    {
        inlineMemory->move(memory.front());
    }

    LayoutHolder::InlineMemory&
        LayoutHolder::InlineMemory::operator=(
            InlineMemory&& inlineMemory) noexcept
    {
        this->~InlineMemory();
        inlineMemory->move(memory.front());
        return *this;
    }

    LayoutHolder::LayoutInterface*
        LayoutHolder::InlineMemory::get(void) const noexcept
    {
        return const_cast<LayoutInterface*>(
            reinterpret_cast<const LayoutInterface*>(memory.data()));
    }

    LayoutHolder::InlineMemory::~InlineMemory(void) noexcept {
        (*this)->~LayoutInterface();
    }

    LayoutHolder::LayoutInterface*
        LayoutHolder::pointer(void) const noexcept
    {
        if (std::holds_alternative<LayoutPtr>(storage))
            return std::get<LayoutPtr>(storage).get();
        return std::get<InlineMemory>(storage).get();
    }

}
