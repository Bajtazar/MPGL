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

#include <functional>
#include <algorithm>
#include <memory>
#include <future>
#include <vector>
#include <thread>
#include <atomic>

namespace mpgl::async {

    /**
     * Manages a threadpool. Automaticly distributes tasks between
     * working threads and allows to get task future
     */
    class Threadpool;

    namespace details {

        /**
         * Interface for promise types used by task class.
         * Stores generalized informations about task and allows
         * to awake parent with different template specializations
         */
        class PromiseTypeInterface {
        protected:
            /**
             * optimization of virtual void awake(void) const = 0
             * [the only virtual function so reduces number of
             * function pointer calls from 2 to 1 without
             * memory size overhead]. Awakes this coroutine
             */
            typedef void(*Awaker)(PromiseTypeInterface const&);

            /**
             * Signalizes that a coroutine has ended its work.
             * Inform parent coroutine [if that coroutine exists]
             * that its child has ended work. If all of parent's
             * children has ended its work then awakes the parent
             * coroutine. If parent does not exist then does
             * nothing
             */
            void finish(void);

            std::atomic<size_t>         childrenCounter = {};
            Threadpool*                 threadpool = nullptr;
            PromiseTypeInterface*       parent = nullptr;
            Awaker                      awake;
        };

    }

}
