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

#include <MPGL/Exceptions/HuffmanTree/HuffmanTreeEmptyMapException.hpp>
#include <MPGL/Exceptions/HuffmanTree/HuffmanTreeUnknownToken.hpp>
#include <MPGL/Collections/PriorityQueue.hpp>
#include <MPGL/Traits/Concepts.hpp>
#include <MPGL/Utility/Ranges.hpp>
#include <MPGL/IO/Readers.hpp>

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
    private:
        struct Node;

        typedef typename std::unique_ptr<Node>      NodePtr;
        typedef std::reference_wrapper<
            NodePtr const>                          NodeCRef;
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
         * Returns a tree used by the fixed DEFLATE coding
         */
        [[nodiscard]] static HuffmanTree createDeflateTree(
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
                : tree{createDeflateTree()} {}

            /**
             * Decodes the symbol under the given iterator
             * and returns decoded token
             *
             * @throw HuffmanTreeUnknownToken when given
             * symbol is unknown
             * @tparam Iter the iterator type
             * @param iterator the reference to the iterator
             * @return the decoded token
             */
            template <BitInputIterator Iter>
            [[nodiscard]] CharType operator()(Iter& iterator) const;
        private:
            HuffmanTree                             tree;
        };

        /**
         * Encodes a huffman coding using the given huffman tree
         */
        class Encoder {
        public:
            /**
             * Construct a new Encoder object from the given
             * huffman tree
             *
             * @param tree the constant reference to the huffman
             * tree
             */
            Encoder(HuffmanTree const& tree);

            /**
             * Construct a new Encoder object with default
             * fixed DEFLATE tree
             */
            Encoder(void);

            /**
             * Encodes the token and saves it in the given
             * iterator
             *
             * @throw HuffmanTreeUnknownToken when given
             * symbol is unknown
             * @tparam Iter the iterator type
             * @param iterator the reference to the iterator
             * @param token the encoded token
             */
            template <BitOutputIterator Iter>
            void operator()(
                Iter& iter,
                CharType token) const;
        private:
            typedef std::pair<
                FrequencyType, CharType>            TokenData;
            typedef std::unordered_map<
                CharType, TokenData>                TokenMap;

            TokenMap                                tokens;

            /**
             * Walks through the tree and saves the tokens codes
             * and lengths
             *
             * @param node the constant reference wrapper to the node
             * @param tokens the reference to the tokens map
             * @param code the current code
             * @param depth the current depth of the search
             */
            static void walkThrough(
                NodeCRef node,
                TokenMap& tokens,
                FrequencyType code,
                CharType depth);
        };

        /**
         * Destroys the Huffman Tree object
         */
        ~HuffmanTree(void) noexcept = default;
    private:
        typedef std::pair<CharType, FrequencyType>  FreqPair;
        /**
         * Represents the node in the huffman tree
         */
        struct Node {
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
             * Returns the result of the starship operator on the
             * nodes frequencies
             *
             * @param left the constant reference to the left node
             * @param right the constant reference to the right node
             * @return the result of the starship operator on the
             * nodes frequencies
             */
            friend auto operator<=> (
                Node const& left,
                Node const& right) noexcept
                    { return left.frequency <=> right.frequency; }

            /**
             * Returns whether nodes has the same frequency
             *
             * @param left the constant reference to the left node
             * @param right the constant reference to the right node
             * @return if nodes has the same frequency
             */
            friend bool operator== (
                Node const& left,
                Node const& right) noexcept
                    { return left.frequency == right.frequency; }
        };

        /**
         * Compares the playloads of the node pointers
         */
        struct NodePtrComparator {

            /**
             * Compares two node pointers payload with each other
             *
             * @param left the constant reference to the left pointer
             * @param right the constant reference to the right pointer
             * @return if the left payload is bigger than the right one
             */
            [[nodiscard]] bool operator() (
                NodePtr const& left,
                NodePtr const& right) const noexcept
                    { return (*left) > (*right); }

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

        typedef std::array<FrequencyType,
            bitsOf<CharType>()>                     CountedArray;
        typedef std::vector<
            std::pair<uint8, uint8>>                CharacterLengthArray;
        typedef PriorityQueue<NodePtr,
            NodePtrComparator>                      MinQueue;

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
        void emplaceNodes(MinQueue& heap);

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

}

#include <MPGL/Compression/HuffmanTree.tpp>
