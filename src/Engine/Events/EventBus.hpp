#pragma once

#include "../Core/Registers/UniversalRegister.hpp"
#include "../Core/Registers/RegistersHolder.hpp"
#include "../Core/Registers/TickRegister.hpp"
#include "ScreenTransformationEvent.hpp"
#include "MouseReleaseEvent.hpp"
#include "MouseMotionEvent.hpp"
#include "MousePressEvent.hpp"
#include "KeyReleaseEvent.hpp"
#include "TextWriteEvent.hpp"
#include "KeyPressEvent.hpp"
#include "ScrollEvent.hpp"

namespace ge {

    typedef UniversalRegister<ScreenTransformationEvent,
        void(ScreenTransformationEvent::*)(Vector2ui const&),
        &ScreenTransformationEvent::onScreenTransformation>     ScreenTransformationRegister;

    typedef UniversalRegister<MouseReleaseEvent, void(MouseReleaseEvent::*)(MouseButton const&),
        &MouseReleaseEvent::onMouseReleaseEvent>                MouseReleaseRegister;

    typedef UniversalRegister<MouseMotionEvent, void(MouseMotionEvent::*)(Vector2f const&),
        &MouseMotionEvent::onMouseMotionEvent>                  MouseMotionRegister;

    typedef UniversalRegister<MousePressEvent, void(MousePressEvent::*)(MouseButton const&),
        &MousePressEvent::onMousePressEvent>                    MousePressRegister;

    typedef UniversalRegister<KeyReleaseEvent, void(KeyReleaseEvent::*)(Key const&),
        &KeyReleaseEvent::onKeyReleaseEvent>                    KeyReleaseRegister;

    typedef UniversalRegister<TextWriteEvent, void(TextWriteEvent::*)(std::string const&),
        &TextWriteEvent::onTextWirteEvent>                      TextWriteEventRegister;

    typedef UniversalRegister<KeyPressEvent, void(KeyPressEvent::*)(Key const&),
        &KeyPressEvent::onKeyPressEvent>                        KeyPressRegister;

    typedef UniversalRegister<ScrollEvent, void(ScrollEvent::*)(Vector2f const&),
        &ScrollEvent::onScrollEvent>                            ScrollRegistry;

    typedef RegistersHolder<TickRegister, ScreenTransformationRegister,
        MouseReleaseRegister, MouseMotionRegister, MousePressRegister,
        KeyReleaseRegister, TextWriteEventRegister, KeyPressRegister,
        ScrollRegistry>                                         EventBus;

}
