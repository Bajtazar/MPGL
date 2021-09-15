#pragma once

namespace ge {

    struct RegisterInterface {
        explicit RegisterInterface(void) noexcept = default;

        virtual void onEvent(void) = 0;

        virtual ~RegisterInterface(void) noexcept = default;
    };

}
