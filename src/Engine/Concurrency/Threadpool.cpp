#include "Threadpool.hpp"

namespace ge {

    uint32 Threadpool::threadpoolSize(uint32 size) noexcept {
        return size ? size :
            std::thread::hardware_concurrency();
    }

    Threadpool::Threadpool(uint32 size) {
        for (uint32 i = 0; i != threadpoolSize(size); ++i)
            threads.emplace_back(&worker, this);
    }

    void Threadpool::worker(StopToken stopToken) noexcept {
        while (!stopToken.stop_requested()) {
            if (auto task = taskQueue.pop())
                (*task)();
            else
                std::this_thread::yield();
        }
    }

}
