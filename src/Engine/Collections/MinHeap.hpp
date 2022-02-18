#pragma once

#include <algorithm>
#include <vector>
#include <ranges>
#include <memory>

namespace mpgl {

    template <typename T>
    concept IsSmartPtr = std::is_object_v<T> && (
        std::is_same_v<T, std::shared_ptr<typename T::element_type>> ||
        std::is_same_v<T, std::unique_ptr<typename T::element_type>>);

    template <typename T>
    struct ComparePtr {
        using value_type = std::remove_cvref_t<T>;
        using const_reference = const value_type&;

        bool operator() (const_reference left, const_reference right) const noexcept { return (*left) > (*right); }
    };

    template <typename T, class Comparator = std::greater<T>>
    using PtrComparable = std::conditional_t<IsSmartPtr<T>, ComparePtr<T>, Comparator>;

    template <typename T, std::predicate<T, T> Compare = PtrComparable<T>,
        std::ranges::forward_range Sequence = std::vector<T>>
    class MinHeap {
    public:
        typedef Sequence             container_type;
        typedef Compare              value_compare;
        typedef std::size_t          size_type;
        typedef T                    value_type;
        typedef value_type&          reference;
        typedef const value_type&    const_reference;

        explicit MinHeap(const Compare& compare = Compare{}) noexcept : compare{compare} {}

        void push(const_reference element) requires std::copyable<value_type>;
        void push(value_type&& element) requires std::movable<value_type>;
        template <typename... Args>
        void emplace(Args&&... args) requires std::constructible_from<value_type, Args...>;

        value_type popBack(void) requires std::movable<value_type>;

        std::size_t size(void) const noexcept requires std::ranges::sized_range<Sequence> { return sequence.size(); }

        bool empty(void) const noexcept { return sequence.empty(); }
    private:
        Sequence sequence;
        Compare compare;
    };

    template <typename T, std::predicate<T, T> Compare, std::ranges::forward_range Sequence>
    void MinHeap<T, Compare, Sequence>::push(const_reference element) requires std::copyable<value_type> {
        sequence.push_back(element);
        std::ranges::push_heap(sequence, compare);
    }

    template <typename T, std::predicate<T, T> Compare, std::ranges::forward_range Sequence>
    void MinHeap<T, Compare, Sequence>::push(value_type&& element) requires std::movable<value_type> {
        sequence.push_back(std::move(element));
        std::ranges::push_heap(sequence, compare);
    }

    template <typename T, std::predicate<T, T> Compare, std::ranges::forward_range Sequence>
    template <typename... Args>
    void MinHeap<T, Compare, Sequence>::emplace(Args&&... args) requires std::constructible_from<value_type, Args...> {
        sequence.emplace_back(std::forward<Args>(args)...);
        std::ranges::push_heap(sequence, compare);
    }

    template <typename T, std::predicate<T, T> Compare, std::ranges::forward_range Sequence>
    T MinHeap<T, Compare, Sequence>::popBack(void) requires std::movable<value_type> {
        std::ranges::pop_heap(sequence, compare);
        auto result = std::move(sequence.back());
        sequence.pop_back();
        return result;
    }

}

template<class T, std::predicate<T, T> Compare,
    std::ranges::forward_range Container, class Alloc>
struct std::uses_allocator<mpgl::MinHeap<T, Compare, Container>,Alloc> :
    std::uses_allocator<Container, Alloc>::type { };
