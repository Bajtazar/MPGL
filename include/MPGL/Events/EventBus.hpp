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
#pragma once

#include <MPGL/Events/Types/ScreenTransformationEvent.hpp>
#include <MPGL/Events/Registers/UniversalRegister.hpp>
#include <MPGL/Events/Registers/RegistersHolder.hpp>
#include <MPGL/Events/Types/WindowMotionEvent.hpp>
#include <MPGL/Events/Types/MouseReleaseEvent.hpp>
#include <MPGL/Events/Types/WindowCloseEvent.hpp>
#include <MPGL/Events/Types/MouseMotionEvent.hpp>
#include <MPGL/Events/Registers/TickRegister.hpp>
#include <MPGL/Events/Types/MousePressEvent.hpp>
#include <MPGL/Events/Types/KeyReleaseEvent.hpp>
#include <MPGL/Events/Types/TextWriteEvent.hpp>
#include <MPGL/Events/Types/KeyPressEvent.hpp>
#include <MPGL/Events/Types/ScrollEvent.hpp>

namespace mpgl {

    /// The screen transformation event register
    typedef UniversalRegister<ScreenTransformationEvent, void(ScreenTransformationEvent::*)(Vector2u const&),
        &ScreenTransformationEvent::onScreenTransformation> ScreenTransformationRegister;

    /// The window motion event register
    typedef UniversalRegister<WindowMotionEvent, void(WindowMotionEvent::*)(Vector2u const&),
        &WindowMotionEvent::onWindowMotion>                 WindowMotionRegister;

    /// The mouse release event register
    typedef UniversalRegister<MouseReleaseEvent, void(MouseReleaseEvent::*)(MouseButton const&),
        &MouseReleaseEvent::onMouseRelease>                 MouseReleaseRegister;

    /// The mouse motion event register
    typedef UniversalRegister<MouseMotionEvent, void(MouseMotionEvent::*)(Vector2f const&),
        &MouseMotionEvent::onMouseMotion>                   MouseMotionRegister;

    /// The window close event register
    typedef UniversalRegister<WindowCloseEvent, void(WindowCloseEvent::*)(void),
        &WindowCloseEvent::onWindowClose>                   WindowCloseRegister;

    /// The mouse press event register
    typedef UniversalRegister<MousePressEvent, void(MousePressEvent::*)(MouseButton const&),
        &MousePressEvent::onMousePress>                     MousePressRegister;

    /// The key release event register
    typedef UniversalRegister<KeyReleaseEvent, void(KeyReleaseEvent::*)(KeyboardKey const&),
        &KeyReleaseEvent::onKeyRelease>                     KeyReleaseRegister;

    /// The text write event register
    typedef UniversalRegister<TextWriteEvent, void(TextWriteEvent::*)(std::string const&),
        &TextWriteEvent::onTextWrite>                       TextWriteRegister;

    /// The key press event register
    typedef UniversalRegister<KeyPressEvent, void(KeyPressEvent::*)(KeyboardKey const&),
        &KeyPressEvent::onKeyPress>                         KeyPressRegister;

    /// The scroll event register
    typedef UniversalRegister<ScrollEvent, void(ScrollEvent::*)(Vector2f const&),
        &ScrollEvent::onScroll>                             ScrollRegister;

    /// The MPGL events registers holder
    typedef RegistersHolder<TickRegister, ScreenTransformationRegister,
        WindowMotionRegister, MouseReleaseRegister, MouseMotionRegister,
        MousePressRegister, WindowCloseRegister, KeyReleaseRegister,
        TextWriteRegister, KeyPressRegister,
        ScrollRegister>                                     EventBus;

}
