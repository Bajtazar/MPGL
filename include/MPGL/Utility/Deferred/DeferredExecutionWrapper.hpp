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
#include <MPGL/Traits/Concepts.hpp>

#include <memory>
#include <functional>

namespace mpgl {

    /**
     * Wrapps the invocable and it's arguments that are supposed to
     * be executed deferredly. Stores the resource handles as the
     * weak pointers. If the resource is not available via the
     * weak pointer then abandons the execution of the invocable
     *
     * @tparam Handles the shared pointer resources types
     */
    template <InstanceOf<std::shared_ptr>... Handles>
    class DeferredExecutionWrapper {
    public:
        /**
         * Wrapps the non-handling resources of the delegation wrapper
         *
         * @tparam Args the non-handling types
         */
        template <PureType... Args>
        class InvocableArgumentsWrapper {
        public:
            /**
             * Wrapps the invocable. Decides whether to execute
             * the invocable or not
             *
             * @tparam Function the wrapped invocable type
             */
            template <std::invocable<Handles..., Args...> Function>
            class DeferredExecutor {
            public:
                /**
                 * Constructs a new deffered executor object
                 *
                 * @param handles the rvalue reference to the
                 * delegation wrapper
                 * @param arguments the rvalue reference to the
                 * arguments wrapper
                 * @param invocable the rvalue reference to the
                 * invocable object
                 */
                explicit DeferredExecutor(
                    DeferredExecutionWrapper&& handles,
                    InvocableArgumentsWrapper&& arguments,
                    Function&& invocable);

                /**
                 * Checks whether the resource-handling references
                 * are still valid. If so then the wrapped invocable
                 * is executed. If not then nothing is done
                 */
                void operator() (void) const;
            private:
                /**
                 * Checks if the resource handlers has expired
                 *
                 * @return whether the resource handlers has expired
                 */
                bool hasExpired(void) const;

                /**
                 * Converts the weak resource pointers to the shared
                 * pointers
                 *
                 * @return the tuple with the shared resource-handling
                 * pointers
                 */
                auto replenishSharedPointers(void) const;

                DeferredExecutionWrapper       handles;
                InvocableArgumentsWrapper        arguments;
                Function                invocable;
            };

            /**
             * Constructs a new invocable arguments wrapper object
             *
             * @param wrapper the rvalue referece to the delegation
             * wrapper
             * @param args the non-handling arguments
             */
            explicit InvocableArgumentsWrapper(
                DeferredExecutionWrapper&& wrapper,
                Args... args);

            /**
             * Creates the functor that handles and manages the given
             * arguments, resources and invocable. This method cannot
             * be called more than once
             *
             * @tparam Function the invocable type
             * @param func the rvalue reference to the invocable
             * @return the delegated functor
             */
            template <std::invocable<Handles..., Args...> Function>
            [[nodiscard]] DeferredExecutor<Function> operator() (
                Function&& func);
        private:
            typedef std::tuple<Args...> ArgTuple;

            DeferredExecutionWrapper            wrapper;
            ArgTuple                            args;
        };

        /**
         * Constructs a new deffered execution wrapper object
         *
         * @param handles the resource handling shared pointer types
         */
        explicit DeferredExecutionWrapper(Handles&&... handles);

        /**
         * Creates the functor that handles and manages the given
         * arguments, resources and invocable. This method cannot
         * be called more than once
         *
         * @tparam Args the non-handling argument types
         * @tparam Func the invocable type
         * @param functor the rvalue reference to the invocable object
         * @param args the invocable's non-handling arguments
         * @return the delegated functor
         */
        template <PureType... Args,
            std::invocable<Handles..., Args...> Func>
        [[nodiscard]] auto operator() (Func&& functor, Args... args);
    private:
        /**
         * Converts the shared pointers to the weak pointers
         */
        class WeakPointers {
            /**
             * Returns the tuple containing the weak versions of the
             * given shared pointers
             *
             * @tparam Args the shared pointers types
             * @param handles the shared pointers
             * @return the tuple used to determin weak pointers types
             */
            template <typename... Args>
            static constexpr auto weaken(Args... handles);
        public:
            using Type = decltype(weaken(std::declval<
                std::remove_cvref_t<Handles>>()...));
        };

        typedef WeakPointers::Type              HandlingTuple;

        HandlingTuple                           handles;
    };

}

#include <MPGL/Utility/Deferred/DeferredExecutionWrapper.tpp>
