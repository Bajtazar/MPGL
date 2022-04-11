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

namespace mpgl {

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
        HuffmanTree<CharType, FrequencyType>::createDeflateTree(
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
    template <BitInputIterator Iter>
    [[nodiscard]] CharType
        HuffmanTree<CharType, FrequencyType>::Decoder::operator()(
            Iter& iterator) const
    {
        NodeCRef node{tree.root};
        while (node.get() && node.get()->notLeaf)
            node = std::cref(*iterator++ ? node.get()->rightNode
                : node.get()->leftNode);
        if (!node.get())
            throw HuffmanTreeUnknownToken{};
        return node.get()->character;
    }

    template <typename CharType, SizeType FrequencyType>
    HuffmanTree<CharType, FrequencyType>::Encoder::Encoder(
        HuffmanTree const& tree)
    {
        walkThrough(std::cref(tree.root), tokens, 0, 0);
    }

    template <typename CharType, SizeType FrequencyType>
    HuffmanTree<CharType, FrequencyType>::Encoder::Encoder(void) {
        auto tree = createDeflateTree();
        walkThrough(std::cref(tree.root), tokens, 0, 0);
    }

    template <typename CharType, SizeType FrequencyType>
    void HuffmanTree<CharType, FrequencyType>::Encoder::walkThrough(
        NodeCRef node,
        TokenMap& tokens,
        FrequencyType code,
        CharType depth)
    {
        while (node.get() && node.get()->notLeaf) {
            walkThrough(std::cref(node.get()->leftNode),
                tokens, code << 1, depth + 1);
            node = std::cref(node.get()->rightNode);
            code = (code << 1) + 1;
            depth += 1;
        }
        if (node.get())
            tokens.emplace(node.get()->character,
                TokenData{code, depth});
    }

    template <typename CharType, SizeType FrequencyType>
    template <BitOutputIterator Iter>
    void
        HuffmanTree<CharType, FrequencyType>::Encoder::operator()(
            Iter& iterator,
            CharType token) const
    {
        auto iter = tokens.find(token);
        if (iter == tokens.end())
            throw HuffmanTreeUnknownToken{};
        auto const& [code, length] = iter->second;
        for (CharType i = 0; i < length; ++i, ++iterator)
            *iterator = bool((code >> (length - 1 - i)) & 1);
    }

}
