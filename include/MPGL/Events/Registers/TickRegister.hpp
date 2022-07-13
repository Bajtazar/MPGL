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

#include <MPGL/Events/Registers/RegisterInterface.hpp>
#include <MPGL/Events/Types/TickEvent.hpp>
#include <MPGL/Traits/Types.hpp>

#include <vector>
#include <memory>

namespace mpgl {

    /**
     * Register that holds the tick events
     */
    class TickRegister : public RegisterInterface<TickEvent> {
    public:
        typedef std::chrono::duration<float64, std::milli>  Duration;

        /**
         * Constructs a new Tick Register object from the given
         * period between ticks
         *
         * @param period the period between ticks
         */
        explicit TickRegister(Duration const& period = {}) noexcept;

        TickRegister(TickRegister&&) = default;

        TickRegister& operator=(TickRegister&&) = default;

        /**
         * Sets a new tick period
         *
         * @param period the new tick period
         */
        void setPeriod(Duration const& period) noexcept
            { this->period = period; }

        /**
         * Returns the tick period
         *
         * @return the tick period
         */
        [[nodiscard]] Duration const& getPeriod(void) const noexcept
            { return period; }

        /**
         * Calls the tick event on the underlying objects
         */
        void onEvent(void) noexcept;

        /**
         * Destroys the Tick Register object
         */
        ~TickRegister(void) noexcept = default;
    private:
        typedef std::chrono::steady_clock                   Clock;
        typedef std::chrono::time_point<Clock>              TimePoint;

        Clock                                               clock;
        TimePoint                                           lastTime;
        Duration                                            period;
    };

}
