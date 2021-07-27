#pragma once

#include "../Exceptions/HuffmanTreeEmptyMapException.hpp"
#include "../Utility/Concepts.hpp"
#include "../Collections/MinHeap.hpp"

#include <map>

namespace ge {

    namespace compression {

        enum class Direction {
            Compress,
            Decompress
        };

    }

    template <compression::Direction Mode, Arithmetic FrequencyType = std::size_t>
    class HuffmanTree {
    public:
        typedef std::map<uint8_t, FrequencyType> CharactersMap;
        typedef std::map<uint8_t, std::string> CodesMap;

        explicit HuffmanTree(const CharactersMap& data);

        CodesMap getCoding(void) const noexcept;
    private:
        struct Node {
            std::unique_ptr<Node> leftNode;
            std::unique_ptr<Node> rightNode;
            FrequencyType frequency;
            uint8_t character;

            explicit Node(uint8_t character, const FrequencyType& frequency)
                : leftNode{nullptr}, rightNode{nullptr}, frequency{frequency},
                character{character} {}
            explicit Node(const std::pair<uint8_t, FrequencyType>& frequencyPair)
                : leftNode{nullptr}, rightNode{nullptr}, frequency{frequencyPair.second},
                character{frequencyPair.first} {}

            friend bool operator> (const Node& left, const Node& right) noexcept { return left.frequency > right.frequency; }
            friend bool operator< (const Node& left, const Node& right) noexcept { return left.frequency < right.frequency; }
        };

        typedef std::unique_ptr<Node> NodePtr;
        typedef std::reference_wrapper<const NodePtr> NodeCRef;

        void walkThrough(NodeCRef node, CodesMap& map, std::string code) const;

        NodePtr root;
    };

    template <compression::Direction Mode, Arithmetic FrequencyType>
    HuffmanTree<Mode, FrequencyType>::HuffmanTree(const CharactersMap& data) : root{nullptr} {
        if (!data.size())
            throw HuffmanTreeEmptyMapException{};
        MinHeap<NodePtr> heap;
        for (const auto& frequencyPair : data)
            heap.push(std::make_unique<Node>(frequencyPair));
        while (heap.size() != 1) {
            auto left = heap.popBack();
            auto right = heap.popBack();
            auto node = std::make_unique<Node>('$', left->frequency + right->frequency);
            node->leftNode = std::move(left);
            node->rightNode = std::move(right);
            heap.push(std::move(node));
        }
        root = std::move(heap.popBack());
    }

    template <compression::Direction Mode, Arithmetic FrequencyType>
    void HuffmanTree<Mode, FrequencyType>::walkThrough(NodeCRef node, CodesMap& map, std::string code) const {
        while (node.get()) {
            if (node.get()->character != '$')
                map[node.get()->character] = code;
            walkThrough(std::cref(node.get()->leftNode), map, code + "0");
            node = std::cref(node.get()->rightNode);
            code += "1";
        }
    }

    template <compression::Direction Mode, Arithmetic FrequencyType>
        HuffmanTree<Mode, FrequencyType>::CodesMap
    HuffmanTree<Mode, FrequencyType>::getCoding(void) const noexcept {
        CodesMap codesMap;
        walkThrough(root, codesMap, "");
        return codesMap;
    }

}
