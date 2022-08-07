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
#include <MPGL/Exceptions/SLGZFileCorruptionException.hpp>
#include <MPGL/Core/Shaders/SLGZLoader.hpp>
#include <MPGL/Compression/GZIPDecoder.hpp>
#include <MPGL/IO/Readers.hpp>
#include <MPGL/IO/FileIO.hpp>

namespace mpgl {

    template <security::SecurityPolicy Policy>
    [[nodiscard]] SLGZLoader<Policy>::Iter
        SLGZLoader<Policy>::getIterator(
            BufferIter const& iter,
            [[maybe_unused]] BufferIter const& end) const noexcept
    {
        if constexpr (security::isSecurePolicy<Policy>) {
            return Iter{ iter, end };
        } else {
            return Iter{ iter };
        }
    }

    template <security::SecurityPolicy Policy>
    void SLGZLoader<Policy>::remove(
        Iter const& iterator,
        Buffer& buffer) const noexcept
    {
        if constexpr (security::isSecurePolicy<Policy>) {
            buffer.erase(buffer.begin(), iterator.getIter());
        } else {
            buffer.erase(buffer.begin(), iterator);
        }
    }

    template <security::SecurityPolicy Policy>
    [[nodiscard]] SLGZLoader<Policy>::ShaderMap
        SLGZLoader<Policy>::operator() (
            std::string const& path) const
    {
        std::map<std::string, Buffer> shaderMap;
        if (auto file = FileIO::readFileToVec(path)) {
            auto data = GZIPDecoder<
                Buffer, Policy>(std::move(*file))();
            auto iter = getIterator(data.begin(), data.end());
            auto table = parseHeader(iter);
            remove(iter, data);
            shaderMap = buildMap(table, data);
        } else
            throw SLGZFileCorruptionException{path};
        return shaderMap;
    }

    template <security::SecurityPolicy Policy>
    [[nodiscard]] SLGZLoader<Policy>::Records
        SLGZLoader<Policy>::parseHeader(Iter& iterator) const
    {
        Records records;
        while (uint32 offset = readType<uint32>(iterator)) {
            uint32 shaderLength = readType<uint32>(iterator);
            std::string shaderName;
            while (auto character = readType<char>(iterator))
                shaderName.push_back(character);
            records.emplace_back(offset, shaderLength, shaderName);
        }
        return records;
    }

    template <security::SecurityPolicy Policy>
    [[nodiscard]] SLGZLoader<Policy>::ShaderMap
        SLGZLoader<Policy>::buildMap(
            Records const& records,
            Buffer const& buffer) const
    {
        ShaderMap shaderMap;
        for (auto const& [offset, length, name] : records) {
            auto iter = getIterator(buffer.begin() + offset - OFFSET,
                buffer.end());
            auto sent = getIterator(buffer.begin() + offset - OFFSET
                + length, buffer.end());
            shaderMap.emplace(name, Buffer{iter, sent});
        }
        return shaderMap;
    }

}
