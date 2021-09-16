#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>

#include "RegisterInterface.hpp"

namespace ge {

    template <class T, typename Signature, Signature Method>
    class UniversalRegister : public RegisterInterface {
        UniversalRegister(void) noexcept = delete;
    };

    template <class T, typename... Args, void(T::*EventMethod)(Args...)>
    class UniversalRegister<T, void(T::*)(Args...), EventMethod>
        : public RegisterInterface
    {
    public:
        typedef std::shared_ptr<T>          pointer;
        typedef std::vector<pointer>        Storage;
        typedef T                           value_type;

        explicit UniversalRegister(void) noexcept = default;

        void pushBack(pointer const& pointer)
            { storage.push_back(pointer); }
        void pushBack(pointer&& pointer) noexcept
            { storage.push_back(std::move(pointer)); }

        void onEvent(Args&&... args);

        using iterator = typename Storage::iterator;
        using const_iterator = typename Storage::const_iterator;
        using reverse_iterator = typename Storage::reverse_iterator;
        using const_reverse_iterator = typename Storage::const_reverse_iterator;

        iterator begin(void) noexcept { return storage.begin(); }
        iterator end(void) noexcept { return storage.end(); }
        const_iterator begin(void) const noexcept { return storage.begin(); }
        const_iterator end(void) const noexcept { return storage.end(); }
        const_iterator cbegin(void) const noexcept { return storage.cbegin(); }
        const_iterator cend(void) const noexcept { return storage.cend(); }

        reverse_iterator rbegin(void) noexcept { return storage.rbegin(); }
        reverse_iterator rend(void) noexcept { return storage.rend(); }
        const_reverse_iterator rbegin(void) const noexcept { return storage.rbegin(); }
        const_reverse_iterator rend(void) const noexcept { return storage.rend(); }
        const_reverse_iterator crbegin(void) const noexcept { return storage.crbegin(); }
        const_reverse_iterator crend(void) const noexcept { return storage.crend(); }

        std::size_t size(void) const noexcept { return storage.size(); }
        bool empty(void) const noexcept { return storage.empty(); }

        pointer& operator[] (std::size_t index) noexcept { return storage[index]; }
        pointer const& operator[] (std::size_t index) const noexcept { return storage[index]; }

        ~UniversalRegister(void) noexcept = default;
    private:
        Storage                             storage;
    };

    template <class T, typename... Args, void(T::*EventMethod)(Args...)>
    void UniversalRegister<T, void(T::*)(Args...), EventMethod>::onEvent(Args&&... args) {
        std::ranges::for_each(storage, [...args = std::forward<Args>(args)]
            (auto& event){ (event.get()->*EventMethod)(args...); });
    }

}
