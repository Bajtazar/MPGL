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

namespace mpgl::async {

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    Alloc Task<ReturnTp, Alloc>::allocator = {};

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    Task<ReturnTp, Alloc>::promise_type::promise_type(
        void) noexcept : details::PromiseTypeInterface{

    .awake=[](details::PromiseTypeInterface const& ref) {
        promise_type const& self = static_cast<promise_type const&>(ref);
        if (self.asleep)
            self.pool->appendTask(
                AwakenedCoroutine{handle_t::from_promise(self)});
        self.asleep = false;
    }

    } {}

}
