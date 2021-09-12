#include "Font.hpp"
#include "../../IO/FileIO.hpp"
#include "../../IO/Logger.hpp"
#include "../../Utility/StringAlgorithm.hpp"
#include "../../Exceptions/FontNoRegularException.hpp"

#include <algorithm>

namespace ge {

    Font::TypeMap const Font::typeVector {
        {Type::Regular, "regular"},
        {Type::BoldItalic, "bolditalic"},
        {Type::Italic, "italic"},
        {Type::Bold, "bold"}
    };

    Font::Font(std::string const& fontName,
        std::string fontDirectory) : fontName{toLower(fontName)}, mask{0}
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
        if (!subfonts.contains(Type::Regular))
            throw FontNoRegualrException{fontName};
    }

    void Font::findSubfont(Files& files, Files& signatures,
        std::string const& type, Type const& flag)
    {
        for (std::size_t i = files.size() - 1; i < files.size(); --i)
            if (std::string::npos != signatures[i].find(fontName))
                if (std::string::npos != signatures[i].find_last_of(type))
                    return addSubfont(i, files, signatures, flag);
    }

    void Font::addSubfont(std::size_t position,
        Files& files, Files& signatures, Type const& flag)
    {
        subfonts.emplace(flag, Subfont{files[position]});
        files.erase(files.cbegin() + position);
        signatures.erase(signatures.cbegin() + position);
        mask += static_cast<uint8_t>(flag);
    }

    Subfont& Font::operator() (Type const& type) {
        if (type & mask)
            return subfonts.at(type);
        Logger::logInvalidFont(fontName, typeVector.at(type));
        return subfonts.at(Type::Regular);
    }

    uint8_t operator& (Font::Type const& left, uint8_t right) noexcept {
        return static_cast<uint8_t>(left) & right;
    }

    uint8_t operator& (uint8_t left, Font::Type const& right) noexcept {
        return left & static_cast<uint8_t>(right);
    }

}
