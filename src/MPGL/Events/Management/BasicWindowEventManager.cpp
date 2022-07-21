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
#include <MPGL/Events/Management/BasicWindowEventManager.hpp>

namespace mpgl {

    void BasicWindowEventManager::onScreenTransformation(
        Layout& layout,
        Vector2u const& oldDimensions) noexcept
    {
        get<ScreenTransformationRegister>(registers).onEvent(
            layout, oldDimensions);
    }

    void BasicWindowEventManager::onScreenTransformation(
        std::vector<std::unique_ptr<Layout>>& layouts,
        Vector2u const& oldDimensions) noexcept
    {
        get<ScreenTransformationRegister>(registers).onEvent(
            layouts, oldDimensions);
    }

    void BasicWindowEventManager::onMouseRelease(
        MouseButton const& button) noexcept
    {
        get<MouseReleaseRegister>(registers).onEvent(button);
    }

    void BasicWindowEventManager::onWindowClose(void) noexcept {
        get<WindowCloseRegister>(registers).onEvent();
    }

    void BasicWindowEventManager::onMouseMotion(
        Vector2f const& position) noexcept
    {
        get<MouseMotionRegister>(registers).onEvent(position);
    }

    void BasicWindowEventManager::onMousePress(
        MouseButton const& button) noexcept
    {
        get<MousePressRegister>(registers).onEvent(button);
    }

    void BasicWindowEventManager::onKeyRelease(
        Key const& key) noexcept
    {
        get<KeyReleaseRegister>(registers).onEvent(key);
    }

    void BasicWindowEventManager::onTextWrite(
        std::string const& unicodeString) noexcept
    {
        get<TextWriteRegister>(registers).onEvent(unicodeString);
    }

    void BasicWindowEventManager::onKeyPress(Key const& key) noexcept {
        get<KeyPressRegister>(registers).onEvent(key);
    }

    void BasicWindowEventManager::onScroll(
        Vector2f const& scroll) noexcept
    {
        get<ScrollRegister>(registers).onEvent(scroll);
    }

    void BasicWindowEventManager::onTick(
        std::chrono::milliseconds const& delta) noexcept
    {
        for (auto& event : get<TickRegister>(registers))
            event->onTick(delta);
    }

    void BasicWindowEventManager::onTick(void) noexcept
    {
        get<TickRegister>(registers).onEvent();
    }

    void BasicWindowEventManager::setPeriod(
        Duration const& period) noexcept
    {
        get<TickRegister>(registers).setPeriod(period);
    }

    void BasicWindowEventManager::push(STEPtr const& event) {
        get<ScreenTransformationRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(STEPtr&& event) {
        get<ScreenTransformationRegister>(registers).pushBack(
            std::move(event));
    }

    void BasicWindowEventManager::push(MREPtr const& event) {
        get<MouseReleaseRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(MREPtr&& event) {
        get<MouseReleaseRegister>(registers).pushBack(std::move(event));
    }

    void BasicWindowEventManager::push(MMEPtr const& event) {
        get<MouseMotionRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(MMEPtr&& event) {
        get<MouseMotionRegister>(registers).pushBack(std::move(event));
    }

    void BasicWindowEventManager::push(WCEPtr const& event) {
        get<WindowCloseRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(WCEPtr&& event) {
        get<WindowCloseRegister>(registers).pushBack(std::move(event));
    }

    void BasicWindowEventManager::push(MPEPtr const& event) {
        get<MousePressRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(MPEPtr&& event) {
        get<MousePressRegister>(registers).pushBack(std::move(event));
    }

    void BasicWindowEventManager::push(KREPtr const& event) {
        get<KeyReleaseRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(KREPtr&& event) {
        get<KeyReleaseRegister>(registers).pushBack(std::move(event));
    }

    void BasicWindowEventManager::push(TWEPtr const& event) {
        get<TextWriteRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(TWEPtr&& event) {
        get<TextWriteRegister>(registers).pushBack(std::move(event));
    }

    void BasicWindowEventManager::push(KPEPtr const& event) {
        get<KeyPressRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(KPEPtr&& event) {
        get<KeyPressRegister>(registers).pushBack(std::move(event));
    }

    void BasicWindowEventManager::push(SEPtr const& event) {
        get<ScrollRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(SEPtr&& event) {
        get<ScrollRegister>(registers).pushBack(std::move(event));
    }

    void BasicWindowEventManager::push(TEPtr const& event) {
        get<TickRegister>(registers).pushBack(event);
    }

    void BasicWindowEventManager::push(TEPtr&& event) {
        get<TickRegister>(registers).pushBack(std::move(event));
    }

}
