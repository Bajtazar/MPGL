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

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    template <PureType Up>
    [[nodiscard]] bool Task<ReturnTp, Alloc>::promise_type::
        YieldAwaiter<Up>::await_ready(void) const noexcept
    {
        return false;
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    template <PureType Up>
    [[nodiscard]] bool Task<ReturnTp, Alloc>::promise_type::
        YieldAwaiter<Up>::await_suspend(
            [[maybe_unused]] std::coroutine_handle<>
            ) const noexcept
    {
        return false;
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    template <PureType Up>
    [[nodiscard]] std::future<Up> Task<ReturnTp, Alloc>::
        promise_type::YieldAwaiter<Up>::await_resume(void) noexcept
    {
        return std::move(future);
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    [[nodiscard]] bool Task<ReturnTp, Alloc>::promise_type::
        SynchronizeAwaiter::await_ready(void) const noexcept
    {
        return counter->load() == 0;
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    [[nodiscard]] void*
        Task<ReturnTp, Alloc>::promise_type::operator new(
            std::size_t size)
    {
        return std::allocator_traits<Alloc>::allocate(
            allocator, size);
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    void Task<ReturnTp, Alloc>::promise_type::operator delete(
        void* ptr, std::size_t size)
    {
        std::allocator_traits<Alloc>::deallocate(
            allocator, ptr, size);
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    [[nodiscard]] Task<ReturnTp, Alloc>
        Task<ReturnTp, Alloc>::promise_type::get_return_object(
            void) noexcept
    {
        return Task{ handle_t::from_promise(*this) };
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    template <PureType Up>
    [[nodiscard]] Task<ReturnTp, Alloc>::promise_type::YieldAwaiter<Up>
        Task<ReturnTp, Alloc>::promise_type::yield_value(
            Task<Up>&& other)
    {
        auto& otherPromise = other.handle.promise();
        otherPromise.parent = this;
        otherPromise.pool = this->pool;
        ++this->childrenCounter;
        return { this->pool->appendTask(std::move(other)) };
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    [[nodiscard]] Task<ReturnTp, Alloc>::promise_type::SynchronizeAwaiter
        Task<ReturnTp, Alloc>::promise_type::await_transform(
            [[maybe_unused]] synchronize_t) noexcept
    {
        asleep = true;
        return { &this->childrenCounter };
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    template <std::convertible_to<ReturnTp> Vp>
    void Task<ReturnTp, Alloc>::promise_type::return_value(
        Vp&& value)
    {
        finish();
        asleep = false;
        promise.set_value(std::forward<Vp>(value));
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    void Task<ReturnTp, Alloc>::promise_type::unhandled_exception(
        void) noexcept
    {
        finish();
        asleep = false;
        promise.set_exception(std::current_exception());
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    [[nodiscard]] std::future<ReturnTp>
        Task<ReturnTp, Alloc>::promise_type::getFuture(void)
    {
        return return promise.get_future();
    }

}
