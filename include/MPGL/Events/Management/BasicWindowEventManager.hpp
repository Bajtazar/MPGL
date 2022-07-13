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

#include <MPGL/Events/Management/EventManager.hpp>

namespace mpgl {

    class BasicWindowEventManager : public WindowEventManager {
    public:
        explicit BasicWindowEventManager(void) noexcept = default;

        BasicWindowEventManager(
            BasicWindowEventManager const&) = delete;
        BasicWindowEventManager(
            BasicWindowEventManager&&) = default;

        BasicWindowEventManager& operator=(
            BasicWindowEventManager const&) = delete;
        BasicWindowEventManager& operator=(
            BasicWindowEventManager&&) = default;

        void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept final;

        void onMouseRelease(MouseButton const& button) noexcept final;

        void onWindowClose(void) noexcept final;

        void onMouseMotion(Vector2f const& position) noexcept final;

        void onMousePress(MouseButton const& button) noexcept final;

        void onKeyRelease(Key const& key) noexcept final;

        void onTextWrite(
            std::string const& unicodeString) noexcept final;

        void onKeyPress(Key const& key) noexcept final;

        void onScroll(Vector2f const& scroll) noexcept final;

        void onTick(
            std::chrono::milliseconds const& delta) noexcept final;

        ~BasicWindowEventManager(void) noexcept = default;
    private:
        typedef std::shared_ptr<ScreenTransformationEvent>  STEPtr;
        typedef std::shared_ptr<MouseReleaseEvent>          MREPtr;
        typedef std::shared_ptr<MouseMotionEvent>           MMEPtr;
        typedef std::shared_ptr<WindowCloseEvent>           WCEPtr;
        typedef std::shared_ptr<MousePressEvent>            MPEPtr;
        typedef std::shared_ptr<KeyReleaseEvent>            KREPtr;
        typedef std::shared_ptr<TextWriteEvent>             TWEPtr;
        typedef std::shared_ptr<KeyPressEvent>              KPEPtr;
        typedef std::shared_ptr<ScrollEvent>                SEPtr;
        typedef std::shared_ptr<TickEvent>                  TEPtr;

        void push(STEPtr const& event) final;

        void push(STEPtr&& event) final;

        void push(MREPtr const& event) final;

        void push(MREPtr&& event) final;

        void push(MMEPtr const& event) final;

        void push(MMEPtr&& event) final;

        void push(WCEPtr const& event) final;

        void push(WCEPtr&& event) final;

        void push(MPEPtr const& event) final;

        void push(MPEPtr&& event) final;

        void push(KREPtr const& event) final;

        void push(KREPtr&& event) final;

        void push(TWEPtr const& event) final;

        void push(TWEPtr&& event) final;

        void push(KPEPtr const& event) final;

        void push(KPEPtr&& event) final;

        void push(SEPtr const& event) final;

        void push(SEPtr&& event) final;

        void push(TEPtr const& event) final;

        void push(TEPtr&& event) final;

        EventBus                                            registers;
    };

}
