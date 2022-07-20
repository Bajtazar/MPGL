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

#include <MPGL/Events/Management/BasicWindowEventManager.hpp>
#include <MPGL/Core/Context/Context.hpp>
#include <MPGL/Collections/Image.hpp>
#include <MPGL/Events/EventBus.hpp>
#include <MPGL/Core/Drawable.hpp>

namespace mpgl {

    /**
     * Base class for windows. Allows to insert drawables into
     * the window object and automatically separates drawables
     * into event registers they derive from
     */
    class WindowBase : protected GraphicalObject {
    public:
        typedef std::shared_ptr<Drawable2D>         DrawablePtr;
        typedef std::unique_ptr<WindowEventManager> EventManagerPtr;
        typedef std::vector<DrawablePtr>            Drawables;
        typedef std::size_t                         size_type;

        WindowBase(WindowBase&&) = delete;
        WindowBase(WindowBase const&) = delete;

        WindowBase& operator=(WindowBase&&) = delete;
        WindowBase& operator=(WindowBase const&) = delete;

        /**
         * Pushes event into event registers it derives from
         *
         * @tparam Tp the type of the event
         * @param event the event shared pointer object
         */
        template <Event Tp>
        void pushEvent(std::shared_ptr<Tp> const& event);

        /**
         * Emplaces event into event registers it derives from
         *
         * @tparam Tp the type of the event
         * @tparam Args the type of arguments passed into the event
         * constructor
         * @param args the arguments passed into event constructor
         */
        template <Event Tp, typename... Args>
            requires std::constructible_from<Tp, Args...>
        void emplaceEvent(Args&&... args);

        /**
         * Pushes drawable into drawable vector and adds drawable
         * to event registers it derives from
         *
         * @tparam Tp the type of the drawable
         * @param drawable the drawable shared pointer object
         */
        template <std::derived_from<Drawable2D> Tp>
        void pushDrawable(std::shared_ptr<Tp> const& drawable);

        /**
         * Moves drawable into drawable vector and adds drawable
         * to event registers it derives from
         *
         * @tparam Tp the type of the drawable
         * @param drawable the drawable shared pointer object
         */
        template <std::derived_from<Drawable2D> Tp>
        void pushDrawable(std::shared_ptr<Tp>&& drawable);

        /**
         * Emplaces drawable into drawables vector and adds
         * drawable to event registers it derives from
         *
         * @tparam Tp the type of the drawable
         * @tparam Args the type of arguments passed into the
         * drawable constructor
         * @param args the arguments passed into the drawable
         * constructor
         */
        template <std::derived_from<Drawable2D> Tp,
            typename... Args>
                requires std::constructible_from<Tp, Args...>
        void emplaceDrawable(Args&&... args);

        /**
         * Pure virtual method. Has to be overloaded. Saves the
         * current window screen to the image
         *
         * @return the window screen shot
         */
        [[nodiscard]] virtual Image saveWindowScreen(void) const = 0;

        /**
         * Virtual destructor. Destroy the Window Base object
         */
        virtual ~WindowBase(void) noexcept = default;
    protected:

        /**
         * Construct a new Window Base object
         *
         * @param eventManager the unique pointer with the event
         * manager object
         */
        explicit WindowBase(EventManagerPtr&& eventManager) noexcept
            : eventManager{std::move(eventManager)} {}

        EventManagerPtr                             eventManager;
        Drawables                                   drawables;
    };

}

#include <MPGL/Core/Windows/WindowBase.tpp>
