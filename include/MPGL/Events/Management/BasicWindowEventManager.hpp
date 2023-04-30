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

#include <MPGL/Events/Management/EventManager.hpp>

namespace mpgl {

    /**
     * Calls all event observers directly after the occurrence of
     * the event. Used by default in the MPGL windows
     */
    class BasicWindowEventManager : public WindowEventManager {
    public:
        /**
         * Constructs a new basic window event manager object
         */
        explicit BasicWindowEventManager(void) noexcept = default;

        BasicWindowEventManager(
            BasicWindowEventManager const&) = delete;
        BasicWindowEventManager(
            BasicWindowEventManager&&) = default;

        BasicWindowEventManager& operator=(
            BasicWindowEventManager const&) = delete;
        BasicWindowEventManager& operator=(
            BasicWindowEventManager&&) = default;

        /**
         * Informs screen transformation event observers that the
         * screen dimensions has been changed
         *
         * @param oldDimensions the old dimensions of the screen
         */
        void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept final;

        /**
         * Informs mouse release event observers that the window
         * has been moved
         *
         * @param oldPosition the old window position
         */
        void onWindowMotion(Vector2u const& oldPosition) noexcept final;

        /**
         * Informs mouse release event observers that the mouse button
         * has been released
         *
         * @param button the clicked mouse button
         */
        void onMouseRelease(MouseButton const& button) noexcept final;

        /**
         * Informs window close event observers that window has been
         * closed
         */
        void onWindowClose(void) noexcept final;

        /**
         * Informs mouse motion event observers that the mouse has been
         * moved
         *
         * @param position the new position of the mouse (in the
         * coordinates relative to the window)
         */
        void onMouseMotion(Vector2f const& position) noexcept final;

        /**
         * Informs mouse press event observers that the mouse button
         * has been pressed
         *
         * @param button the clicked mouse button
         */
        void onMousePress(MouseButton const& button) noexcept final;

        /**
         * Informs key release event observers that the keyboard key
         * has been released
         *
         * @param key the released keyboard key
         */
        void onKeyRelease(KeyboardKey const& key) noexcept final;

        /**
         * Informs text write event observers that the text has been
         * written
         *
         * @param unicodeString the written text in the unicode format
         */
        void onTextWrite(
            std::string const& unicodeString) noexcept final;

        /**
         * Informs key press event observers that the keyboard key has
         * been pressed
         *
         * @param key the pressed keyboard key
         */
        void onKeyPress(KeyboardKey const& key) noexcept final;

        /**
         * Informs scroll event observers that the mouse scroll has
         * been scrolled
         *
         * @param scroll the mouse scroll values
         */
        void onScroll(Vector2f const& scroll) noexcept final;

        /**
         * Informs tick event observers that the tick has occurred.
         * Propagates already existing tick
         *
         * @param delta the time difference between last and current
         * tick
         */
        void onTick(
            std::chrono::milliseconds const& delta) noexcept final;

        /**
         * Informs tick event observers that the tick has occurred.
         * Creates tick "from scratch"
         */
        void onTick(void) noexcept final;

        /**
         * Sets a new tick period
         *
         * @param period the new tick period
         */
        void setPeriod(Duration const& period) noexcept final;

        /**
         * Destroys the basic window event manager object
         */
        ~BasicWindowEventManager(void) noexcept = default;
    private:
        typedef std::shared_ptr<ScreenTransformationEvent>  STEPtr;
        typedef std::shared_ptr<WindowMotionEvent>          WMEPtr;
        typedef std::shared_ptr<MouseReleaseEvent>          MREPtr;
        typedef std::shared_ptr<MouseMotionEvent>           MMEPtr;
        typedef std::shared_ptr<WindowCloseEvent>           WCEPtr;
        typedef std::shared_ptr<MousePressEvent>            MPEPtr;
        typedef std::shared_ptr<KeyReleaseEvent>            KREPtr;
        typedef std::shared_ptr<TextWriteEvent>             TWEPtr;
        typedef std::shared_ptr<KeyPressEvent>              KPEPtr;
        typedef std::shared_ptr<ScrollEvent>                SEPtr;
        typedef std::shared_ptr<TickEvent>                  TEPtr;

        /**
         * Registers screen transformation event pointer in the manager
         *
         * @param event the constant reference to the screen
         * transformation event shared pointer
         */
        void push(STEPtr const& event) final;

        /**
         * Registers screen transformation event pointer in the manager
         *
         * @param event the rvalue reference to the screen
         * transformation event shared pointer
         */
        void push(STEPtr&& event) final;

        /**
         * Registers window motion event pointer in the manager
         *
         * @param event the constant reference to the window motion
         * event shared pointer
         */
        void push(WMEPtr const& event) final;

        /**
         * Registers window motion event pointer in the manager
         *
         * @param event the rvalue reference to the window motion
         * event shared pointer
         */
        void push(WMEPtr&& event) final;

        /**
         * Registers mouse release event pointer in the manager
         *
         * @param event the constant reference to the mouse release
         * event shared pointer
         */
        void push(MREPtr const& event) final;

        /**
         * Registers mouse release event pointer in the manager
         *
         * @param event the rvalue reference to the mouse release
         * event shared pointer
         */
        void push(MREPtr&& event) final;

        /**
         * Registers mouse motion event pointer in the manager
         *
         * @param event the constant reference to the mouse motion
         * event shared pointer
         */
        void push(MMEPtr const& event) final;

        /**
         * Registers mouse motion event pointer in the manager
         *
         * @param event the rvalue reference to the mouse motion
         * event shared pointer
         */
        void push(MMEPtr&& event) final;

        /**
         * Registers window close event pointer in the manager
         *
         * @param event the constant reference to the window close
         * event shared pointer
         */
        void push(WCEPtr const& event) final;

        /**
         * Registers window close event pointer in the manager
         *
         * @param event the rvalue reference to the window close
         * event shared pointer
         */
        void push(WCEPtr&& event) final;

        /**
         * Registers mouse press event pointer in the manager
         *
         * @param event the constant reference to the mouse press
         * event shared pointer
         */
        void push(MPEPtr const& event) final;

        /**
         * Registers mouse press event pointer in the manager
         *
         * @param event the rvalue reference to the mouse press
         * event shared pointer
         */
        void push(MPEPtr&& event) final;

        /**
         * Registers keyboard release event pointer in the manager
         *
         * @param event the constant reference to the keyboard release
         * event shared pointer
         */
        void push(KREPtr const& event) final;

        /**
         * Registers keyboard release event pointer in the manager
         *
         * @param event the rvalue reference to the keyboard release
         * event shared pointer
         */
        void push(KREPtr&& event) final;

        /**
         * Registers text write event pointer in the manager
         *
         * @param event the constant reference to the text write
         * event shared pointer
         */
        void push(TWEPtr const& event) final;

        /**
         * Registers text write event pointer in the manager
         *
         * @param event the rvalue reference to the text write
         * event shared pointer
         */
        void push(TWEPtr&& event) final;

        /**
         * Registers key press event pointer in the manager
         *
         * @param event the constant reference to the keyboard press
         * event shared pointer
         */
        void push(KPEPtr const& event) final;

        /**
         * Registers key press event pointer in the manager
         *
         * @param event the constant reference to the keyboard press
         * event shared pointer
         */
        void push(KPEPtr&& event) final;

        /**
         * Registers scroll event pointer in the manager
         *
         * @param event the constant reference to the scroll event
         * shared pointer
         */
        void push(SEPtr const& event) final;

        /**
         * Registers rvalue event pointer in the manager
         *
         * @param event the constant reference to the scroll event
         * shared pointer
         */
        void push(SEPtr&& event) final;

        /**
         * Registers tick event pointer in the manager
         *
         * @param event the constant reference to the tick event
         * shared pointer
         */
        void push(TEPtr const& event) final;

        /**
         * Registers tick event pointer in the manager
         *
         * @param event the rvalue reference to the tick event
         * shared pointer
         */
        void push(TEPtr&& event) final;

        EventBus                                            registers;
    };

}
