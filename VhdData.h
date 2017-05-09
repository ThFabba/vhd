/*
 * VHD on-disk data declarations
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

#include "Compiler.h"
#include <stdint.h>

/*
 * VHD Footer
 */

/* Cookie */
#define VHD_FOOTER_COOKIE ('cone' | (uint64_t)'ctix' << 32)

/* Features */
#define VHD_FEATURE_TEMPORARY 0x01
#define VHD_FEATURE_RESERVED1 0x02

/* FileFormatVersion */
#define VHD_FILE_FORMAT_VERSION 0x00010000

/* CreatorApplication */
#define VHD_CREATOR_APPLICATION_VPC 'vpc '
#define VHD_CREATOR_APPLICATION_VIRUTAL_SERVER 'vs '
#define VHD_CREATOR_APPLICATION_WINDOWS 'win '
#define VHD_CREATOR_APPLICATION_VBOX 'vbox'

/* CreatorHostOs */
#define VHD_CREATOR_HOST_OS_WINDOWS 'Wi2k'
#define VHD_CREATOR_HOST_O_SMAC 'Mac '

/* DiskType */
#define VHD_DISK_TYPE_FIXED 2
#define VHD_DISK_TYPE_DYNAMIC 3
#define VHD_DISK_TYPE_DIFFERENCING 4

typedef struct _VHD_UNIQUE_ID
{
    unsigned char Id[16];
} VHD_UNIQUE_ID, *PVHD_UNIQUE_ID;

typedef union _VHD_DISK_GEOMETRY
{
    struct
    {
        uint16_t Cylinders;
        uint8_t Heads;
        uint8_t SectorsPerTrack;
    } s;
    uint32_t AsLong;
} VHD_DISK_GEOMETRY, *PVHD_DISK_GEOMETRY;

typedef struct _VHD_FOOTER
{
    uint64_t Cookie;
    uint32_t Features;
    uint32_t FileFormatVersion;
    uint64_t DataOffset;
    uint32_t TimeStamp;
    uint32_t CreatorApplication;
    uint32_t CreatorVersion;
    uint32_t CreatorHostOs;
    uint64_t OriginalSize;
    uint64_t CurrentSize;
    VHD_DISK_GEOMETRY DiskGeometry;
    uint32_t DiskType;
    uint32_t Checksum;
    VHD_UNIQUE_ID UniqueId;
    uint8_t SavedState;
    uint8_t Reserved1[427];
} VHD_FOOTER, *PVHD_FOOTER;
c_assert(sizeof(VHD_FOOTER) == 512);

/*
 * Dynamic Disk Header
 */

/* Cookie */
#define VHD_DYNAMIC_DISK_HEADER_COOKIE 'cxsparse'

/* HeaderVersion */
#define VHD_DYNAMIC_DISK_HEADER_VERSION 0x00010000

/* PlatformCode */
#define VHD_PLATFORM_CODE_WIN_RELATIVE 'W2ru'
#define VHD_PLATFORM_CODE_WIN_ABSOLUTE 'W2ku'
#define VHD_PLATFORM_CODE_MAC 'Mac '
#define VHD_PLATFORM_CODE_MACX 'MacX'

typedef struct _VHD_PARENT_LOCATOR_ENTRY
{
    uint32_t PlatformCode;
    uint32_t PlatformDataSpace;
    uint32_t PlatformDataLength;
    uint32_t Reserved1;
    uint64_t PlatformDataOffset;
} VHD_PARENT_LOCATOR_ENTRY, *PVHD_PARENT_LOCATOR_ENTRY;
c_assert(sizeof(VHD_PARENT_LOCATOR_ENTRY) == 24);

typedef struct _VHD_DYNAMIC_DISK_HEADER
{
    uint64_t Cookie;
    uint64_t DataOffset;
    uint64_t TableOffset;
    uint32_t HeaderVersion;
    uint32_t MaxTableEntries;
    uint32_t BlockSize;
    uint32_t Checksum;
    VHD_UNIQUE_ID ParentUniqueId;
    uint32_t ParentTimeStamp;
    uint32_t Reserved1;
    uint16_t ParentUnicodeName[256];
    VHD_PARENT_LOCATOR_ENTRY ParentLocatorEntry[8];
    uint8_t Reserved[256];
} VHD_DYNAMIC_DISK_HEADER, *PVHD_DYNAMIC_DISK_HEADER;
c_assert(sizeof(VHD_DYNAMIC_DISK_HEADER) == 1024);
