#pragma once

#include <inttypes.h>

namespace ge {

    enum class MouseButton : uint8 {
        Left            = 0x00,
        Right           = 0x01,
        Middle          = 0x02,
        Button4         = 0x03,
        Button5         = 0x04,
        Button6         = 0x05,
        Button7         = 0x06,
        Button8         = 0x07
    };

}
