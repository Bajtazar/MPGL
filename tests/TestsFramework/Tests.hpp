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

#include "Tester.hpp"
#include "Compare.hpp"

#ifndef ge_tests__module
#define ge_tests__module

#define ge_tests_vec            mpgl::tests::AssertVector
#define ge_tests_Tester         mpgl::tests::Tester

/**
 * The test function macro
 *
 * @param TestName the name of the test function
 */
#define Test(TestName) \
    void TestName (ge_tests_vec& ge_tests_asserts); \
    volatile char TestName##_residue = ge_tests_Tester::mock( \
        #TestName, TestName); \
    void TestName (ge_tests_vec& ge_tests_asserts)

/**
 * Begins the constexpr test
 *
 * @param TestName the name of the test function
 */
#define BeginConstexprTest(TestName) \
    constexpr bool TestName (void) { bool ge_test_assertions = true;

/**
 * The constexpr assert macro
 *
 * @param assertion the assertion boolean value
 */
#define ConstexprAssert(assertion) \
    ge_test_assertions = ge_test_assertions && assertion;

/**
 * The constexpr equal macro
 *
 * @param left the left object
 * @param right the right object
 */
#define ConstexprEqual(left, right) \
    ge_test_assertions = ge_test_assertions && mpgl::tests::compare( \
    left , right);


/**
 * The constexpr equal macro
 *
 * @param left the left iterator object
 * @param right the right iterator object
 */
#define ConstexprEqualIter(leftIter, leftSent, rightIter, rightSent) \
    ge_test_assertions = ge_test_assertions && mpgl::tests::compare( \
    leftIter , leftSent , rightIter , rightSent );

/**
 * The constexpr on throw macro
 *
 * @param assertion the checked construction
 * @param exception the generated exception
 */
#define ConstexprOnThrow(assertion, exception) \
    try { \
        assertion ; \
        ge_test_assertions = false; \
    } catch ( exception const& ) {}

/**
 * Ends the constexpr test
 */
#define EndConstexprTest return ge_test_assertions; }

/**
 * The assert macro
 *
 * @param assertion the assertion boolean value
 */
#define Assert(assertion) \
    ge_tests_asserts.push_back(assertion);

/**
 * The equal macro
 *
 * @param left the left object
 * @param right the right object
 */
#define Equal(left, right) \
    ge_tests_asserts.push_back(mpgl::tests::compare(left, right));

/**
 * The equal macro
 *
 * @param left the left iterator object
 * @param right the right iterator object
 */
#define EqualIter(leftIter, leftSent, rightIter, rightSent) \
    ge_tests_asserts.push_back(mpgl::tests::compare( \
    leftIter , leftSent , rightIter , rightSent );

/**
 * The on throw macro
 *
 * @param assertion the checked construction
 * @param exception the generated exception
 */
#define OnThrow(assertion, exception) \
    try { \
        assertion ; \
        ge_tests_asserts.push_back(false); \
    } catch ( exception const& ) { \
        ge_tests_asserts.push_back(true); \
    }

#endif
