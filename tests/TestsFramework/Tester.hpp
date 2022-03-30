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
#include <string>
#include <vector>
#include <tuple>

namespace mpgl::tests {

    typedef std::vector<bool>                   AssertVector;
    typedef std::function<void(AssertVector&)>  TestFunction;

    /**
     * The interface of the test module
     */
    class Tester {
    public:
        typedef std::string                     String;
        typedef std::size_t                     size_type;
        // name of function, positives, all
        typedef std::tuple<String, size_type,
            size_type>                          TestResult;
        typedef std::vector<TestResult>         ResultVector;

        Tester(void) = delete;

        /**
         * Adds the new function to the checked functions
         *
         * @param name the name of the function
         * @param func the function object
         * @return the true char
         */
        [[nodiscard]] static char mock(
            std::string const& name,
            TestFunction func) noexcept
                { testVector.emplace_back(name, func); return true; }

        /**
         * Returns the results of testing
         *
         * @return the vector containing the results of testing
         */
        [[nodiscard]] static ResultVector testFunctions(void) noexcept;
    private:
        typedef std::pair<String, TestFunction> TestPair;
        typedef std::vector<TestPair>           TestVector;

        /**
         * Tests the given function
         *
         * @param pair the constant reference to the name-function pair
         * @return the result of the testing
         */
        static TestResult testFunction(
            TestPair const& pair) noexcept;

        static thread_local TestVector          testVector;
    };

}
