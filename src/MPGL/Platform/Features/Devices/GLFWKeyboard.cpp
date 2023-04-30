/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
#include <MPGL/Platform/Features/Devices/GLFWKeyboard.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace mpgl::platform {

    [[nodiscard]] KeyboardKey decodeGLFWKeyboardKey(
        int32 glfwKey) noexcept
    {
        switch (glfwKey) {
            case GLFW_KEY_SPACE:
                return KeyboardKey::Space;
            case GLFW_KEY_APOSTROPHE:
                return KeyboardKey::Apostrophe;
            case GLFW_KEY_COMMA:
                return KeyboardKey::Comma;
            case GLFW_KEY_MINUS:
                return KeyboardKey::Minus;
            case GLFW_KEY_PERIOD:
                return KeyboardKey::Period;
            case GLFW_KEY_SLASH:
                return KeyboardKey::Slash;
            case GLFW_KEY_0:
                return KeyboardKey::Number0;
            case GLFW_KEY_1:
                return KeyboardKey::Number1;
            case GLFW_KEY_2:
                return KeyboardKey::Number2;
            case GLFW_KEY_3:
                return KeyboardKey::Number3;
            case GLFW_KEY_4:
                return KeyboardKey::Number4;
            case GLFW_KEY_5:
                return KeyboardKey::Number5;
            case GLFW_KEY_6:
                return KeyboardKey::Number6;
            case GLFW_KEY_7:
                return KeyboardKey::Number7;
            case GLFW_KEY_8:
                return KeyboardKey::Number8;
            case GLFW_KEY_9:
                return KeyboardKey::Number9;
            case GLFW_KEY_SEMICOLON:
                return KeyboardKey::Semicolon;
            case GLFW_KEY_EQUAL:
                return KeyboardKey::Equal;
            case GLFW_KEY_A:
                return KeyboardKey::A;
            case GLFW_KEY_B:
                return KeyboardKey::B;
            case GLFW_KEY_C:
                return KeyboardKey::C;
            case GLFW_KEY_D:
                return KeyboardKey::D;
            case GLFW_KEY_E:
                return KeyboardKey::E;
            case GLFW_KEY_F:
                return KeyboardKey::F;
            case GLFW_KEY_G:
                return KeyboardKey::G;
            case GLFW_KEY_H:
                return KeyboardKey::H;
            case GLFW_KEY_I:
                return KeyboardKey::I;
            case GLFW_KEY_J:
                return KeyboardKey::J;
            case GLFW_KEY_K:
                return KeyboardKey::K;
            case GLFW_KEY_L:
                return KeyboardKey::L;
            case GLFW_KEY_M:
                return KeyboardKey::M;
            case GLFW_KEY_N:
                return KeyboardKey::N;
            case GLFW_KEY_O:
                return KeyboardKey::O;
            case GLFW_KEY_P:
                return KeyboardKey::P;
            case GLFW_KEY_Q:
                return KeyboardKey::Q;
            case GLFW_KEY_R:
                return KeyboardKey::R;
            case GLFW_KEY_S:
                return KeyboardKey::S;
            case GLFW_KEY_T:
                return KeyboardKey::T;
            case GLFW_KEY_U:
                return KeyboardKey::U;
            case GLFW_KEY_W:
                return KeyboardKey::W;
            case GLFW_KEY_V:
                return KeyboardKey::V;
            case GLFW_KEY_X:
                return KeyboardKey::X;
            case GLFW_KEY_Y:
                return KeyboardKey::Y;
            case GLFW_KEY_Z:
                return KeyboardKey::Z;
            case GLFW_KEY_LEFT_BRACKET:
                return KeyboardKey::LeftBracket;
            case GLFW_KEY_BACKSLASH:
                return KeyboardKey::BackSlash;
            case GLFW_KEY_RIGHT_BRACKET:
                return KeyboardKey::RightBracket;
            case GLFW_KEY_GRAVE_ACCENT:
                return KeyboardKey::GraveAccent;
            case GLFW_KEY_WORLD_1:
                return KeyboardKey::World1;
            case GLFW_KEY_WORLD_2:
                return KeyboardKey::World2;
            case GLFW_KEY_ESCAPE:
                return KeyboardKey::Escape;
            case GLFW_KEY_ENTER:
                return KeyboardKey::Enter;
            case GLFW_KEY_TAB:
                return KeyboardKey::Tab;
            case GLFW_KEY_BACKSPACE:
                return KeyboardKey::BackSpace;
            case GLFW_KEY_INSERT:
                return KeyboardKey::Insert;
            case GLFW_KEY_DELETE:
                return KeyboardKey::Delete;
            case GLFW_KEY_RIGHT:
                return KeyboardKey::Right;
            case GLFW_KEY_LEFT:
                return KeyboardKey::Left;
            case GLFW_KEY_DOWN:
                return KeyboardKey::Down;
            case GLFW_KEY_UP:
                return KeyboardKey::Up;
            case GLFW_KEY_PAGE_UP:
                return KeyboardKey::PageUp;
            case GLFW_KEY_PAGE_DOWN:
                return KeyboardKey::PageDown;
            case GLFW_KEY_HOME:
                return KeyboardKey::Home;
            case GLFW_KEY_END:
                return KeyboardKey::End;
            case GLFW_KEY_CAPS_LOCK:
                return KeyboardKey::CapsLock;
            case GLFW_KEY_SCROLL_LOCK:
                return KeyboardKey::ScrollLock;
            case GLFW_KEY_NUM_LOCK:
                return KeyboardKey::NumLock;
            case GLFW_KEY_PRINT_SCREEN:
                return KeyboardKey::PrintScreen;
            case GLFW_KEY_PAUSE:
                return KeyboardKey::Pause;
            case GLFW_KEY_F1:
                return KeyboardKey::F1;
            case GLFW_KEY_F2:
                return KeyboardKey::F2;
            case GLFW_KEY_F3:
                return KeyboardKey::F3;
            case GLFW_KEY_F4:
                return KeyboardKey::F4;
            case GLFW_KEY_F5:
                return KeyboardKey::F5;
            case GLFW_KEY_F6:
                return KeyboardKey::F6;
            case GLFW_KEY_F7:
                return KeyboardKey::F7;
            case GLFW_KEY_F8:
                return KeyboardKey::F8;
            case GLFW_KEY_F9:
                return KeyboardKey::F9;
            case GLFW_KEY_F10:
                return KeyboardKey::F10;
            case GLFW_KEY_F11:
                return KeyboardKey::F11;
            case GLFW_KEY_F12:
                return KeyboardKey::F12;
            case GLFW_KEY_F13:
                return KeyboardKey::F13;
            case GLFW_KEY_F14:
                return KeyboardKey::F14;
            case GLFW_KEY_F15:
                return KeyboardKey::F15;
            case GLFW_KEY_F16:
                return KeyboardKey::F16;
            case GLFW_KEY_F17:
                return KeyboardKey::F17;
            case GLFW_KEY_F18:
                return KeyboardKey::F18;
            case GLFW_KEY_F19:
                return KeyboardKey::F19;
            case GLFW_KEY_F20:
                return KeyboardKey::F20;
            case GLFW_KEY_F21:
                return KeyboardKey::F21;
            case GLFW_KEY_F22:
                return KeyboardKey::F22;
            case GLFW_KEY_F23:
                return KeyboardKey::F23;
            case GLFW_KEY_F24:
                return KeyboardKey::F24;
            case GLFW_KEY_F25:
                return KeyboardKey::F25;
            case GLFW_KEY_KP_0:
                return KeyboardKey::Numpad0;
            case GLFW_KEY_KP_1:
                return KeyboardKey::Numpad1;
            case GLFW_KEY_KP_2:
                return KeyboardKey::Numpad2;
            case GLFW_KEY_KP_3:
                return KeyboardKey::Numpad3;
            case GLFW_KEY_KP_4:
                return KeyboardKey::Numpad4;
            case GLFW_KEY_KP_5:
                return KeyboardKey::Numpad5;
            case GLFW_KEY_KP_6:
                return KeyboardKey::Numpad6;
            case GLFW_KEY_KP_7:
                return KeyboardKey::Numpad7;
            case GLFW_KEY_KP_8:
                return KeyboardKey::Numpad8;
            case GLFW_KEY_KP_9:
                return KeyboardKey::Numpad9;
            case GLFW_KEY_KP_DECIMAL:
                return KeyboardKey::NumpadDecimal;
            case GLFW_KEY_KP_DIVIDE:
                return KeyboardKey::NumpadDivide;
            case GLFW_KEY_KP_MULTIPLY:
                return KeyboardKey::NumpadMultiply;
            case GLFW_KEY_KP_SUBTRACT:
                return KeyboardKey::NumpadSubtract;
            case GLFW_KEY_KP_ADD:
                return KeyboardKey::NumpadAdd;
            case GLFW_KEY_KP_ENTER:
                return KeyboardKey::Enter;
            case GLFW_KEY_KP_EQUAL:
                return KeyboardKey::Equal;
            case GLFW_KEY_LEFT_SHIFT:
                return KeyboardKey::LeftShift;
            case GLFW_KEY_LEFT_CONTROL:
                return KeyboardKey::LeftControl;
            case GLFW_KEY_LEFT_ALT:
                return KeyboardKey::LeftAlt;
            case GLFW_KEY_LEFT_SUPER:
                return KeyboardKey::LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT:
                return KeyboardKey::RightShift;
            case GLFW_KEY_RIGHT_CONTROL:
                return KeyboardKey::RightControl;
            case GLFW_KEY_RIGHT_ALT:
                return KeyboardKey::RightAlt;
            case GLFW_KEY_RIGHT_SUPER:
                return KeyboardKey::RightSuper;
            case GLFW_KEY_MENU:
                return KeyboardKey::Menu;
        }
        return KeyboardKey::Unknown;
    }

}
