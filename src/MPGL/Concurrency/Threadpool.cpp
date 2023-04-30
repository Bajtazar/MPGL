/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2023
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
#include <MPGL/Concurrency/Threadpool.hpp>

namespace mpgl::async {

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
