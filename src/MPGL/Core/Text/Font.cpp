/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
#include <MPGL/Exceptions/FontNoRegularException.hpp>

#include <MPGL/Utility/StringAlgorithm.hpp>
#include <MPGL/Core/Text/Font.hpp>
#include <MPGL/IO/FileIO.hpp>

#include <algorithm>

namespace mpgl {

    Font::TypeMap const Font::typeVector {
        {Type::Regular, "regular"},
        {Type::BoldItalic, "bolditalic"},
        {Type::Italic, "italic"},
        {Type::Bold, "bold"}
    };

    Font::Container::Container(std::string const& fontName)
        : fontName{toLower(fontName)}, mask{0} {}

    Font::Font(
        std::string const& fontName,
        std::string fontDirectory)
            : pointer{new Container{fontName}}
    {
        if (!fontDirectory.size())
            fontDirectory = fontName;
        Files files = FileIO::getRecursiveDirFiles(fontDirectory);
        Files signatures;
        signatures.reserve(files.size());
        std::ranges::transform(files, std::back_inserter(signatures),
            [](auto const& string) { return toLower(string); });
        std::ranges::for_each(typeVector, [&](const auto& pair) {
            findSubfont(files, signatures, pair.second, pair.first); });
        if (!pointer->subfonts.contains(Type::Regular))
            throw FontNoRegularException{fontName};
    }

    void Font::findSubfont(
        Files& files,
        Files& signatures,
        std::string const& type,
        Type const& flag)
    {
        for (std::size_t i = files.size() - 1; i < files.size(); --i)
            if (std::string::npos != signatures[i].find(pointer->fontName))
                if (std::string::npos != signatures[i].find_last_of(type))
                    return addSubfont(i, files, signatures, flag);
    }

    void Font::addSubfont(
        std::size_t position,
        Files& files,
        Files& signatures,
        Type const& flag)
    {
        pointer->subfonts.emplace(flag, Subfont{files[position]});
        files.erase(files.cbegin() + position);
        signatures.erase(signatures.cbegin() + position);
        pointer->mask += static_cast<uint8>(flag);
    }

    Subfont& Font::operator() (Type const& type) {
        if (type & pointer->mask)
            return pointer->subfonts.at(type);
        return pointer->subfonts.at(Type::Regular);
    }

    Subfont const& Font::operator() (Type const& type) const {
        if (type & pointer->mask)
            return pointer->subfonts.at(type);
        return pointer->subfonts.at(Type::Regular);
    }

    uint8 operator& (Font::Type const& left, uint8 right) noexcept {
        return static_cast<uint8>(left) & right;
    }

    uint8 operator& (uint8 left, Font::Type const& right) noexcept {
        return left & static_cast<uint8>(right);
    }

}
