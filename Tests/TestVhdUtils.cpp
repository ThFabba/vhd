/*
 * Vhd::Utils class tests
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
#include "Test.hpp"
#include "Vhd.hpp"
#include <cstring>

static
void
TestUnixTimeFromVhdTime(void)
{
    int64_t unixTime, unixTime2;
    int64_t days;

    /* Lower bound of VHD time, 2000-01-01 00:00:00 UTC */
    unixTime = Vhd::Utils::UnixTimeFromVhdTime(0);
    days = (2000 - 1970) * (365 * 4 + 1) / 4;
    EXPECT_EQUAL(unixTime, days * 24 * 60 * 60);
    EXPECT_EQUAL(unixTime, 946684800);

    /* Unix time and VHD time both count seconds, so should have equal diffs */
    unixTime2 = Vhd::Utils::UnixTimeFromVhdTime(1000000);
    EXPECT_EQUAL(unixTime2 - unixTime, 1000000 - 0);

    /* 2016-07-31 12:34:56 UTC */
    unixTime = Vhd::Utils::UnixTimeFromVhdTime(523283696);
    EXPECT_EQUAL(unixTime, 1469968496);

    /* Upper bound of 32 bit signed unix time, 2038-01-19 03:14:07 UTC */
    unixTime = Vhd::Utils::UnixTimeFromVhdTime(1200798847);
    EXPECT_EQUAL(unixTime, 2147483647);
    unixTime = Vhd::Utils::UnixTimeFromVhdTime(1200798848);
    EXPECT_EQUAL(unixTime, 2147483648);

    /* Upper bound of 32 bit unsigned unix time, 2106-07-02 06:28:15 UTC */
    unixTime = Vhd::Utils::UnixTimeFromVhdTime(3348282495);
    EXPECT_EQUAL(unixTime, 4294967295);
    unixTime = Vhd::Utils::UnixTimeFromVhdTime(3348282496);
    EXPECT_EQUAL(unixTime, 4294967296);

    /* Upper bound of VHD time, 2136-07-02 06:28:15 UTC */
    unixTime = Vhd::Utils::UnixTimeFromVhdTime(0xffffffff);
    EXPECT_EQUAL(unixTime, 5241652095);
}

static
void
TestVhdTimeFromUnixTime(void)
{
    uint32_t vhdTime, vhdTime2;

    /* Zero and negative unix time is out of range for VHD, should return 0 */
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(0);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(-1);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(-9223372036854775806);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(-9223372036854775807);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime((int64_t)((uint64_t)-9223372036854775807 - 1));
    EXPECT_EQUAL(vhdTime, 0);

    /* Lower bound of VHD time, 2000-01-01 00:00:00 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(946684799);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(946684800);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(946684801);
    EXPECT_EQUAL(vhdTime, 1);

    /* Unix time and VHD time both count seconds, so should have equal diffs */
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(2000000000);
    vhdTime2 = Vhd::Utils::VhdTimeFromUnixTime(2000000000 + 1000000);
    EXPECT_EQUAL(vhdTime2 - vhdTime, 1000000);

    /* 2016-07-31 12:34:56 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(1469968496);
    EXPECT_EQUAL(vhdTime, 523283696);

    /* Upper bound of 32 bit signed unix time, 2038-01-19 03:14:07 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(2147483647);
    EXPECT_EQUAL(vhdTime, 1200798847);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(2147483648);
    EXPECT_EQUAL(vhdTime, 1200798848);

    /* Upper bound of 32 bit unsigned unix time, 2106-07-02 06:28:15 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(4294967295);
    EXPECT_EQUAL(vhdTime, 3348282495);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(4294967296);
    EXPECT_EQUAL(vhdTime, 3348282496);

    /* Upper bound of VHD time, 2136-07-02 06:28:15 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(5241652094);
    EXPECT_EQUAL(vhdTime, 0xfffffffe);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(5241652095);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(5241652096);
    EXPECT_EQUAL(vhdTime, 0xffffffff);

    /* Large out of bound numbers should return the max value 0xffffffff */
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(6000000000);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(1000000000000);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(1000000000000000);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(9223372036854775806);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromUnixTime(9223372036854775807);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
}

static
void
TestNtTimeFromVhdTime(void)
{
    uint64_t ntTime, ntTime2;

    /* Lower bound of VHD time, 2000-01-01 00:00:00 UTC */
    ntTime = Vhd::Utils::NtTimeFromVhdTime(0);
    EXPECT_EQUAL(ntTime, 125911584000000000);

    /* NT time counts in 100ns intervals */
    ntTime2 = Vhd::Utils::NtTimeFromVhdTime(1000000);
    EXPECT_EQUAL(ntTime2 - ntTime, (uint64_t)(1000000 - 0) * 10000000);

    /* 2016-07-31 12:34:56 UTC */
    ntTime = Vhd::Utils::NtTimeFromVhdTime(523283696);
    EXPECT_EQUAL(ntTime, 131144420960000000);

    /* Upper bound of 32 bit signed unix time, 2038-01-19 03:14:07 UTC */
    ntTime = Vhd::Utils::NtTimeFromVhdTime(1200798847);
    EXPECT_EQUAL(ntTime, 137919572470000000);
    ntTime = Vhd::Utils::NtTimeFromVhdTime(1200798848);
    EXPECT_EQUAL(ntTime, 137919572480000000);

    /* Upper bound of 32 bit unsigned unix time, 2106-07-02 06:28:15 UTC */
    ntTime = Vhd::Utils::NtTimeFromVhdTime(3348282495);
    EXPECT_EQUAL(ntTime, 159394408950000000);
    ntTime = Vhd::Utils::NtTimeFromVhdTime(3348282496);
    EXPECT_EQUAL(ntTime, 159394408960000000);

    /* Upper bound of VHD time, 2136-07-02 06:28:15 UTC */
    ntTime = Vhd::Utils::NtTimeFromVhdTime(0xfffffffe);
    EXPECT_EQUAL(ntTime, 168861256940000000);
    ntTime = Vhd::Utils::NtTimeFromVhdTime(0xffffffff);
    EXPECT_EQUAL(ntTime, 168861256950000000);
}

static
void
TestVhdTimeFromNtTime(void)
{
    uint32_t vhdTime, vhdTime2;

    /* Zero and very small NT time is out of range for VHD, should return 0 */
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(0);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(1);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(100000000);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(10000000000000);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(100000000000000000);
    EXPECT_EQUAL(vhdTime, 0);

    /* Lower bound of VHD time, 2000-01-01 00:00:00 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(125911583999999999);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(125911584000000000);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(125911584009999999);
    EXPECT_EQUAL(vhdTime, 0);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(125911584010000000);
    EXPECT_EQUAL(vhdTime, 1);

    /* NT time counts in 100ns intervals */
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(150000000000000000);
    vhdTime2 = Vhd::Utils::VhdTimeFromNtTime(150000000000000000 + (uint64_t)1000000 * 10000000);
    EXPECT_EQUAL(vhdTime2 - vhdTime, 1000000);

    /* 2016-07-31 12:34:56 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(131144420960000000);
    EXPECT_EQUAL(vhdTime, 523283696);

    /* Upper bound of 32 bit signed unix time, 2038-01-19 03:14:07 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(137919572470000000);
    EXPECT_EQUAL(vhdTime, 1200798847);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(137919572479999999);
    EXPECT_EQUAL(vhdTime, 1200798847);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(137919572480000000);
    EXPECT_EQUAL(vhdTime, 1200798848);

    /* Upper bound of 32 bit unsigned unix time, 2106-07-02 06:28:15 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(159394408950000000);
    EXPECT_EQUAL(vhdTime, 3348282495);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(159394408959999999);
    EXPECT_EQUAL(vhdTime, 3348282495);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(159394408960000000);
    EXPECT_EQUAL(vhdTime, 3348282496);

    /* Upper bound of VHD time, 2136-07-02 06:28:15 UTC */
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(168861256940000000);
    EXPECT_EQUAL(vhdTime, 0xfffffffe);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(168861256950000000);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(168861256950000001);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(168861256960000000);
    EXPECT_EQUAL(vhdTime, 0xffffffff);

    /* Large out of bound numbers should return the max value 0xffffffff */
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(170000000000000000);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(170000000000000000);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(9223372036854775806);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(18446744073709551614u);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
    vhdTime = Vhd::Utils::VhdTimeFromNtTime(18446744073709551615u);
    EXPECT_EQUAL(vhdTime, 0xffffffff);
}

static
void
TestIsUniqueIdEqual(void)
{
    VHD_UNIQUE_ID id1, id2;
    unsigned i;

    memset(&id1, 0, sizeof(id1));
    memset(&id2, 0, sizeof(id2));
    EXPECT(Vhd::Utils::IsUniqueIdEqual(&id1, &id2));
    EXPECT(Vhd::Utils::IsUniqueIdEqual(&id2, &id1));

    memset(&id1, 0, sizeof(id1));
    memset(&id2, 0, sizeof(id2));
    for (i = 0; i < sizeof(id1); i++)
    {
        id1.Id[i] = 0x80;
        EXPECT(!Vhd::Utils::IsUniqueIdEqual(&id1, &id2));
        EXPECT(!Vhd::Utils::IsUniqueIdEqual(&id2, &id1));

        id2.Id[i] = 0x80;
        EXPECT(Vhd::Utils::IsUniqueIdEqual(&id1, &id2));
        EXPECT(Vhd::Utils::IsUniqueIdEqual(&id2, &id1));

        id1.Id[i] = 0;
        EXPECT(!Vhd::Utils::IsUniqueIdEqual(&id1, &id2));
        EXPECT(!Vhd::Utils::IsUniqueIdEqual(&id2, &id1));

        id2.Id[i] = 0;
    }
}

static
void
TestCalculateChecksum(void)
{
    unsigned char buffer[512];
    unsigned i;
    uint32_t checksum;

    for (i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = i & 0xff;
    }

    checksum = Vhd::Utils::CalculateChecksum(buffer, sizeof(buffer), 0);
    EXPECT_EQUAL(checksum, 0xffff00ff);

    checksum = Vhd::Utils::CalculateChecksum(buffer, sizeof(buffer), 0x00010203);
    EXPECT_EQUAL(checksum, 0xffff0105);

    checksum = Vhd::Utils::CalculateChecksum(NULL, 0, 0x00000000);
    EXPECT_EQUAL(checksum, 0xffffffff);

    checksum = Vhd::Utils::CalculateChecksum(buffer, 1, 0x00000000);
    EXPECT_EQUAL(checksum, 0xffffffff);

    checksum = Vhd::Utils::CalculateChecksum(buffer, 2, 0x00000000);
    EXPECT_EQUAL(checksum, 0xfffffffe);

    checksum = Vhd::Utils::CalculateChecksum(buffer, 2, 0x00008000);
    EXPECT_EQUAL(checksum, 0x0000007e);
}

int
main(void)
{
    TestUnixTimeFromVhdTime();
    TestVhdTimeFromUnixTime();
    TestNtTimeFromVhdTime();
    TestVhdTimeFromNtTime();
    TestIsUniqueIdEqual();
    TestCalculateChecksum();

    return Test::FinishTestRun();
}
