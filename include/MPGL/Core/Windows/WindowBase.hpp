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
        typedef std::shared_ptr<Drawable2D>    DrawablePtr;
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
         * Returns constant reference to the drawables shared
         * pointer at given index
         *
         * @param index the index of the drawable
         * @return constant reference to the drawables shared
         * pointer
         */
        [[nodiscard]] DrawablePtr const&
            operator[] (size_type index) const noexcept
                { return drawables[index]; }

        /**
         * Returns reference to the drawables shared pointer
         * at given index
         *
         * @param index the index of the drawable
         * @return reference to the drawables shared
         * pointer
         */
        [[nodiscard]] DrawablePtr&
            operator[] (size_type index) noexcept
                { return drawables[index]; }

        using iterator
            = typename Drawables::iterator;
        using const_iterator
            = typename Drawables::const_iterator;
        using reverse_iterator
            = typename Drawables::reverse_iterator;
        using const_reverse_iterator
            = typename Drawables::const_reverse_iterator;

        /**
         * Returns the iterator to the begining of the drawables
         * vector
         *
         * @return iterator to the begining of the drawables
         * vector
         */
        [[nodiscard]] iterator begin(void) noexcept
            { return drawables.begin(); }

        /**
         * Returns the iterator to the end of the drawables
         * vector
         *
         * @return iterator to the end of the drawables
         * vector
         */
        [[nodiscard]] iterator end(void) noexcept
            { return drawables.end(); }

        /**
         * Returns the constant iterator to the begining of
         * the drawables vector
         *
         * @return constant iterator to the begining of the
         * drawables vector
         */
        [[nodiscard]] const_iterator begin(void) const noexcept
            { return drawables.begin(); }

        /**
         * Returns the constant iterator to the end of
         * the drawables vector
         *
         * @return constant iterator to the end of the
         * drawables vector
         */
        [[nodiscard]] const_iterator end(void) const noexcept
            { return drawables.end(); }

        /**
         * Returns the constant iterator to the begining of
         * the drawables vector
         *
         * @return constant iterator to the begining of the
         * drawables vector
         */
        [[nodiscard]] const_iterator cbegin(void) const noexcept
            { return drawables.cbegin(); }

        /**
         * Returns the constant iterator to the end of
         * the drawables vector
         *
         * @return constant iterator to the end of the
         * drawables vector
         */
        [[nodiscard]] const_iterator cend(void) const noexcept
            { return drawables.cend(); }

        /**
         * Returns the reverse iterator to the end of
         * the drawables vector
         *
         * @return reverse iterator to the end of the
         * drawables vector
         */
        [[nodiscard]] reverse_iterator rbegin(void) noexcept
            { return drawables.rbegin(); }

        /**
         * Returns the reverse iterator to the begining of
         * the drawables vector
         *
         * @return reverse iterator to the begining of the
         * drawables vector
         */
        [[nodiscard]] reverse_iterator rend(void) noexcept
            { return drawables.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the drawables vector
         *
         * @return constant reverse iterator to the end of the
         * drawables vector
         */
        [[nodiscard]] const_reverse_iterator
            rbegin(void) const noexcept
                { return drawables.rbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the drawables vector
         *
         * @return constant reverse iterator to the begining of the
         * drawables vector
         */
        [[nodiscard]] const_reverse_iterator
            rend(void) const noexcept
                { return drawables.rend(); }

        /**
         * Returns the constant reverse iterator to the end of
         * the drawables vector
         *
         * @return constant reverse iterator to the end of the
         * drawables vector
         */
        [[nodiscard]] const_reverse_iterator
            crbegin(void) const noexcept
                { return drawables.crbegin(); }

        /**
         * Returns the constant reverse iterator to the begining of
         * the drawables vector
         *
         * @return constant reverse iterator to the begining of the
         * drawables vector
         */
        [[nodiscard]] const_reverse_iterator
            crend(void) const noexcept
                { return drawables.crend(); }

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
         */
        explicit WindowBase(void) noexcept = default;

        EventBus                                    events;
        Drawables                                   drawables;
    };

}

#include <MPGL/Core/Windows/WindowBase.tpp>
