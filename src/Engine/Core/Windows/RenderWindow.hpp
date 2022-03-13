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

#include "../Textures/Texture.hpp"
#include "WindowBase.hpp"

namespace mpgl {

    /**
     * Renders given content inside a texture. Directly modifies
     * the texture which simplifies interaction. Allows to perform
     * events on handled objects like a normal window
     */
    class RenderWindow : public WindowBase,
        public ScreenTransformationEvent,
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
        typedef Texture::Options                    Options;

        /**
         * The default texture options for Render Window class
         */
        constexpr static Options                    defaultOptions = {
            Options::TextureWrapper::ClampToEdge,
            Options::TextureWrapper::ClampToEdge,
            Options::MinifyingTextureFilter::Linear,
            Options::MagnifyingTextureFilter::Linear,
            Color{}, false
        };

        /**
         * Construct a new Render Window object with given
         * texture options
         *
         * @param options the texture options
         */
        explicit RenderWindow(Options const& options
            = defaultOptions);

        RenderWindow(RenderWindow const&) = delete;

        /**
         * Construct a new Render Window object from a given
         * rvalue Render Window object
         *
         * @param window the moved framed window
         */
        RenderWindow(RenderWindow&& window) noexcept;

        RenderWindow& operator=(RenderWindow const&) = delete;

        /**
         * Assigns the given rvalue Franed Window object
         *
         * @param window the given rvalue Render Window object
         * @return reference to this object
         */
        RenderWindow& operator=(RenderWindow&& window) noexcept;

        /**
         * Renders the frame into the texture
         *
         * @param cleaning the buffer cleaning options
         * @param color the color of the background
         */
        void render(CleaningOptions const& cleaning
            = CleaningOptions::ColorAndDepthAndStencil,
            Color const& color = {}) noexcept;

        /**
         * Perform screen transformation on the inside
         * drawable objects
         *
         * @param oldDimensions the old dimensions of the window
         */
        void onScreenTransformation(
            Vector2u const& oldDimensions) noexcept final;

        /**
         * Informs contained objects that mouse button has been
         * released
         *
         * @param button the released mouse button
         */
        void onMouseRelease(MouseButton const& button) noexcept final;

        /**
         * Informs contained objects that window has been closed
         */
        void onWindowClose(void) noexcept final;

        /**
         * Informs contained objects that mouse has been shifted
         *
         * @param position the new mouse position
         */
        void onMouseMotion(Vector2f const& position) noexcept final;

        /**
         * Informs contained objects that mouse button has been
         * pressed
         *
         * @param button the pressed mouse button
         */
        void onMousePress(MouseButton const& button) noexcept final;

        /**
         * Informs contained objects that keyboard key has been
         * released
         *
         * @param key the released key
         */
        void onKeyRelease(Key const& key) noexcept final;

        /**
         * Informs contained objects that unicode character
         * has been inserted
         *
         * @param unicodeString the unicode character saved in
         * UTF-8 format
         */
        void onTextWrite(
            std::string const& unicodeString) noexcept final;

        /**
         * Informs contained objects that keyboard key has been
         * pressed
         *
         * @param key the pressed key
         */
        void onKeyPress(Key const& key) noexcept final;

        /**
         * Informs contained objects that scroll has been moved
         *
         * @param scroll the scroll position
         */
        void onScroll(Vector2f const& scroll) noexcept final;

        /**
         * Calls the tick event on the contained objects
         *
         * @param duration the time between last and current tick
         */
        void onTick(
            std::chrono::milliseconds const& duration) noexcept final;

        /**
         * Get the Texture object
         *
         * @return the rendered texture
         */
        [[nodiscard]] Texture const& getTexture(void) const noexcept
            { return windowTexture; }

        /**
         * Saves the current window screen to the image.
         *
         * @return the window screen shot
         */
        [[nodiscard]] Image saveWindowScreen(void) const final;

        /**
         * Destroy the Render Window object
         */
        ~RenderWindow(void) noexcept;
    private:
        Texture                                     windowTexture;
        uint32                                      framebuffer;
        uint32                                      renderbuffer;

        /**
         * Finishes the texture configuration
         *
         * @param buffer the texture buffer
         * @param options the texture options
         */
        void finishTextureSetting(
            TextureBuffer const& buffer,
            Options const& options) noexcept;

        /**
         * Binds renderbuffer to framebuffer

         */
        void bindRenderbuffer(void) noexcept;

        /**
         * Binds current framebuffer object
         *
         * @param cleaning the buffer cleaning options
         * @param color the color of the background
         */
        void bind(
            CleaningOptions const& cleaning,
            Color const& color) noexcept;

        /**
         * Unbinds the current framebuffer
         */
        void unbind(void) noexcept;
    };

}
