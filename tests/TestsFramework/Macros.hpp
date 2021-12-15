#pragma once

#include "Tester.hpp"
#include "Compare.hpp"

#ifndef ge_tests__module
#define ge_tests__module

#define ge_tests_vec            ge::tests::AssertVector
#define ge_tests_Tester         ge::tests::Tester

#define Test(TestName) \
    void TestName (ge_tests_vec& ge_tests_asserts); \
    volatile char TestName##_residue = ge_tests_Tester::mock( \
        #TestName, TestName); \
    void TestName (ge_tests_vec& ge_tests_asserts)

#define Assert(assertion) \
    ge_tests_asserts.push_back(assertion);

#define Equal(left, right) \
    ge_tests_asserts.push_back(ge::tests::compare(left, right));

#define OnThrow(assertion, exception) \
    try { \
        assertion ; \
        ge_tests_asserts.push_back(false); \
    } catch ( exception const& ) { \
        ge_tests_asserts.push_back(true); \
    }

#endif
