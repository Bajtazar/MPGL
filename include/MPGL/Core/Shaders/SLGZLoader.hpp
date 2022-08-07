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

#include <MPGL/Utility/Tokens/Security.hpp>
#include <MPGL/Iterators/SafeIterator.hpp>
#include <MPGL/Traits/Types.hpp>

#include <string>
#include <vector>
#include <tuple>
#include <list>
#include <map>

namespace mpgl {

    /**
     * The SLGZ file format parser
     */
    template <security::SecurityPolicy Policy = Secured>
    class SLGZLoader {
    public:
        using Buffer = std::vector<char>;
        using ShaderMap = std::map<std::string, Buffer>;

        /**
         * Constructs a new SLGZLoader object
         *
         * @param token the security policy token
         */
        explicit SLGZLoader(Policy token = {}) noexcept {}

        /**
         * Parses the SLGZ file format and returns a map containing
         * the shader names and their codes
         *
         * @throws SLGZFileCorruptionException if file does not
         * exist or is corrupted
         * @param path a path to the SLGZ file
         * @return the map containing
         * the shader names and their codes
         */
        [[nodiscard]] ShaderMap operator() (
            std::string const& path) const;
    private:
        using Record = std::tuple<uint32, uint32, std::string>;
        using Records = std::list<Record>;
        using BufferIter = typename Buffer::const_iterator;
        using Iter = std::conditional_t<security::isSecurePolicy<
            Policy>, SafeIterator<BufferIter>, BufferIter>;

        /**
         * Returns an iterator to the buffer
         *
         * @param iter the iterator to the begining of the range
         * @param end the iterator to the end of the range
         * @return an iterator to the buffer
         */
        [[nodiscard]] Iter getIterator(
            BufferIter const& iter,
            [[maybe_unused]] BufferIter const& end) const noexcept;

        /**
         * Removes a subrange from the buffer. The begining of
         * the buffer is also the begining of the subrange
         *
         * @param iterator the iterator to the end of the subrange
         * @param buffer the reference to the buffer object
         */
        void remove(
            Iter const& iterator,
            Buffer& buffer) const noexcept;

        /**
         * Parses the SLGZ's file header
         *
         * @param iterator the reference to the file's iterator
         * @return the records contained within the header
         */
        [[nodiscard]] Records parseHeader(Iter& iterator) const;

        /**
         * Builds the shader map according to the header records
         * and the decompressed shaders
         *
         * @param records the constant reference to the records
         * @param buffer the constant reference to the decompressed
         * shaders
         * @return the shader map
         */
        [[nodiscard]] ShaderMap buildMap(
            Records const& records,
            Buffer const& buffer) const;

        static constexpr size_t const OFFSET = 1ul;
    };

}

#include <MPGL/Core/Shaders/SLGZLoader.tpp>
