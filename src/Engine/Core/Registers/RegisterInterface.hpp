#pragma once

namespace ge {

    struct RegisterInterface {

        virtual void onEvent(void) noexcept = 0;

        virtual ~RegisterInterface(void) noexcept = default;
    };

}
