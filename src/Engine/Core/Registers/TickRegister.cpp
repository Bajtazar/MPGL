#include "TickRegister.hpp"

#include <algorithm>

namespace ge {

    TickRegister::TickRegister(Duration const& period) noexcept
        : lastTime{Clock::now()}, period{period} {}

    void TickRegister::onEvent(void) {
        TimePoint now = Clock::now();
        Duration difference = std::chrono::duration_cast<Duration>(now - lastTime);
        if (difference < period)
            return;
        std::ranges::for_each(storage, [&difference](auto& event){ event->onTick(difference); });
        lastTime = now;
    }

}
