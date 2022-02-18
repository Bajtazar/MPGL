#include "Threadpool.hpp"

namespace mpgl {

    uint32 Threadpool::threadpoolSize(uint32 size) noexcept {
        return size ? size :
            std::thread::hardware_concurrency();
    }

    Threadpool::Threadpool(uint32 size) {
        for (uint32 i = 0; i != threadpoolSize(size); ++i)
            threads.emplace_back(&Threadpool::worker, this,
                stopSource.get_token(), queues.emplace());
        link.setLink(queues.attachment());
    }

    void Threadpool::worker(
        StopToken stopToken, Attachment queueLink) noexcept
    {
        while (!stopToken.stop_requested()) {
            if (auto task = acquireTask(queueLink))
                (*task)();
            else
                std::this_thread::yield();
        }
    }

    Threadpool::OptionalTask Threadpool::acquireTask(
        Attachment const& localQueue) noexcept
    {
        if (auto task = localQueue->pop())
            return task;
        for (auto otherQueue = localQueue; ++otherQueue != localQueue; )
            if (auto task = otherQueue->pop())
                return task;
        return {};
    }

    void Threadpool::QueueLink::setLink(
        Attachment attachment) noexcept
    {
        Guard guard{mutex};
        link = attachment;
    }

}
