#pragma once

#include "../Exceptions/HuffmanTreeEmptyMapException.hpp"
#include "../Collections/MinHeap.hpp"
#include "../Utility/Concepts.hpp"

#include <unordered_map>
#include <map>

namespace ge {

    template <typename CharType = uint8_t, SizeType FrequencyType = std::size_t, CharType InnerNode = '$'>
    class HuffmanTree {
    public:
        typedef std::unordered_map<CharType, FrequencyType>     DecodingMap;
        typedef std::map<CharType, FrequencyType>               CharactersMap;
        typedef std::map<CharType, std::string>                 CodesMap;
        typedef std::vector<CharType>                           FrequencyArray;

        explicit HuffmanTree(const CharactersMap& data);

        CodesMap getCoding(void) const noexcept;
        DecodingMap getDecoder(void) const noexcept;

        static DecodingMap createDecodingMap(const FrequencyArray& frequency);
        static DecodingMap createDefaultDecoder(void);

    private:
        struct Node {
            std::unique_ptr<Node> leftNode;
            std::unique_ptr<Node> rightNode;
            FrequencyType frequency;
            CharType character;

            explicit Node(CharType character = 0, const FrequencyType& frequency = 0)
                : leftNode{nullptr}, rightNode{nullptr}, frequency{frequency},
                character{character} {}
            explicit Node(const std::pair<CharType, FrequencyType>& frequencyPair)
                : leftNode{nullptr}, rightNode{nullptr}, frequency{frequencyPair.second},
                character{frequencyPair.first} {}

            friend bool operator> (const Node& left, const Node& right) noexcept { return left.frequency > right.frequency; }
            friend bool operator< (const Node& left, const Node& right) noexcept { return left.frequency < right.frequency; }
        };

        typedef std::unique_ptr<Node>                            NodePtr;
        typedef std::reference_wrapper<const NodePtr>            NodeCRef;

        typedef std::array<FrequencyType, sizeof(FrequencyType)> CountedArray;
        typedef std::unordered_map<CharType, FrequencyType>      FrequencyMap;

        void walkThrough(NodeCRef node, CodesMap& map, std::string code) const;
        void walkThrough(NodeCRef node, DecodingMap& map, FrequencyType code) const;

        NodePtr root;

        static CountedArray generateFrequencyArray(const FrequencyArray& frequency);
        static CountedArray generateSmallestCodes(const CountedArray& counted, FrequencyType min, FrequencyType max);
    };

    template <typename CharType, SizeType FrequencyType, CharType InnerNode>
    HuffmanTree<CharType, FrequencyType, InnerNode>::HuffmanTree(const CharactersMap& data) : root{nullptr} {
        if (!data.size())
            throw HuffmanTreeEmptyMapException{};
        MinHeap<NodePtr> heap;
        for (const auto& frequencyPair : data)
            heap.push(std::make_unique<Node>(frequencyPair));
        while (heap.size() != 1) {
            auto left = heap.popBack();
            auto right = heap.popBack();
            auto node = std::make_unique<Node>(InnerNode, left->frequency + right->frequency);
            node->leftNode = std::move(left);
            node->rightNode = std::move(right);
            heap.push(std::move(node));
        }
        root = std::move(heap.popBack());
    }

    template <typename CharType, SizeType FrequencyType, CharType InnerNode>
    HuffmanTree<CharType, FrequencyType, InnerNode>::CountedArray
        HuffmanTree<CharType, FrequencyType, InnerNode>::generateFrequencyArray(const FrequencyArray& frequency)
    {
        CountedArray counted;
        counted.fill(0);
        for (const auto& length : frequency)
            counted[length]++;
        counted[0] = 0;
        return counted;
    }

    template <typename CharType, SizeType FrequencyType, CharType InnerNode>
    HuffmanTree<CharType, FrequencyType, InnerNode>::CountedArray
        HuffmanTree<CharType, FrequencyType, InnerNode>::generateSmallestCodes(const CountedArray& counted, FrequencyType min, FrequencyType max)
    {
        CountedArray smallestCodes;
        FrequencyType code = 0;
        for (auto bits : std::views::iota(min, max + 1))
            smallestCodes[bits] = code = (code + counted[bits - 1]) << 1;
        return smallestCodes;
    }

    template <typename CharType, SizeType FrequencyType, CharType InnerNode>
    HuffmanTree<CharType, FrequencyType, InnerNode>::DecodingMap
        HuffmanTree<CharType, FrequencyType, InnerNode>::createDecodingMap(const FrequencyArray& frequency)
    {
        auto [minFreq, maxFreq] = std::ranges::minmax_element(frequency);
        CountedArray smallestCodes = generateSmallestCodes(generateFrequencyArray(frequency), *minFreq, *maxFreq);
        DecodingMap decoder;
        std::ranges::for_each(frequency, [&, i = 0](const auto& bits) mutable -> void {
            if (auto iter = i++; bits) {
                decoder[iter] = smallestCodes[bits];
                ++smallestCodes[bits];
            }
        });
        return decoder;
    }

    template <typename CharType, SizeType FrequencyType, CharType InnerNode>
    HuffmanTree<CharType, FrequencyType, InnerNode>::DecodingMap
        HuffmanTree<CharType, FrequencyType, InnerNode>::createDefaultDecoder(void)
    {
        FrequencyArray frequency;
        frequency.reserve(288);
        std::ranges::fill_n(std::back_inserter(frequency), 144, 8);
        std::ranges::fill_n(std::back_inserter(frequency), 112, 9);
        std::ranges::fill_n(std::back_inserter(frequency), 24, 7);
        std::ranges::fill_n(std::back_inserter(frequency), 8, 8);
        return createDecodingMap(frequency);
    }

    template <typename CharType, SizeType FrequencyType, CharType InnerNode>
    void HuffmanTree<CharType, FrequencyType, InnerNode>::walkThrough(NodeCRef node, CodesMap& map, std::string code) const {
        while (node.get()) {
            if (node.get()->character != InnerNode)
                map[node.get()->character] = code;
            walkThrough(std::cref(node.get()->leftNode), map, code + "0");
            node = std::cref(node.get()->rightNode);
            code += "1";
        }
    }

    template <typename CharType, SizeType FrequencyType, CharType InnerNode>
        HuffmanTree<CharType, FrequencyType, InnerNode>::CodesMap
    HuffmanTree<CharType, FrequencyType, InnerNode>::getCoding(void) const noexcept {
        CodesMap codesMap;
        walkThrough(root, codesMap, "");
        return codesMap;
    }

    template <typename CharType, SizeType FrequencyType, CharType InnerNode>
        HuffmanTree<CharType, FrequencyType, InnerNode>::DecodingMap
    HuffmanTree<CharType, FrequencyType, InnerNode>::getDecoder(void) const noexcept {
        DecodingMap codesMap;
        walkThrough(root, codesMap, 0);
        return codesMap;
    }

    template <typename CharType, SizeType FrequencyType, CharType InnerNode>
    void HuffmanTree<CharType, FrequencyType, InnerNode>::walkThrough(NodeCRef node, DecodingMap& map, FrequencyType code) const {
        while (node.get()) {
            if (node.get()->character != InnerNode)
                map[node.get()->character] = code;
            walkThrough(std::cref(node.get()->leftNode), map, 2 * code);
            node = std::cref(node.get()->rightNode);
            code = 2 * code + 1;
        }
    }


}
