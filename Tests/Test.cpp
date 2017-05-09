/*
 * Test namespace implementation
 *
 * Copyright (C) 2016 Thomas Faber
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "System.hpp"
#include "Test.hpp"
#include <cstdio>
#include <cstdlib>

namespace Test
{

int ExpectedAsserts = 0;
int TestFailures = 0;
int TestsTotal = 0;
const char *LastFailureFunction = NULL;

int
FinishTestRun(void)
{
    std::fprintf(stderr, "%d tests executed, %d failures\n", TestsTotal, TestFailures);
    return TestFailures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

void
Expect(
    bool ConditionResult,
    const char *Condition,
    const char *File,
    int Line,
    const char *Function)
{
    TestsTotal++;
    if (!ConditionResult)
    {
        TestFailures++;
        if (Function != LastFailureFunction)
        {
            std::fprintf(stderr, "In function %s:\n", Function);
            LastFailureFunction = Function;
        }
        std::fprintf(stderr, "%s:%d: Test failed: %s\n", File, Line, Condition);
    }
}

} /* namespace Test */

namespace System
{

void AssertMessage(const char *Message, const char *File, int Line)
{
    (void)Message;
    (void)File;
    (void)Line;

    EXPECT_NOT_EQUAL(Test::ExpectedAsserts, 0);
    if (Test::ExpectedAsserts)
    {
        Test::ExpectedAsserts--;
    }
}

} /* namespace System */
