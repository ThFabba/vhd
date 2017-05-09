/*
 * Test declarations
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
#pragma once

namespace Test
{

extern int ExpectedAsserts;
extern int TestFailures;
extern int TestsTotal;
extern const char *LastFailureFunction;

int FinishTestRun(void);
void Expect(bool ConditionResult, const char *Condition, const char *File, int Line, const char *Function);

} /* namespace Test */

#define EXPECT(cond) Test::Expect(cond, #cond, __FILE__, __LINE__, __FUNCTION__)
#define EXPECT_EQUAL(val1, val2) Test::Expect((val1) == (val2), #val1 " == " #val2, __FILE__, __LINE__, __FUNCTION__)
#define EXPECT_NOT_EQUAL(val1, val2) Test::Expect((val1) != (val2), #val1 " != " #val2, __FILE__, __LINE__, __FUNCTION__)
