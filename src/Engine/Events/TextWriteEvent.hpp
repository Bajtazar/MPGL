#pragma once

#include <string>

#include "Event.hpp"

namespace mpgl {

    struct TextWriteEvent : public EventBase {
        explicit TextWriteEvent(void) noexcept = default;

        virtual void onTextWrite(
            std::string const& unicodeString) noexcept = 0;

        virtual ~TextWriteEvent(void) = default;
    };

}
