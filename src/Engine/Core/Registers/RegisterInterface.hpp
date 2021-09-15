#pragma once

namespace ge {

    struct RegisterInterface {

        virtual void onEvent(void) = 0;

        virtual ~RegisterInterface(void) noexcept = default;
    };

}
