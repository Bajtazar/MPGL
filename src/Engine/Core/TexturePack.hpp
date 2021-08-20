#pragma once

#include "../Exceptions/ExecusionUnknownPolicyException.hpp"
#include "../Utility/Execution.hpp"
#include "Texture.hpp"

#include <algorithm>
#include <execution>
#include <thread>
#include <ranges>
#include <memory>
#include <vector>
#include <future>

namespace ge {

    template <Allocator Alloc = std::allocator<Texture<>>>
    class TexturePack {
    public:
        typedef std::string                         Key;
        typedef typename Alloc::value_type          Value;
        typedef std::pair<Key, Value>               ValuePair;

        template <std::ranges::random_access_range Range>
            requires std::same_as<std::ranges::range_value_t<Range>, ValuePair>
        explicit TexturePack(Range&& values, Value const& defaultTexture
            = Value::defaultTexture());

        template <execution::ExecutionPolicy Policy, std::ranges::random_access_range Range>
            requires std::same_as<std::ranges::range_value_t<Range>, ValuePair>
        explicit TexturePack(Policy policy, Range&& values, Value const& defaultTexture
            = Value::defaultTexture());

        Value const& operator[] (Key const& key) const;
    private:
        struct Node {
            typedef std::unique_ptr<Node>           NodePtr;

            Key const               key;
            Value const             value;
            NodePtr                 rightNode;
            NodePtr                 leftNode;

            explicit Node(Key&& key, Value&& value) :
                key{std::move(key)}, value{std::move(value)},
                rightNode{nullptr}, leftNode{nullptr} {}
        };

        typedef typename Node::NodePtr              NodePtr;
        typedef std::reference_wrapper<NodePtr>     NodeRef;
        typedef std::vector<std::future<void>>      FutureVector;

        NodePtr                     root;
        Value const                 defaultValue;

        template <std::ranges::random_access_range Range>
        void sequenceInit(Range&& range);
        template <std::ranges::random_access_range Range>
        void parallelInit(Range&& range);

        template <std::random_access_iterator Iter>
        void sequenceAddNode(NodeRef placer, Iter segmentStart, Iter segmentEnd);
        template <std::random_access_iterator Iter>
        void parallelAddNode(NodeRef placer, Iter segmentStart, Iter segmentEnd);
        template <std::random_access_iterator Iter>
        void invokeParallel(FutureVector& vector, NodeRef placer, Iter segmentStart, Iter segmentEnd);

        static constexpr std::size_t perThread = 25;
    };

    template <Allocator Alloc>
    TexturePack<Alloc>::Value const& TexturePack<Alloc>::operator[] (Key const& key) const {
        auto node = std::cref(root);
        while (node.get()) {
            if (node.get()->key == key)
                return node.get()->value;
            node = std::cref(key > node.get()->key ? node.get()->rightNode : node.get()->leftNode);
        }
        return defaultValue;
    }

    template <Allocator Alloc>
    template <std::ranges::random_access_range Range>
        requires std::same_as<std::ranges::range_value_t<Range>, typename TexturePack<Alloc>::ValuePair>
    TexturePack<Alloc>::TexturePack(Range&& vector, Value const& defaultValue)
        : root{nullptr}, defaultValue{defaultValue}
    {
        sequenceInit(vector);
    }

    template <Allocator Alloc>
    template <execution::ExecutionPolicy Policy, std::ranges::random_access_range Range>
        requires std::same_as<std::ranges::range_value_t<Range>, typename TexturePack<Alloc>::ValuePair>
    TexturePack<Alloc>::TexturePack([[maybe_unused]]Policy policy, Range&& vector, Value const& defaultValue)
        : root{nullptr}, defaultValue{defaultValue}
    {
        if constexpr (execution::isParallelPolicy<Policy>)
            parallelInit(vector);
        else if constexpr (execution::isSequencedPolicy<Policy>)
            sequenceInit(vector);
        else
            throw ExecusionUnknownPolicyException{};
    }

    template <Allocator Alloc>
    template <std::ranges::random_access_range Range>
    void TexturePack<Alloc>::sequenceInit(Range&& range) {

        std::ranges::sort(range, std::ranges::less{}, &std::remove_cvref_t<Range>::value_type::first);
        sequenceAddNode(std::ref(root), std::ranges::begin(range), std::ranges::end(range));
    }

    template <Allocator Alloc>
    template <std::ranges::random_access_range Range>
    void TexturePack<Alloc>::parallelInit(Range&& range) {
        std::sort(std::execution::par, std::ranges::begin(range), std::ranges::end(range),
            [](auto const& left, auto const& right) { return left.first < right.first; });
        if (std::ranges::distance(range) > perThread)
            parallelAddNode(std::ref(root), std::ranges::begin(range), std::ranges::end(range));
        else
            sequenceAddNode(std::ref(root), std::ranges::begin(range), std::ranges::end(range));
    }

    template <Allocator Alloc>
    template <std::random_access_iterator Iter>
    void TexturePack<Alloc>::sequenceAddNode(NodeRef placer, Iter segmentStart, Iter segmentEnd) {
        for (Iter middle; segmentStart != segmentEnd; segmentStart = ++middle) {
            middle = segmentStart + (std::distance(segmentStart, segmentEnd) / 2);
            placer.get() = std::make_unique<Node>(std::move(middle->first), std::move(middle->second));
            sequenceAddNode(std::ref(placer.get()->leftNode), segmentStart, middle);
            placer = std::ref(placer.get()->rightNode);
        }
    }

    template <Allocator Alloc>
    template <std::random_access_iterator Iter>
    void TexturePack<Alloc>::invokeParallel(FutureVector& vector, NodeRef placer, Iter segmentStart, Iter segmentEnd) {
        if (std::distance(segmentStart, segmentStart) > perThread)
            vector.emplace_back(std::async(&TexturePack<Alloc>::parallelAddNode<Iter>, this,
                placer, segmentStart, segmentEnd));
        else
            vector.emplace_back(std::async(&TexturePack<Alloc>::sequenceAddNode<Iter>, this,
                placer, segmentStart, segmentEnd));
    }

    template <Allocator Alloc>
    template <std::random_access_iterator Iter>
    void TexturePack<Alloc>::parallelAddNode(NodeRef placer, Iter segmentStart, Iter segmentEnd) {
        FutureVector futures;
        for (Iter middle; segmentStart != segmentEnd; segmentStart = ++middle) {
            middle = segmentStart + (std::distance(segmentStart, segmentEnd) / 2);
            placer.get() = std::make_unique<Node>(std::move(middle->first), std::move(middle->second));
            invokeParallel(futures, std::ref(placer.get()->leftNode), segmentStart, middle);
            placer = std::ref(placer.get()->rightNode);
        }
        std::for_each(std::execution::par, futures.begin(), futures.end(),
            [](auto& future){ future.get(); });
    }

}
