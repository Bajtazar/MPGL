#pragma once

#include <string>

namespace ge {

    struct TextWriteEvent {
        explicit TextWriteEvent(void) noexcept = default;

        virtual void onTextWirteEvent(std::string const& unicodeString) = 0;

        virtual ~TextWriteEvent(void) = default;
    };

}
