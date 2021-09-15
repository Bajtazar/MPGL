#pragma once

#include <string>

namespace ge {

    struct OnTextWrittenEvent {
        explicit OnTextWrittenEvent(void) noexcept = default;

        virtual void onTextWittenEvent(std::string const& unicodeString) = 0;

        virtual ~OnTextWrittenEvent(void) = default;
    };

}
