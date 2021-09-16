#pragma once

#include "../../Events/TickEvent.hpp"
#include "RegisterInterface.hpp"

#include <vector>
#include <memory>

namespace ge {

    class TickRegister : public RegisterInterface {
    public:
        typedef std::chrono::duration<double, std::milli>   Duration;
        typedef std::shared_ptr<TickEvent>                  pointer;
        typedef TickEvent                                   value_type;
        typedef std::vector<pointer>                        Storage;

        explicit TickRegister(Duration const& period = {}) noexcept;

        void setPeriod(Duration const& period) noexcept
            { this->period = period; }

        void pushBack(pointer const& pointer)
            { storage.push_back(pointer); }
        void pushBack(pointer&& pointer) noexcept
            { storage.push_back(std::move(pointer)); }

        void onEvent(void);

        using iterator = typename Storage::iterator;
        using const_iterator = typename Storage::const_iterator;
        using reverse_iterator = typename Storage::reverse_iterator;
        using const_reverse_iterator = typename Storage::const_reverse_iterator;

        iterator begin(void) noexcept { return storage.begin(); }
        iterator end(void) noexcept { return storage.end(); }
        const_iterator begin(void) const noexcept { return storage.begin(); }
        const_iterator end(void) const noexcept { return storage.end(); }
        const_iterator cbegin(void) const noexcept { return storage.cbegin(); }
        const_iterator cend(void) const noexcept { return storage.cend(); }

        reverse_iterator rbegin(void) noexcept { return storage.rbegin(); }
        reverse_iterator rend(void) noexcept { return storage.rend(); }
        const_reverse_iterator rbegin(void) const noexcept { return storage.rbegin(); }
        const_reverse_iterator rend(void) const noexcept { return storage.rend(); }
        const_reverse_iterator crbegin(void) const noexcept { return storage.crbegin(); }
        const_reverse_iterator crend(void) const noexcept { return storage.crend(); }

        std::size_t size(void) const noexcept { return storage.size(); }
        bool empty(void) const noexcept { return storage.empty(); }

        pointer& operator[] (std::size_t index) noexcept { return storage[index]; }
        pointer const& operator[] (std::size_t index) const noexcept { return storage[index]; }

        ~TickRegister(void) noexcept = default;
    private:
        typedef std::chrono::steady_clock                   Clock;
        typedef std::chrono::time_point<Clock>              TimePoint;

        Clock                                               clock;
        Storage                                             storage;
        TimePoint                                           lastTime;
        Duration                                            period;
    };

}
