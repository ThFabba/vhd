/*
 * Vhd namespace declarations
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

#include "VhdData.h"
#include "System.hpp"

namespace Vhd
{

enum DiskType
{
    DiskNone = 0,
    DiskFixed = 2,
    DiskDynamic = 3,
    DiskDifferencing = 4
};

class Utils
{
    Utils();

public:
    static inline uint64_t HostFromNetwork(uint64_t FileValue);
    static inline uint32_t HostFromNetwork(uint32_t FileValue);
    static inline uint16_t HostFromNetwork(uint16_t FileValue);

    static int64_t UnixTimeFromVhdTime(uint32_t VhdTime);
    static uint32_t VhdTimeFromUnixTime(int64_t UnixTime);
    static uint64_t NtTimeFromVhdTime(uint32_t VhdTime);
    static uint32_t VhdTimeFromNtTime(uint64_t NtTime);

    static bool IsUniqueIdEqual(const VHD_UNIQUE_ID *Id1, const VHD_UNIQUE_ID *Id2);

    static uint32_t CalculateChecksum(const void *Header, size_t HeaderSize, uint32_t HeaderChecksum);
};

/*
 * Represents
 */
class File
{
    System::File m_File;
    VHD_FOOTER m_Footer;
    VHD_DYNAMIC_DISK_HEADER *m_DynamicHeader;



public:
    void GetCreator(uint32_t *Application, uint32_t *Version, uint32_t *Os) const;
    uint32_t GetCreationTime() const;
    DiskType GetType() const;
    void GetUniqueId(struct UniqueId *Id) const;
    bool IsInSavedState() const;

    const VHD_FOOTER *GetFooter() const;
    const VHD_DYNAMIC_DISK_HEADER *GetDynamicDiskHeader() const;

    File *GetParent() const;

    friend class Disk;
};


/*
 *
 */
class Disk
{
    File m_File;

    Disk();
    Error Initialize();

public:

    uint64_t GetSize() const;
    uint64_t GetOriginalSize() const;
    void GetGeometry(uint16_t *Cydlinders, uint8_t *Heads, uint8_t *Sectors) const;

    File *GetFile() const;
#if 0 // use GetFile to access these?
    DiskType GetType() const;
    void GetUniqueId(struct UniqueId *Id) const;
#endif

    Error Read(void *Buffer, unsigned Length, unsigned *BytesRead) const;
    //Error Write(const void *Buffer, unsigned Length, unsigned *BytesWritten) const;

    static Error Create(const char *Path);
};

} /* namespace Vhd */
