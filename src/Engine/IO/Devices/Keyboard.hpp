#pragma once

#include <inttypes.h>

namespace mpgl {

    enum class Key : uint16 {
        Undefined       = 0xFFFF,
        Space           = 0x0020,
        Apostrophe      = 0x0027,
        Comma           = 0x002C,
        Minus           = 0x002D,
        Period          = 0x002E,
        Slash           = 0x002F,
        Number0         = 0x0030,
        Number1         = 0x0031,
        Number2         = 0x0032,
        Number3         = 0x0033,
        Number4         = 0x0034,
        Number5         = 0x0035,
        Number6         = 0x0036,
        Number7         = 0x0037,
        Number8         = 0x0038,
        Number9         = 0x0039,
        Semicolon       = 0x003B,
        Equal           = 0x003D,
        A               = 0x0041,
        B               = 0x0042,
        C               = 0x0043,
        D               = 0x0044,
        E               = 0x0045,
        F               = 0x0046,
        G               = 0x0047,
        H               = 0x0048,
        I               = 0x0049,
        J               = 0x004A,
        K               = 0x004B,
        L               = 0x004C,
        M               = 0x004D,
        N               = 0x004E,
        O               = 0x004F,
        P               = 0x0050,
        Q               = 0x0051,
        R               = 0x0052,
        S               = 0x0053,
        T               = 0x0054,
        U               = 0x0055,
        V               = 0x0056,
        W               = 0x0057,
        X               = 0x0058,
        Y               = 0x0059,
        Z               = 0x005A,
        LeftBracket     = 0x005B,
        BackSlash       = 0x005C,
        RightSlash      = 0x005D,
        GraveAccent     = 0x0060,
        World1          = 0x00A1,
        World2          = 0x00A2,
        Escape          = 0x0100,
        Enter           = 0x0101,
        Tab             = 0x0102,
        BackSpace       = 0x0103,
        Insert          = 0x0104,
        Delete          = 0x0105,
        Right           = 0x0106,
        Left            = 0x0107,
        Down            = 0x0108,
        Up              = 0x0109,
        PageUp          = 0x010A,
        PageDown        = 0x010B,
        Home            = 0x010C,
        End             = 0x010D,
        CapsLock        = 0x0118,
        ScrollLock      = 0x0119,
        NumLock         = 0x011A,
        PrintScreen     = 0x011B,
        Pause           = 0x011C,
        F1              = 0x0122,
        F2              = 0x0123,
        F3              = 0x0124,
        F4              = 0x0125,
        F5              = 0x0126,
        F6              = 0x0127,
        F7              = 0x0128,
        F8              = 0x0129,
        F9              = 0x012A,
        F10             = 0x012B,
        F11             = 0x012C,
        F12             = 0x012D,
        F13             = 0x012E,
        F14             = 0x012F,
        F15             = 0x0130,
        F16             = 0x0131,
        F17             = 0x0132,
        F18             = 0x0133,
        F19             = 0x0134,
        F20             = 0x0135,
        F21             = 0x0136,
        F22             = 0x0137,
        F23             = 0x0138,
        F24             = 0x0139,
        F25             = 0x013A,
        Numpad0         = 0x0140,
        Numpad1         = 0x0141,
        Numpad2         = 0x0142,
        Numpad3         = 0x0143,
        Numpad4         = 0x0144,
        Numpad5         = 0x0145,
        Numpad6         = 0x0146,
        Numpad7         = 0x0147,
        Numpad8         = 0x0148,
        Numpad9         = 0x0149,
        NumpadDecimal   = 0x014A,
        NumpadDivide    = 0x014B,
        NumpadMultiply  = 0x014C,
        NumpadSubtract  = 0x014D,
        NumpadAdd       = 0x014E,
        NumpadEnter     = 0x014F,
        NumpadEqual     = 0x0150,
        LeftShift       = 0x0154,
        LeftControl     = 0x0155,
        LeftAlt         = 0x0156,
        LeftSuper       = 0x0157,
        RightShift      = 0x0158,
        RightControl    = 0x0159,
        RightAlt        = 0x015A,
        RightSuper      = 0x015B,
        Menu            = 0x015C
    };

}
