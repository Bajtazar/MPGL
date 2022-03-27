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

namespace mpgl {

    /**
     * Compressed and decompresses data using huffman tree
     *
     * @tparam CharType the token type
     * @tparam FrequencyType the token frequency type
     */
    template <typename CharType = uint8,
        SizeType FrequencyType = std::size_t>
    class HuffmanTree {
    public:
        typedef std::unordered_map<CharType,
            FrequencyType>                          DecodingMap;
        typedef std::map<CharType, FrequencyType>   CharactersMap;
        typedef std::map<CharType, std::string>     CodesMap;
        typedef std::vector<CharType>               FrequencyArray;

        /**
         * Construct a new Huffman Tree object from the given
         * characters and the number of tokens with the length
         * of counted range index
         *
         * @tparam CountedRange the size range type
         * @tparam CharRange the characters range type
         * @param counted the constant reference to the
         * size range object
         * @param characters the constant reference to the
         * characters range object
         */
        template <RandomAccessConvertible<CharType> CountedRange,
            ForwardConvertible<CharType> CharRange>
        explicit HuffmanTree(
            CountedRange const& counted,
            CharRange const& characters);

        /**
         * Construct a new Huffman Tree object from the given
         * token lengths range
         *
         * @tparam LengthRange the length range type
         * @param lengths the constant reference to the range
         * object containing token sizes
         */
        template <ForwardConvertible<CharType> LengthRange>
        explicit HuffmanTree(LengthRange const& lengths);

        /**
         * Construct a new Huffman Tree object from a map
         * containing tokens and number of their occurrences
         *
         * @throw HuffmanTreeEmptyMapException if the given
         * map is empty
         * @param data the constant reference to the map containing
         * tokens and the number of their occurrences
         */
        explicit HuffmanTree(CharactersMap const& data);

        HuffmanTree(const HuffmanTree& tree) noexcept = delete;

        /**
         * Construct a new Huffman Tree object from the given
         * rvalue reference to the huffman tree object
         *
         * @param tree the rvalue reference to the huffman tree
         * object
         */
        HuffmanTree(HuffmanTree&& tree)
            : root{std::move(tree.root)} {}

        HuffmanTree& operator= (HuffmanTree const& tree) = delete;

        /**
         * Assigns the given rvalue reference to the huffman tree
         * object to this object
         *
         * @param tree the rvalue reference to the huffman tree
         * object
         * @return the reference to this object
         */
        HuffmanTree& operator= (HuffmanTree&& tree)
            { root = std::move(tree.root); return *this; }

        /**
         * Returns coding in a form that can be easy printed
         * for human readability
         *
         * @return the map containing tokens and their codes
         * saved in string as binary numbers
         */
        [[nodiscard]] CodesMap getCoding(void) const noexcept;

        /**
         * Returns the decoding map. This map can also be used
         * to code
         *
         * @return the map containing tokens and their codes
         */
        [[nodiscard]] DecodingMap getDecoder(void) const noexcept;

        /**
         * Returns a decoder used by the fixed DEFLATE coding
         */
        [[nodiscard]] static HuffmanTree createDeflateDecoder(
            void) requires (sizeof(CharType) * CHAR_BIT > 8);

        /**
         * Decodes a huffman coding using the given huffman tree
         */
        class Decoder {
        public:
            /**
             * Construct a new Decoder object from the given
             * rvalue reference to the huffman tree
             *
             * @param tree the rvalue reference to the huffman tree
             */
            Decoder(HuffmanTree&& tree)
                : tree{std::move(tree)} {}

            /**
             * Construct a new Decoder object with default
             * fixed DEFLATE tree
             */
            Decoder(void)
                : tree{createDeflateDecoder()} {}

            /**
             * Decodes the symbol under the given iterator
             * and returns decoded token
             *
             * @throw HuffmanTreeDecoderException when given
             * symbol is unknown
             * @tparam Iter the iterator type
             * @param iterator the reference to the iterator
             * @return the decoded token
             */
            template <BitIterator Iter>
            [[nodiscard]] CharType operator()(Iter& iterator) const;
        private:
            HuffmanTree                             tree;
        };

        ~HuffmanTree(void) noexcept = default;
    private:
        typedef std::pair<CharType, FrequencyType>  FreqPair;
        /**
         * Represents the node in the huffman tree
         */
        struct Node {
            typedef std::unique_ptr<Node>           NodePtr;

            NodePtr                                 leftNode;
            NodePtr                                 rightNode;
            FrequencyType                           frequency;
            CharType                                character;
            bool                                    notLeaf;

            /**
             * Construct a new Node object from the given
             * character, frequency and information whether
             * node is a leaf
             *
             * @param character the node's character
             * @param frequency the node's frequency
             * @param notLeaf if node is a leaf
             */
            explicit Node(
                CharType character = 0,
                FrequencyType frequency = 0,
                bool notLeaf = false)
                    : leftNode{nullptr}, rightNode{nullptr},
                    frequency{frequency}, character{character},
                    notLeaf{notLeaf} {}

            /**
             * Construct a new Node object from the given
             * frequency pair
             *
             * @param frequencyPair the character type and its
             * frequency
             */
            explicit Node(FreqPair const& frequencyPair)
                : leftNode{nullptr}, rightNode{nullptr},
                frequency{frequencyPair.second},
                character{frequencyPair.first},
                notLeaf{false} {}

            /**
             * Returns whether one node has higher frequency
             * than another
             *
             * @param left the constant reference to the left node
             * @param right the constant reference to the right node
             * @return if left node has higher frequency
             */
            friend bool operator> (
                Node const& left,
                Node const& right) noexcept
                    { return left.frequency > right.frequency; }

            /**
             * Returns whether one node has lower frequency
             * than another
             *
             * @param left the constant reference to the left node
             * @param right the constant reference to the right node
             * @return if left node has lower frequency
             */
            friend bool operator< (
                Node const& left,
                Node const& right) noexcept
                    { return left.frequency < right.frequency; }
        };

        /**
         * Returns the bit size of the type
         *
         * @tparam Tp the type
         * @return the bit size of the type
         */
        template <typename Tp>
        static constexpr std::size_t bitsOf(void) noexcept
            { return sizeof(Tp) * CHAR_BIT; }

        typedef typename Node::NodePtr              NodePtr;
        typedef std::reference_wrapper<
            NodePtr const>                          NodeCRef;

        typedef std::array<FrequencyType,
            bitsOf<CharType>()>                     CountedArray;
        typedef std::vector<
            std::pair<uint8, uint8>>                CharacterLengthArray;

        /**
         * Walks through the tree and saves the tokens codes
         *
         * @param node the constant reference wrapper to the node
         * @param map the codes map
         * @param code the current code
         */
        void walkThrough(
            NodeCRef node,
            CodesMap& map,
            std::string code) const;

        /**
         * Walks through the tree and saves the tokens codes
         *
         * @param node the constant reference wrapper to the node
         * @param map the decoding map
         * @param code the current code
         */
        void walkThrough(
            NodeCRef node,
            DecodingMap& map,
            FrequencyType code) const;

        /**
         * Adds a new node to the tree
         *
         * @param length the length of the code
         * @param character the token character
         * @param code the token code
         */
        void addNode(
            CharType length,
            CharType character,
            FrequencyType code);

        /**
         * Emplaces nodes from the given headp in the tree
         *
         * @param heap the reference to the minimum headp
         * with the nodes
         */
        void emplaceNodes(MinHeap<NodePtr>& heap);

        /**
         * Generates count array for the given lengths range
         *
         * @tparam LengthsRange the lengths range type
         * @param lengths the tokens lengths
         * @return the counted array
         */
        template <ForwardConvertible<CharType> LengthsRange>
        CountedArray generateFrequencyArray(
            LengthsRange const& lengths);

        /**
         * Generates optimal codes for the given counted range
         *
         * @tparam CountedRange the counted range type
         * @param counted the constant reference to the
         * counted array
         * @param min the minimum length of the code
         * @param max the maximum length of the code
         * @return the smallest codes array
         */
        template <ForwardConvertible<FrequencyType> CountedRange>
        CountedArray generateSmallestCodes(
            CountedRange const& counted,
            FrequencyType min,
            FrequencyType max);

        NodePtr                                     root;
    };

    template <typename CharType, SizeType FrequencyType>
    void HuffmanTree<CharType, FrequencyType>::emplaceNodes(
        MinHeap<NodePtr>& heap)
    {
        while (heap.size() != 1) {
            auto left = heap.popBack();
            auto right = heap.popBack();
            auto node = std::make_unique<Node>(0,
                left->frequency + right->frequency, true);
            node->leftNode = std::move(left);
            node->rightNode = std::move(right);
            heap.push(std::move(node));
        }
        root = std::move(heap.popBack());
    }

    template <typename CharType, SizeType FrequencyType>
    HuffmanTree<CharType, FrequencyType>::HuffmanTree(
        CharactersMap const& data)
            : root{nullptr}
    {
        if (!data.size())
            throw HuffmanTreeEmptyMapException{};
        MinHeap<NodePtr> heap;
        for (const auto& frequencyPair : data)
            heap.push(std::make_unique<Node>(frequencyPair));
        emplaceNodes(heap);
    }

    template <typename CharType, SizeType FrequencyType>
    template <ForwardConvertible<CharType> FreqRange>
    HuffmanTree<CharType, FrequencyType>::HuffmanTree(
        FreqRange const& frequency)
            : root{nullptr}
        {
        auto [minFreq, maxFreq] = std::ranges::minmax_element(frequency);
        auto smallestCodes = generateSmallestCodes(
            generateFrequencyArray(frequency), *minFreq, *maxFreq);
        std::ranges::for_each(frequency,
            [&, i = 0](const auto& bits) mutable {
                if (auto iter = i++; bits)
                    addNode(bits, iter, smallestCodes[bits]++);
            });
    }

    template <typename CharType, SizeType FrequencyType>
    template <RandomAccessConvertible<CharType> CountedRange,
        ForwardConvertible<CharType> CharRange>
    HuffmanTree<CharType, FrequencyType>::HuffmanTree(
        CountedRange const& counted,
        CharRange const& characters)
            : root{nullptr}
    {
        auto [minFreq, maxFreq] = findFirstAndLastIf(counted,
            [](const auto& value) -> bool { return value != 0; });
        auto smallestCodes = generateSmallestCodes(counted,
            minFreq - counted.begin(), maxFreq - counted.begin());
        auto iter = characters.begin();
        std::ranges::for_each(counted,
            [&, i = 0](auto code) mutable {
                auto length = i++;
                for (CharType j = 0; j < code; ++j)
                    addNode(length, *iter++, smallestCodes[length]++);
            });
    }

    template <typename CharType, SizeType FrequencyType>
    void HuffmanTree<CharType, FrequencyType>::addNode(
        CharType length,
        CharType character,
        FrequencyType code)
    {
        auto node = std::make_unique<Node>(character, length);
        std::reference_wrapper<NodePtr> actualNode{root};
        std::bitset<bitsOf<FrequencyType>()> bits{code};
        while (length--) {
            if (!actualNode.get())
                actualNode.get() = std::make_unique<Node>(0, 0, true);
            actualNode = std::ref(bits[length] ?
                actualNode.get()->rightNode :
                actualNode.get()->leftNode);
        };
        actualNode.get() = std::move(node);
    }

    template <typename CharType, SizeType FrequencyType>
    template <ForwardConvertible<CharType> FreqRange>
    HuffmanTree<CharType, FrequencyType>::CountedArray
        HuffmanTree<CharType, FrequencyType>::generateFrequencyArray(
            FreqRange const& frequency)
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
        HuffmanTree<CharType, FrequencyType>::generateSmallestCodes(
            CountedRange const& counted,
            FrequencyType min,
            FrequencyType max)
    {
        CountedArray smallestCodes{};
        FrequencyType code = 0;
        for (auto bits : std::views::iota(min, max + 1))
            smallestCodes[bits] = code =
                (code + counted[bits - 1]) << 1;
        return smallestCodes;
    }

    template <typename CharType, SizeType FrequencyType>
    [[nodiscard]] HuffmanTree<CharType, FrequencyType>
        HuffmanTree<CharType, FrequencyType>::createDeflateDecoder(
            void) requires (sizeof(CharType) * CHAR_BIT > 8)
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
    void HuffmanTree<CharType, FrequencyType>::walkThrough(
        NodeCRef node,
        CodesMap& map,
        std::string code) const
    {
        while (node.get() && node.get()->notLeaf) {
            walkThrough(std::cref(node.get()->leftNode),
                map, code + "0");
            node = std::cref(node.get()->rightNode);
            code += "1";
        }
        if (node.get())
            map[node.get()->character] = code;
    }

    template <typename CharType, SizeType FrequencyType>
    [[nodiscard]] HuffmanTree<CharType, FrequencyType>::CodesMap
        HuffmanTree<CharType, FrequencyType>::getCoding(
            void) const noexcept
    {
        CodesMap codesMap;
        walkThrough(root, codesMap, "");
        return codesMap;
    }

    template <typename CharType, SizeType FrequencyType>
    [[nodiscard]] HuffmanTree<CharType, FrequencyType>::DecodingMap
        HuffmanTree<CharType, FrequencyType>::getDecoder(
            void) const noexcept
    {
        DecodingMap codesMap;
        walkThrough(root, codesMap, 0);
        return codesMap;
    }

    template <typename CharType, SizeType FrequencyType>
    void HuffmanTree<CharType, FrequencyType>::walkThrough(
        NodeCRef node,
        DecodingMap& map,
        FrequencyType code) const
    {
        while (node.get() && node.get()->notLeaf) {
            walkThrough(std::cref(node.get()->leftNode),
                map, code << 1);
            node = std::cref(node.get()->rightNode);
            code = (code << 1) + 1;
        }
        if (node.get())
            map[node.get()->character] = code;
    }

    template <typename CharType, SizeType FrequencyType>
    template <BitIterator Iter>
    [[nodiscard]] CharType
        HuffmanTree<CharType, FrequencyType>::Decoder::operator()(
            Iter& iterator) const
    {
        NodeCRef node{tree.root};
        while (node.get() && node.get()->notLeaf)
            node = std::cref(*iterator++ ? node.get()->rightNode
                : node.get()->leftNode);
        if (!node.get())
            throw HuffmanTreeDecoderException{};
        return node.get()->character;
    }

}
