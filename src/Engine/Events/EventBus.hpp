#pragma once

#include "../Core/Registers/UniversalRegister.hpp"
#include "../Core/Registers/RegistersHolder.hpp"
#include "../Core/Registers/TickRegister.hpp"
#include "ScreenTransformationEvent.hpp"
#include "MouseReleaseEvent.hpp"
#include "WindowCloseEvent.hpp"
#include "MouseMotionEvent.hpp"
#include "MousePressEvent.hpp"
#include "KeyReleaseEvent.hpp"
#include "TextWriteEvent.hpp"
#include "KeyPressEvent.hpp"
#include "ScrollEvent.hpp"

namespace ge {

    typedef UniversalRegister<ScreenTransformationEvent,
        void(ScreenTransformationEvent::*)(Vector2u const&),
        &ScreenTransformationEvent::onScreenTransformation>     ScreenTransformationRegister;

    typedef UniversalRegister<MouseReleaseEvent, void(MouseReleaseEvent::*)(MouseButton const&),
        &MouseReleaseEvent::onMouseRelease>                 MouseReleaseRegister;

    typedef UniversalRegister<MouseMotionEvent, void(MouseMotionEvent::*)(Vector2f const&),
        &MouseMotionEvent::onMouseMotion>                   MouseMotionRegister;

    typedef UniversalRegister<WindowCloseEvent, void(WindowCloseEvent::*)(void),
        &WindowCloseEvent::onWindowClose>                   WindowCloseRegister;

    typedef UniversalRegister<MousePressEvent, void(MousePressEvent::*)(MouseButton const&),
        &MousePressEvent::onMousePress>                     MousePressRegister;

    typedef UniversalRegister<KeyReleaseEvent, void(KeyReleaseEvent::*)(Key const&),
        &KeyReleaseEvent::onKeyRelease>                     KeyReleaseRegister;

    typedef UniversalRegister<TextWriteEvent, void(TextWriteEvent::*)(std::string const&),
        &TextWriteEvent::onTextWrite>                       TextWriteRegister;

    typedef UniversalRegister<KeyPressEvent, void(KeyPressEvent::*)(Key const&),
        &KeyPressEvent::onKeyPress>                         KeyPressRegister;

    typedef UniversalRegister<ScrollEvent, void(ScrollEvent::*)(Vector2f const&),
        &ScrollEvent::onScroll>                             ScrollRegistry;

    typedef RegistersHolder<TickRegister, ScreenTransformationRegister,
        MouseReleaseRegister, MouseMotionRegister, MousePressRegister,
        WindowCloseRegister, KeyReleaseRegister, TextWriteRegister,
        KeyPressRegister, ScrollRegistry>                   EventBus;

}
