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

#include <MPGL/Core/Windows/RenderWindow.hpp>

#include <memory>

namespace mpgl {

    /**
     * Wraps the Render Window reference and calls the render method
     * on each onTick call. Transmits each event to the Render
     * Window reference
     */
    class RenderingAdapter :
        public ScreenTransformationEvent,
        public WindowMotionEvent,
        public MouseReleaseEvent,
        public WindowCloseEvent,
        public MouseMotionEvent,
        public MousePressEvent,
        public KeyReleaseEvent,
        public TextWriteEvent,
        public KeyPressEvent,
        public ScrollEvent,
        public TickEvent
    {
    public:
        /**
         * Construct a new Rendering Adapter object. Wraps the
         * Render Window reference and saves the background
         * color
         *
         * @param renderWindow the Render Window reference
         * @param cleaning the buffer cleaning options
         * @param background the color of the background used by
         * the render method
         */
        explicit RenderingAdapter(
            RenderWindow& renderWindow,
            CleaningOptions const& cleaning
                = CleaningOptions::ColorAndDepthAndStencil,
            Color const& background = {}) noexcept
                : background{background}, cleaning{cleaning},
                reference{renderWindow} {}

        RenderingAdapter(RenderingAdapter const&) noexcept = default;
        RenderingAdapter(RenderingAdapter&&) noexcept = default;

        RenderingAdapter& operator=(
            RenderingAdapter const&) noexcept = default;
        RenderingAdapter& operator=(
            RenderingAdapter&&) noexcept = default;

        /**
         * Calls the onScreenTransformation method on the
         * Render Window reference
         *
         * @param oldDimensions the old dimensions of the screen
         */
        void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept final;

        /**
         * Calls the onMouseRelease method on the Render Window
         * reference
         *
         * @param button the released button
         */
        void onMouseRelease(MouseButton const& button) noexcept final;

        /**
         * Informs contained objects that window has been moved
         */
        void onWindowMotion(
            Vector2u const& oldPosition) noexcept final;

        /**
         * Calls the onWindowClose method on the Render Window
         * reference
         */
        void onWindowClose(void) noexcept final;

        /**
         * Calls the onMouseMotion method on the Render Window
         * reference
         *
         * @param position the position of the mouse
         */
        void onMouseMotion(Vector2f const& position) noexcept final;

        /**
         * Calls the onMousePress method on the Render Window
         * reference
         *
         * @param button the pressed button
         */
        void onMousePress(MouseButton const& button) noexcept final;

        /**
         * Calls the onKeyRelease method on the Render Window
         * reference
         *
         * @param key the released key
         */
        void onKeyRelease(Key const& key) noexcept final;

        /**
         * Calls the onTextWrite method on the Render Window
         * reference
         *
         * @param unicodeString the unicode character saved in
         * UTF-8 format
         */
        void onTextWrite(
            std::string const& unicodeString) noexcept final;

        /**
         * Calls the onKeyPress method on the Render Window
         * reference
         *
         * @param key the pressed key
         */
        void onKeyPress(Key const& key) noexcept final;

        /**
         * Calls the onScroll method on the Render Window
         * reference
         *
         * @param scroll the scroll position
         */
        void onScroll(Vector2f const& scroll) noexcept final;

        /**
         * Calls the render and onTick methods on the Render Window
         * reference
         *
         * @param delta the time between last and current tick
         */
        void onTick(
            std::chrono::milliseconds const& delta) noexcept final;

        /**
         * Get the Texture object
         *
         * @return the rendered texture
         */
        [[nodiscard]] Texture const& getTexture(void) const noexcept
            { return reference.get().getTexture(); }

        /**
         * Destroy the Rendering Adapter object
         */
        ~RenderingAdapter(void) noexcept = default;
    private:
        typedef std::reference_wrapper<RenderWindow>    RenderWindowRef;

        Color                                           background;
        CleaningOptions                                 cleaning;
        RenderWindowRef                                 reference;
    };

}
