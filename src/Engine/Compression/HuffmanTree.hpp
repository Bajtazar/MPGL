#pragma once

#include "../Exceptions/HuffmanTreeEmptyMapException.hpp"
#include "../Exceptions/HuffmanTreeDecoderException.hpp"
#include "../Iterators/BitIterator.hpp"
#include "../Collections/MinHeap.hpp"
#include "../Traits/Concepts.hpp"
#include "../Utility/Ranges.hpp"

#include <unordered_map>
#include <climits>
#include <bitset>
#include <map>

namespace ge {

    template <typename CharType = uint8, SizeType FrequencyType = std::size_t>
    class HuffmanTree {
    public:
        typedef std::unordered_map<CharType, FrequencyType>     DecodingMap;
        typedef std::map<CharType, FrequencyType>               CharactersMap;
        typedef std::map<CharType, std::string>                 CodesMap;
        typedef std::vector<CharType>                           FrequencyArray;

        template <RandomAccessConvertible<CharType> CountedRange, ForwardConvertible<CharType> CharRange>
        explicit HuffmanTree(const CountedRange& counted, const CharRange& characters);
        template <ForwardConvertible<CharType> FreqRange>
        explicit HuffmanTree(const FreqRange& frequency);
        explicit HuffmanTree(const CharactersMap& data);

        HuffmanTree(const HuffmanTree& tree) noexcept = delete;
        HuffmanTree(HuffmanTree&& tree) : root{std::move(tree.root)} {}

        HuffmanTree& operator= (const HuffmanTree& tree) noexcept = delete;
        HuffmanTree& operator= (HuffmanTree&& tree) { root = std::move(tree.root); return *this; }

        CodesMap getCoding(void) const noexcept;
        DecodingMap getDecoder(void) const noexcept;

        static HuffmanTree createDeflateDecoder(void) requires (sizeof(CharType) * CHAR_BIT > 8);

        class Decoder {
        public:
            Decoder(HuffmanTree&& tree) : tree{std::move(tree)} {}
            Decoder(void) : tree{createDeflateDecoder()} {}

            template <BitIterator Iter>
            CharType decodeToken(Iter& iterator) const;
        private:
            HuffmanTree tree;
        };

        ~HuffmanTree(void) noexcept = default;
    private:
        struct Node {
            std::unique_ptr<Node> leftNode;
            std::unique_ptr<Node> rightNode;
            FrequencyType frequency;
            CharType character;
            bool isInner;

            explicit Node(CharType character = 0, const FrequencyType& frequency = 0, bool isInner = false)
                : leftNode{nullptr}, rightNode{nullptr}, frequency{frequency},
                character{character}, isInner{isInner} {}
            explicit Node(const std::pair<CharType, FrequencyType>& frequencyPair)
                : leftNode{nullptr}, rightNode{nullptr}, frequency{frequencyPair.second},
                character{frequencyPair.first}, isInner{false} {}

            friend bool operator> (const Node& left, const Node& right) noexcept { return left.frequency > right.frequency; }
            friend bool operator< (const Node& left, const Node& right) noexcept { return left.frequency < right.frequency; }
        };

        typedef std::unique_ptr<Node>                                   NodePtr;
        typedef std::reference_wrapper<const NodePtr>                   NodeCRef;

        typedef std::array<FrequencyType, sizeof(CharType) * CHAR_BIT>  CountedArray;
        typedef std::vector<std::pair<uint8, uint8>>                    CharacterLengthArray;

        void walkThrough(NodeCRef node, CodesMap& map, std::string code) const;
        void walkThrough(NodeCRef node, DecodingMap& map, FrequencyType code) const;
        void addNode(CharType length, CharType character, FrequencyType code);

        NodePtr root;

        template <ForwardConvertible<CharType> FreqRange>
        CountedArray generateFrequencyArray(const FreqRange& frequency);
        template <ForwardConvertible<FrequencyType> CountedRange>
        CountedArray generateSmallestCodes(const CountedRange& counted, FrequencyType min, FrequencyType max);
    };

    template <typename CharType, SizeType FrequencyType>
    HuffmanTree<CharType, FrequencyType>::HuffmanTree(const CharactersMap& data) : root{nullptr} {
        if (!data.size())
            throw HuffmanTreeEmptyMapException{};
        MinHeap<NodePtr> heap;
        for (const auto& frequencyPair : data)
            heap.push(std::make_unique<Node>(frequencyPair));
        while (heap.size() != 1) {
            auto left = heap.popBack();
            auto right = heap.popBack();
            auto node = std::make_unique<Node>(0, left->frequency + right->frequency, true);
            node->leftNode = std::move(left);
            node->rightNode = std::move(right);
            heap.push(std::move(node));
        }
        root = std::move(heap.popBack());
    }

    template <typename CharType, SizeType FrequencyType>
    template <ForwardConvertible<CharType> FreqRange>
    HuffmanTree<CharType, FrequencyType>::HuffmanTree(const FreqRange& frequency) : root{nullptr} {
        auto [minFreq, maxFreq] = std::ranges::minmax_element(frequency);
        auto smallestCodes = generateSmallestCodes(generateFrequencyArray(frequency), *minFreq, *maxFreq);
        std::ranges::for_each(frequency, [&, i = 0](const auto& bits) mutable -> void {
            if (auto iter = i++; bits)
                addNode(bits, iter, smallestCodes[bits]++);
        });
    }

    template <typename CharType, SizeType FrequencyType>
    template <RandomAccessConvertible<CharType> CountedRange, ForwardConvertible<CharType> CharRange>
    HuffmanTree<CharType, FrequencyType>::HuffmanTree(const CountedRange& counted, const CharRange& characters)
        : root{nullptr}
    {
        auto [minFreq, maxFreq] = findFirstAndLastIf(counted, [](const auto& value)->bool{ return value != 0; });
        auto smallestCodes = generateSmallestCodes(counted, minFreq - counted.begin(), maxFreq - counted.begin());
        auto iter = characters.begin();
        std::ranges::for_each(counted, [&, i = 0](auto code) mutable -> void {
            auto length = i++;
            for ([[maybe_unused]] auto _ : std::views::iota(static_cast<CharType>(0), code))
                addNode(length, *iter++, smallestCodes[length]++);
        });
    }

    template <typename CharType, SizeType FrequencyType>
    void HuffmanTree<CharType, FrequencyType>::addNode(CharType length, CharType character, FrequencyType code) {
        auto node = std::make_unique<Node>(character, length);
        std::reference_wrapper<NodePtr> actualNode {root};
        std::bitset<sizeof(FrequencyType) * CHAR_BIT> bits {code};
        while (length--) {
            if (!actualNode.get())
                actualNode.get() = std::move(std::make_unique<Node>(0, 0, true));
            actualNode = std::ref(bits[length] ? actualNode.get()->rightNode : actualNode.get()->leftNode);
        };
        actualNode.get() = std::move(node);
    }

    template <typename CharType, SizeType FrequencyType>
    template <ForwardConvertible<CharType> FreqRange>
    HuffmanTree<CharType, FrequencyType>::CountedArray
        HuffmanTree<CharType, FrequencyType>::generateFrequencyArray(const FreqRange& frequency)
    {
        CountedArray counted{};
        for (const auto& length : frequency)
            ++counted[length];
        counted[0] = 0;
        return counted;
    }

    template <typename CharType, SizeType FrequencyType>
    template <ForwardConvertible<FrequencyType> CountedRange>
    HuffmanTree<CharType, FrequencyType>::CountedArray
        HuffmanTree<CharType, FrequencyType>::generateSmallestCodes(const CountedRange& counted, FrequencyType min, FrequencyType max)
    {
        CountedArray smallestCodes{};
        FrequencyType code = 0;
        for (auto bits : std::views::iota(min, max + 1))
            smallestCodes[bits] = code = (code + counted[bits - 1]) << 1;
        return smallestCodes;
    }

    template <typename CharType, SizeType FrequencyType>
    HuffmanTree<CharType, FrequencyType>
        HuffmanTree<CharType, FrequencyType>::createDeflateDecoder(void) requires (sizeof(CharType) * CHAR_BIT > 8)
    {
        FrequencyArray frequency;
        frequency.reserve(288);
        std::ranges::fill_n(std::back_inserter(frequency), 144, 8);
        std::ranges::fill_n(std::back_inserter(frequency), 112, 9);
        std::ranges::fill_n(std::back_inserter(frequency), 24, 7);
        std::ranges::fill_n(std::back_inserter(frequency), 8, 8);
        return HuffmanTree<CharType, FrequencyType>{frequency};
    }

    template <typename CharType, SizeType FrequencyType>
    void HuffmanTree<CharType, FrequencyType>::walkThrough(NodeCRef node, CodesMap& map, std::string code) const {
        while (node.get() && node.get()->isInner) {
            walkThrough(std::cref(node.get()->leftNode), map, code + "0");
            node = std::cref(node.get()->rightNode);
            code += "1";
        }
        if (node.get())
            map[node.get()->character] = code;
    }

    template <typename CharType, SizeType FrequencyType>
        HuffmanTree<CharType, FrequencyType>::CodesMap
    HuffmanTree<CharType, FrequencyType>::getCoding(void) const noexcept {
        CodesMap codesMap;
        walkThrough(root, codesMap, "");
        return codesMap;
    }

    template <typename CharType, SizeType FrequencyType>
        HuffmanTree<CharType, FrequencyType>::DecodingMap
    HuffmanTree<CharType, FrequencyType>::getDecoder(void) const noexcept {
        DecodingMap codesMap;
        walkThrough(root, codesMap, 0);
        return codesMap;
    }

    template <typename CharType, SizeType FrequencyType>
    void HuffmanTree<CharType, FrequencyType>::walkThrough(NodeCRef node, DecodingMap& map, FrequencyType code) const {
        while (node.get() && node.get()->isInner) {
            walkThrough(std::cref(node.get()->leftNode), map, code << 1);
            node = std::cref(node.get()->rightNode);
            code = (code << 1) + 1;
        }
        if (node.get())
            map[node.get()->character] = code;
    }

    template <typename CharType, SizeType FrequencyType>
    template <BitIterator Iter>
    CharType HuffmanTree<CharType, FrequencyType>::Decoder::decodeToken(Iter& iterator) const {
        std::reference_wrapper<const NodePtr> node{tree.root};
        while (node.get() && node.get()->isInner)
            node = std::cref(*iterator++ ? node.get()->rightNode : node.get()->leftNode);
        if (!node.get())
            throw HuffmanTreeDecoderException{};
        return node.get()->character;
    }


}
