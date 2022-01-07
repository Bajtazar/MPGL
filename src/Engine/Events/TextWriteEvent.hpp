#pragma once

#include <string>

#include "Event.hpp"

namespace ge {

    struct TextWriteEvent : public EventBase {
        explicit TextWriteEvent(void) noexcept = default;

        virtual void onTextWirteEvent(
            std::string const& unicodeString) noexcept = 0;

        virtual ~TextWriteEvent(void) = default;
    };

}
