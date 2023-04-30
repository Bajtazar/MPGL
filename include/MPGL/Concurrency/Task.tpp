/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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

namespace mpgl::async::details {

    template <PureType ReturnTp>
    void PromiseTypeTemplatedInterface<ReturnTp>::
        unhandled_exception(void) noexcept
    {
        finish();
        promise.set_exception(std::current_exception());
    }

    template <PureType ReturnTp>
    template <std::convertible_to<ReturnTp> Vp>
    void PromiseTypeBase<ReturnTp>::return_value(Vp&& value) {
        this->finish();
        this->promise.set_value(std::forward<Vp>(value));
    }

}

namespace mpgl::async {

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    Alloc Task<ReturnTp, Alloc>::allocator = {};

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    Task<ReturnTp, Alloc>::promise_type::promise_type(
        void) noexcept : details::PromiseTypeBase<ReturnTp>{

    [](details::PromiseTypeInterface& reference) -> void {
        promise_type& self = static_cast<promise_type&>(reference);
        if (self.asleep)
            self.threadpool->appendTask(
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
            allocator, static_cast<std::byte*>(ptr), size);
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
        otherPromise.threadpool = this->threadpool;
        ++this->childrenCounter;
        return { this->threadpool->appendTask(std::move(other)) };
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
    [[nodiscard]] std::future<ReturnTp>
        Task<ReturnTp, Alloc>::promise_type::getFuture(void)
    {
        return this->promise.get_future();
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    void Task<ReturnTp, Alloc>::promise_type::AwakenedCoroutine
        ::operator() (void) const
    {
        handle();
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    [[nodiscard]] Task<ReturnTp, Alloc>::operator bool() const {
        return !handle.done();
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    void Task<ReturnTp, Alloc>::operator() (void) const {
        handle();
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    [[nodiscard]] bool Task<ReturnTp, Alloc>::isValid(
        void) const noexcept
    {
        return handle != nullptr;
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    [[nodiscard]] Task<ReturnTp, Alloc>::future_type
        Task<ReturnTp, Alloc>::getFuture(void)
    {
        return handle.promise().getFuture();
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    bool Task<ReturnTp, Alloc>::terminate(void) {
        if (handle) {
            handle.destroy();
            handle = nullptr;
            return true;
        }
        return false;
    }

    template <PureType ReturnTp, Allocator<std::byte> Alloc>
    void Task<ReturnTp, Alloc>::setThreadpool(
        Threadpool* pool) noexcept
    {
        handle.promise().threadpool = pool;
    }

}
