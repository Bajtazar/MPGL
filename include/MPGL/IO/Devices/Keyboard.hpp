/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022
 *      Grzegorz Czarnecki (grzegorz.czarnecki.2021@gmail.com)
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held liable
 *  for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose, including commercial applications, and to alter it and
 *  redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment in the
 *  product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *  distribution
 */
#pragma once

#include <MPGL/Traits/Types.hpp>

namespace mpgl {

    /**
     * Describes the keyboard keys
     */
    enum class Key : int16 {
        Space           = 0x00,
        Apostrophe      = 0x01,
        Comma           = 0x02,
        Minus           = 0x03,
        Period          = 0x04,
        Slash           = 0x05,
        Number0         = 0x06,
        Number1         = 0x07,
        Number2         = 0x08,
        Number3         = 0x09,
        Number4         = 0x0A,
        Number5         = 0x0B,
        Number6         = 0x0C,
        Number7         = 0x0D,
        Number8         = 0x0E,
        Number9         = 0x0F,
        Semicolon       = 0x10,
        Equal           = 0x11,
        A               = 0x12,
        B               = 0x13,
        C               = 0x14,
        D               = 0x15,
        E               = 0x16,
        F               = 0x17,
        G               = 0x18,
        H               = 0x19,
        I               = 0x1A,
        J               = 0x1B,
        K               = 0x1C,
        L               = 0x1D,
        M               = 0x1E,
        N               = 0x1F,
        O               = 0x20,
        P               = 0x21,
        Q               = 0x22,
        R               = 0x23,
        S               = 0x24,
        T               = 0x25,
        U               = 0x26,
        V               = 0x27,
        W               = 0x28,
        X               = 0x29,
        Y               = 0x2A,
        Z               = 0x2B,
        LeftBracket     = 0x2C,
        BackSlash       = 0x2D,
        RightBracket    = 0x2E,
        GraveAccent     = 0x2F,
        World1          = 0x30,
        World2          = 0x31,
        Escape          = 0x32,
        Enter           = 0x33,
        Tab             = 0x34,
        BackSpace       = 0x35,
        Insert          = 0x36,
        Delete          = 0x37,
        Right           = 0x38,
        Left            = 0x39,
        Down            = 0x3A,
        Up              = 0x3B,
        PageUp          = 0x3C,
        PageDown        = 0x3D,
        Home            = 0x3E,
        End             = 0x3F,
        CapsLock        = 0x40,
        ScrollLock      = 0x41,
        NumLock         = 0x42,
        PrintScreen     = 0x43,
        Pause           = 0x44,
        F1              = 0x45,
        F2              = 0x46,
        F3              = 0x47,
        F4              = 0x48,
        F5              = 0x49,
        F6              = 0x4A,
        F7              = 0x4B,
        F8              = 0x4C,
        F9              = 0x4D,
        F10             = 0x4E,
        F11             = 0x4F,
        F12             = 0x50,
        F13             = 0x51,
        F14             = 0x52,
        F15             = 0x53,
        F16             = 0x54,
        F17             = 0x55,
        F18             = 0x56,
        F19             = 0x57,
        F20             = 0x58,
        F21             = 0x59,
        F22             = 0x5A,
        F23             = 0x5B,
        F24             = 0x5C,
        F25             = 0x5D,
        Numpad0         = 0x5E,
        Numpad1         = 0x5F,
        Numpad2         = 0x60,
        Numpad3         = 0x61,
        Numpad4         = 0x62,
        Numpad5         = 0x63,
        Numpad6         = 0x64,
        Numpad7         = 0x65,
        Numpad8         = 0x66,
        Numpad9         = 0x67,
        NumpadDecimal   = 0x68,
        NumpadDivide    = 0x69,
        NumpadMultiply  = 0x6A,
        NumpadSubtract  = 0x6B,
        NumpadAdd       = 0x6C,
        NumpadEnter     = 0x6D,
        NumpadEqual     = 0x6E,
        LeftShift       = 0x6F,
        LeftControl     = 0x70,
        LeftAlt         = 0x71,
        LeftSuper       = 0x72,
        RightShift      = 0x73,
        RightControl    = 0x74,
        RightAlt        = 0x75,
        RightSuper      = 0x76,
        Menu            = 0x77,
        Unknown         = 0xFF
    };

}
