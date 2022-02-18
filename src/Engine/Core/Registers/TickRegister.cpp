#include "TickRegister.hpp"

#include <algorithm>

namespace mpgl {

    TickRegister::TickRegister(Duration const& period) noexcept
        : lastTime{Clock::now()}, period{period} {}

    void TickRegister::onEvent(void) noexcept {
        TimePoint now = Clock::now();
        auto difference = std::chrono::duration_cast<
            std::chrono::milliseconds>(now - lastTime);
        if (difference < period)
            return;
        std::ranges::for_each(storage, [&difference](auto& event){ event->onTick(difference); });
        lastTime = now;
    }

}
