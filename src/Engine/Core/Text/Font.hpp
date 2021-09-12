#pragma once

#include "Subfont.hpp"

namespace ge {

    class Font {
    public:
        enum class Type : uint8_t {
            Regular = 0x01,
            Italic = 0x02,
            Bold = 0x04,
            BoldItalic = 0x08
        };

        explicit Font(std::string const& fontName,
                      std::string fontDirectory = {});

        uint8_t getMask(void) const noexcept { return mask; }

        Subfont& operator() (Type const& type);
    private:
        typedef std::map<Type, Subfont>     SubfontsMap;
        typedef std::vector<std::string>    Files;
        typedef std::map<Type const, std::string const>
                                            TypeMap;

        SubfontsMap                         subfonts;
        std::string                         fontName;
        uint8_t                             mask;

        void findSubfont(Files& files, Files& signatures,
            std::string const& type, Type const& flag);

        void addSubfont(std::size_t position, Files& files, Files& signatures,
            Type const& flag);

        static const TypeMap                typeVector;
    };

    uint8_t operator& (Font::Type const& left, uint8_t right) noexcept;

    uint8_t operator& (uint8_t left, Font::Type const& right) noexcept;

}
