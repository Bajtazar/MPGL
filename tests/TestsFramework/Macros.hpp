#pragma once

#include "Tester.hpp"
#include "Compare.hpp"

#ifndef ge_tests__module
#define ge_tests__module

#define ge_tests_vec            mpgl::tests::AssertVector
#define ge_tests_Tester         mpgl::tests::Tester

#define Test(TestName) \
    void TestName (ge_tests_vec& ge_tests_asserts); \
    volatile char TestName##_residue = ge_tests_Tester::mock( \
        #TestName, TestName); \
    void TestName (ge_tests_vec& ge_tests_asserts)

#define BeginConstexprTest(TestName) \
    constexpr bool TestName (void) { bool ge_test_assertions = true;

#define ConstexprAssert(assertion) \
    ge_test_assertions = ge_test_assertions && assertion;

#define ConstexprEqual(left, right) \
    ge_test_assertions = ge_test_assertions && mpgl::tests::compare( \
    left , right);

#define ConstexprEqualIter(leftIter, leftSent, rightIter, rightSent) \
    ge_test_assertions = ge_test_assertions && mpgl::tests::compare( \
    leftIter , leftSent , rightIter , rightSent );

#define ConstexprOnThrow(assertion, exception) \
    try { \
        assertion ; \
        ge_test_assertions = false; \
    } catch ( exception const& ) {}

#define EndConstexprTest return ge_test_assertions; }

#define Assert(assertion) \
    ge_tests_asserts.push_back(assertion);

#define Equal(left, right) \
    ge_tests_asserts.push_back(mpgl::tests::compare(left, right));

#define EqualIter(leftIter, leftSent, rightIter, rightSent) \
    ge_tests_asserts.push_back(mpgl::tests::compare( \
    leftIter , leftSent , rightIter , rightSent );

#define OnThrow(assertion, exception) \
    try { \
        assertion ; \
        ge_tests_asserts.push_back(false); \
    } catch ( exception const& ) { \
        ge_tests_asserts.push_back(true); \
    }

#endif
