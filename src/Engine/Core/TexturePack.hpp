#pragma once

#include "Texture.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace ge {

    template <Allocator Alloc = std::allocator<Texture<>>>
    class TexturePack {
    public:
        typedef std::string                         Key;
        typedef typename Alloc::value_type          Value;
        typedef std::pair<Key, Value>               ValuePair;
        typedef std::vector<ValuePair>              ValueVector;

        explicit TexturePack(ValueVector values, Value const& defaultTexture
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
        typedef typename ValueVector::iterator      Iter;
        typedef std::reference_wrapper<NodePtr>     NodeRef;

        NodePtr                     root;
        Value const                 defaultValue;
        void addNode(NodeRef placer, Iter segmentStart, Iter segmentEnd);
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
    TexturePack<Alloc>::TexturePack(ValueVector vector, Value const& defaultValue)
        : root{nullptr}, defaultValue{defaultValue}
    {
        std::ranges::sort(vector, std::ranges::less{}, &ValueVector::value_type::first);
        addNode(std::ref(root), vector.begin(), vector.end());
    }

    template <Allocator Alloc>
    void TexturePack<Alloc>::addNode(NodeRef placer, Iter segmentStart, Iter segmentEnd) {
        for (Iter middle; segmentStart != segmentEnd; segmentStart = ++middle) {
            middle = segmentStart + (std::distance(segmentStart, segmentEnd) / 2);
            placer.get() = std::make_unique<Node>(std::move(middle->first), std::move(middle->second));
            addNode(std::ref(placer.get()->leftNode), segmentStart, middle);
            placer = std::ref(placer.get()->rightNode);
        }
    }

}
