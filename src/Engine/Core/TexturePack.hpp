#pragma once

#include "../Exceptions/ExecusionUnknownPolicyException.hpp"
#include "Texture.hpp"

#include <unordered_map>
#include <algorithm>
#include <vector>

namespace ge {

    template <Allocator Alloc = std::allocator<uint32_t>>
    class TexturePack {
    public:
        typedef std::string                         Key;
        typedef Texture<Alloc>                      Value;
        typedef std::pair<Key const, Value>         ValuePair;

        template <std::ranges::random_access_range Range>
            requires SameRangeType<Range, ValuePair>
        explicit TexturePack(Range&& range,
            Value const& defaultTexture = Value::defaultTexture(),
            Alloc const& alloc = {});

        Value const& operator[] (Key const& key) const noexcept;
    private:
        typedef typename std::allocator_traits<
            Alloc>::rebind_alloc<ValuePair>         VPAlloc;
        typedef std::hash<Key>                      Hash;
        typedef std::equal_to<Key>                  Pred;
        typedef std::unordered_map<Key, Value,
            Hash, Pred, VPAlloc>                    TextureMap;

        TextureMap                                  textureMap;
        Value const                                 defaultValue;
    };

    template <Allocator Alloc>
    template <std::ranges::random_access_range Range>
        requires SameRangeType<Range,
            typename TexturePack<Alloc>::ValuePair>
    TexturePack<Alloc>::TexturePack(Range&& range,
        Value const& defaultTexture, Alloc const& alloc)
            : defaultValue{defaultTexture},
                textureMap{range.begin(), range.end(),
                0, Hash{}, Pred{}, alloc} {}

    template <Allocator Alloc>
    TexturePack<Alloc>::Value const&
        TexturePack<Alloc>::operator[] (Key const& key) const noexcept
    {
        if (auto iter = textureMap.find(key); iter != textureMap.end())
            return iter->second;
        return defaultValue;
    }

}
