#pragma once

#include "Subfont.hpp"

namespace mpgl {

    class Font {
    public:
        enum class Type : uint8 {
            Regular = 0x01,
            Italic = 0x02,
            Bold = 0x04,
            BoldItalic = 0x08
        };

        explicit Font(std::string const& fontName,
                      std::string fontDirectory = {});

        uint8 getMask(void) const noexcept
            { return mask; }

        Subfont& operator() (Type const& type);
    private:
        typedef std::map<Type, Subfont>     SubfontsMap;
        typedef std::vector<std::string>    Files;
        typedef std::map<Type const, std::string const>
                                            TypeMap;

        SubfontsMap                         subfonts;
        std::string                         fontName;
        uint8                             mask;

        void findSubfont(Files& files, Files& signatures,
            std::string const& type, Type const& flag);

        void addSubfont(std::size_t position, Files& files, Files& signatures,
            Type const& flag);

        static const TypeMap                typeVector;
    };

    uint8 operator& (Font::Type const& left, uint8 right) noexcept;

    uint8 operator& (uint8 left, Font::Type const& right) noexcept;

}
