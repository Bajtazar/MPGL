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

#include <MPGL/Core/Layouts/Layout.hpp>
#include <MPGL/Events/EventBus.hpp>

namespace mpgl {

    namespace details {

        /**
         * Base for the event manager class. Implements interface for
         * one event
         *
         * @tparam EventTp the type of the event
         */
        template <Event EventTp>
        class EventManagerBase : public EventTp {
        public:
            /**
             * Constructs a new event manager base tail object
             */
            explicit EventManagerBase(void) noexcept = default;

            EventManagerBase(EventManagerBase const&) = delete;
            EventManagerBase(EventManagerBase&&) = delete;

            EventManagerBase& operator=(
                EventManagerBase const&) = delete;
            EventManagerBase& operator=(
                EventManagerBase&&) = delete;

            /**
             * Destroys the event manager base object
             */
            virtual ~EventManagerBase(void) noexcept = default;
        protected:
            /**
             * Registers an event pointer in the event manager
             *
             * @param event the constant reference to the event's
             * shared pointer
             */
            virtual void push(
                std::shared_ptr<EventTp> const& event) = 0;

            /**
             * Registers an event pointer in the event manager
             *
             * @param event the rvalue reference to the event's
             * shared pointer
             */
            virtual void push(
                std::shared_ptr<EventTp>&& event) = 0;

            /**
             * Registers an event object in the event manager if
             * the event type is derived from the EventTp type
             *
             * @tparam Ep the event's type
             * @param event the constant reference to the event's
             * shared pointer
             */
            template <Event Ep>
            void pushIfDerived(std::shared_ptr<Ep> const& event);
        };

        /**
         * Specialization of the event manager base class for the
         * tick event. Allows to set a new period of the tick
         */
        template <>
        class EventManagerBase<TickEvent> : public TickEvent {
        public:
            typedef std::chrono::duration<float64,
                std::milli>                     Duration;

            /**
             * Constructs a new event manager base tail object
             */
            explicit EventManagerBase(void) noexcept = default;

            EventManagerBase(EventManagerBase const&) = delete;
            EventManagerBase(EventManagerBase&&) = delete;

            EventManagerBase& operator=(
                EventManagerBase const&) = delete;
            EventManagerBase& operator=(
                EventManagerBase&&) = delete;

            /**
             * Informs tick event observers that the tick has occurred.
             * Propagates already existing tick
             *
             * @param delta the time difference between last and current
             * tick
             */
            virtual void onTick(
                std::chrono::milliseconds const& delta) noexcept = 0;

            /**
             * Informs tick event observers that a new tick has
             * occured. Creates tick "from scratch"
             */
            virtual void onTick(void) noexcept = 0;

            /**
             * Sets a new tick period
             *
             * @param period the new tick period
             */
            virtual void setPeriod(Duration const& period) noexcept = 0;

            /**
             * Destroys the event manager base object
             */
            virtual ~EventManagerBase(void) noexcept = default;
        protected:
            /**
             * Registers an event pointer in the event manager
             *
             * @param event the constant reference to the tick event's
             * shared pointer
             */
            virtual void push(
                std::shared_ptr<TickEvent> const& event) = 0;

            /**
             * Registers an event pointer in the event manager
             *
             * @param event the rvalue reference to the tick event's
             * shared pointer
             */
            virtual void push(
                std::shared_ptr<TickEvent>&& event) = 0;

            /**
             * Registers an event object in the event manager if
             * the event type is derived from the tick event
             *
             * @tparam Ep the event's type
             * @param event the constant reference to the event's
             * shared pointer
             */
            template <Event Ep>
            void pushIfDerived(std::shared_ptr<Ep> const& event);
        };

    }

    /**
     * An interface for the event managers. Event manager is called in
     * the active window directly after the event occurrence and it
     * decides what to do next (it can inform observers about given
     * event or take more sophisticated actions). It allows to design
     * more optimised code by removing redundancies (for example:
     * checking in the every event handler whether key X has been
     * pressed independently can be replaced with event manager that
     * check it once and calls a designated subgroup of observers)
     *
     * @tparam Events the types of events that are being managed
     */
    template <Event... Events>
    class EventManager : public details::EventManagerBase<Events>... {
    public:
        /**
         * Constructs a new event manager object
         */
        explicit EventManager(void) noexcept = default;

        EventManager(EventManager const&) = delete;
        EventManager(EventManager&&) = delete;

        EventManager& operator=(
            EventManager const&) = delete;
        EventManager& operator=(
            EventManager&&) = delete;

        /**
         * Register an event pointer in the event manager
         *
         * @tparam Ep the event's type
         * @param event the constant reference to the event's shared
         * pointer
         */
        template <Event Ep>
        void push(std::shared_ptr<Ep> const& event);

        /**
         * Emplaces an event in the event manager
         *
         * @tparam Ep the event's type
         * @tparam Args the event's constructor argument types
         * @param args the event's constructor arguments
         */
        template <Event Ep, typename... Args>
            requires std::constructible_from<Ep, Args...>
        void emplace(Args&&... args);

        /**
         * Destroys the event manager object
         */
        virtual ~EventManager(void) noexcept = default;
    };

    typedef EventManager<
        ScreenTransformationEvent,
        MouseReleaseEvent,
        MouseMotionEvent,
        WindowCloseEvent,
        MousePressEvent,
        KeyReleaseEvent,
        TextWriteEvent,
        KeyPressEvent,
        ScrollEvent,
        TickEvent>                              WindowEventManager;

}

#include <MPGL/Events/Management/EventManager.tpp>
