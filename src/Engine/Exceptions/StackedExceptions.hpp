/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (c) 2021-2022 Grzegorz Czarnecki (wahadlowiec@interia.pl)
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

#include "../Traits/Concepts.hpp"
#include "MPGLException.hpp"

#include <exception>
#include <list>

namespace mpgl {

    /**
     * Exception that allows to stack multiple exceptions
     *
     */
    class StackedExceptions : public MPGLException {
    private:
        typedef std::list<std::exception_ptr>       Exceptions;
    public:
        /**
         * Constructs StackedException from a given iterators
         *
         * @tparam Iter iterator type for the given collection.
         * Its value type has to be an std::exception_ptr
         * @tparam Sent sentinel type for the given collection
         * @param iter the iterator that marks the begining
         * of the copied range
         * @param sent the sentinel for the copied range
         */
        template <std::input_iterator Iter,
            std::sentinel_for<Iter> Sent>
                requires SameIterType<Iter, std::exception_ptr>
        explicit StackedExceptions(Iter iter, Sent const& sent);

        /**
         * Constructs StackedException from a given range
         *
         * @tparam Range range type of given range. Its value
         * type has to be an std::exception_ptr
         * @param range the given exceptions range
         */
        template <std::ranges::input_range Range>
            requires SameRangeType<Range, std::exception_ptr>
        explicit StackedExceptions(Range&& range);

        StackedExceptions(StackedExceptions const&) = default;
        StackedExceptions(StackedExceptions&&) = default;

        StackedExceptions& operator=(
            StackedExceptions const&) = default;

        StackedExceptions& operator=(
            StackedExceptions&&) = default;

        /**
         * Returns information how many exceptions has been stacked
         * inside the object.
         *
         * @return message with number of exceptions
         */
        const char* what(void) const noexcept
            { return message.c_str(); }

        /**
         * Returns how many exceptions has been stacked inside
         * the object.
         *
         * @return number of exceptions
         */
        std::size_t size(void) const noexcept
            { return exceptions.size(); }

        typedef typename Exceptions::const_iterator const_iterator;

        /**
         * Returns the begin of the exceptions constant iterator
         *
         * @return the begin iterator
         */
        const_iterator begin(void) const noexcept
            { return exceptions.begin(); }

        /**
         * Returns the end of the exceptions constant iterator
         *
         * @return the end iterator
         */
        const_iterator end(void) const noexcept
            { return exceptions.end(); }

        /**
         * Returns the begin of the exceptions constant iterator
         *
         * @return the begin iterator
         */
        const_iterator cbegin(void) const noexcept
            { return exceptions.cbegin(); }

        /**
         * Returns the end of the exceptions constant iterator
         *
         * @return the end iterator
         */
        const_iterator cend(void) const noexcept
            { return exceptions.cend(); }

        /**
         * Destroy the Stacked Exceptions object
         */
        ~StackedExceptions(void) noexcept = default;
    private:
        Exceptions const                            exceptions;
        std::string const                           message;

        template <std::input_iterator Iter,
            std::sentinel_for<Iter> Sent>
        static std::string createMessageFromExceptions(
            Iter const& iter,
            Sent const& sent);
    };

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
            requires SameIterType<Iter, std::exception_ptr>
    StackedExceptions::StackedExceptions(Iter iter, Sent const& sent)
        : exceptions{iter, sent},
        message{createMessageFromExceptions(iter, sent)} {}

    template <std::ranges::input_range Range>
        requires SameRangeType<Range, std::exception_ptr>
    StackedExceptions::StackedExceptions(Range&& range)
        : StackedExceptions{std::ranges::begin(range),
            std::ranges::end(range)} {}

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    std::string StackedExceptions::createMessageFromExceptions(
        Iter const& iter,
        Sent const& sent)
    {
        return {"There is " + std::to_string(
            std::distance(iter, sent)) + " gathered exceptions\n"};
    }

}