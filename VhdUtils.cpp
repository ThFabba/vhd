/*
 * Vhd::Utils class implementation
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
#include <cstring>

#include "Vhd.hpp"

namespace Vhd
{
#define VHD_TIME_MIN 0
#define VHD_TIME_MAX 0xffffffff

#define UNIX_TIME_JAN_1ST_2000 946684800

#define NT_TIME_JAN_1ST_2000 125911584000000000

int64_t Utils::UnixTimeFromVhdTime(uint32_t VhdTime)
{
    return widening_cast<int64_t>(VhdTime) + UNIX_TIME_JAN_1ST_2000;
}

uint32_t Utils::VhdTimeFromUnixTime(int64_t UnixTime)
{
    int64_t vhdTime;

    if (UnixTime < UNIX_TIME_JAN_1ST_2000)
    {
        return 0;
    }

    vhdTime = UnixTime - UNIX_TIME_JAN_1ST_2000;
    if (vhdTime > VHD_TIME_MAX)
    {
        return VHD_TIME_MAX;
    }

    return narrowing_cast<uint32_t>(vhdTime);
}

uint64_t Utils::NtTimeFromVhdTime(uint32_t VhdTime)
{
    return widening_cast<uint64_t>(VhdTime) * 10000000 + NT_TIME_JAN_1ST_2000;
}

uint32_t Utils::VhdTimeFromNtTime(uint64_t NtTime)
{
    uint64_t vhdTime;

    if (NtTime < NT_TIME_JAN_1ST_2000)
    {
        return 0;
    }

    vhdTime = (NtTime - NT_TIME_JAN_1ST_2000) / 10000000;
    if (vhdTime > VHD_TIME_MAX)
    {
        return VHD_TIME_MAX;
    }

    return narrowing_cast<uint32_t>(vhdTime);
}

bool Utils::IsUniqueIdEqual(const VHD_UNIQUE_ID *Id1, const VHD_UNIQUE_ID *Id2)
{
    return !std::memcmp(Id1, Id2, sizeof(*Id1));
}

uint32_t Utils::CalculateChecksum(const void *Header, size_t HeaderSize, uint32_t HeaderChecksum)
{
    const unsigned char *buffer;
    size_t i;
    uint32_t checksum;

    buffer = static_cast<const unsigned char *>(Header);
    checksum = 0;

    for (i = 0; i < HeaderSize; i++)
    {
        checksum += buffer[i];
    }

    checksum -= (HeaderChecksum >>  0) & 0xff;
    checksum -= (HeaderChecksum >>  8) & 0xff;
    checksum -= (HeaderChecksum >> 16) & 0xff;
    checksum -= (HeaderChecksum >> 24) & 0xff;

    return ~checksum;
}

} /* namespace Vhd */
