#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>

#include "RegisterInterface.hpp"

namespace ge {

    template <class T, void (T::* EventMethod)(void)>
        requires requires (T& t) { (t->*EventMethod)(); }
    class UniversalRegister : public RegisterInterface {
    public:
        explicit UniversalRegister(void) noexcept = default;

        void pushBack(const std::shared_ptr<T>& pointer) noexcept { storage.push_back(pointer); }
        void pushBack(std::shared_ptr<T>&& pointer) noexcept { storage.push_back(std::move(pointer)); }

        virtual void onEvent(void) noexcept final;

        using value_type = std::shared_ptr<T>;
        using iterator = std::vector<T>::iterator;
        using const_iterator = std::vector<T>::const_iterator;
        using reverse_iterator = std::vector<T>::reverse_iterator;
        using const_reverse_iterator = std::vector<T>::const_reverse_iterator;

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

        std::shared_ptr<T>& operator[] (std::size_t index) noexcept { return storage[index]; }
        const std::shared_ptr<T>& operator[] (std::size_t index) const noexcept { return storage[index]; }

        ~UniversalRegister(void) noexcept = default;
    private:
        std::vector<std::shared_ptr<T>> storage;
    };

    template <class T, void (T::* EventMethod)(void)>
        requires requires (T& t) { (t->*EventMethod)(); }
    void UniversalRegister<T, EventMethod>::onEvent(void) noexcept {
        std::ranges::for_each(storage, [](auto& event){ (event->*EventMethod)(); });
    }

}
