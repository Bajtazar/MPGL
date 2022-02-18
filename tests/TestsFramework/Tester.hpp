#pragma once

#include <functional>
#include <string>
#include <vector>
#include <tuple>

namespace mpgl::tests {

    typedef std::vector<bool>                   AssertVector;
    typedef std::function<void(AssertVector&)>  TestFunction;

    class Tester {
    public:
        typedef std::string                     String;
        typedef std::size_t                     size_type;
        // name of function, positives, all
        typedef std::tuple<String, size_type,
            size_type>                          TestResult;
        typedef std::vector<TestResult>         ResultVector;

        Tester(void) = delete;

        static char mock(std::string const& name,
            TestFunction func) noexcept
                { testVector.emplace_back(name, func); return true; }

        static ResultVector testFunctions(void) noexcept;
    private:
        typedef std::pair<String, TestFunction> TestPair;
        typedef std::vector<TestPair>           TestVector;

        static TestResult testFunction(
            TestPair const& pair) noexcept;

        static thread_local TestVector          testVector;
    };

}
