/*
 * System::File class tests
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
#include "System.hpp"
#include "VhdData.h"
#include <cstring>

static
void
TestOpen(void)
{
    System::File::Handle handle;
    Error error;

    {
        System::File f;

        /* GetHandle for invalid */
        EXPECT_EQUAL(f.Valid(), false);
        Test::ExpectedAsserts = 1;
        handle = f.GetHandle();
        EXPECT_EQUAL(Test::ExpectedAsserts, 0);
        EXPECT_EQUAL(handle.P, NULL);

        /* Close without open */
        Test::ExpectedAsserts = 1;
        f.Close();
        EXPECT_EQUAL(Test::ExpectedAsserts, 0);
        EXPECT_EQUAL(handle.P, NULL);

        /* GetHandle after Close */
        EXPECT_EQUAL(f.Valid(), false);
        Test::ExpectedAsserts = 1;
        handle = f.GetHandle();
        EXPECT_EQUAL(Test::ExpectedAsserts, 0);
        EXPECT_EQUAL(handle.P, NULL);

        /* Open with a nonexistent file */
        error = f.Open("DoesNotExist.vhd");
        EXPECT_EQUAL(error, ErrorFileNotFound);

        /* GetHandle after failed Open */
        EXPECT_EQUAL(f.Valid(), false);
        Test::ExpectedAsserts = 1;
        handle = f.GetHandle();
        EXPECT_EQUAL(Test::ExpectedAsserts, 0);
        EXPECT_EQUAL(handle.P, NULL);

        /* Open with a real file */
        error = f.Open("EmptyFixed3MB.vhd");
        EXPECT_EQUAL(error, ErrorOk);
    }
}

static
void
TestPosition(void)
{
    System::File f;
    Error error;
    uint64_t offset;
    uint64_t size;
    uint64_t i;

    /* Tell on invalid file */
    offset = 0x5555555555555555;
    Test::ExpectedAsserts = 1;
    error = f.Tell(&offset);
    EXPECT_EQUAL(Test::ExpectedAsserts, 0);
    EXPECT_EQUAL(error, ErrorInvalidOperation);
    EXPECT_EQUAL(offset, 0);

    /* Seek on invalid file */
    Test::ExpectedAsserts = 1;
    error = f.Seek(0);
    EXPECT_EQUAL(Test::ExpectedAsserts, 0);
    EXPECT_EQUAL(error, ErrorInvalidOperation);

    /* GetSize on invalid file */
    size = 0x5555555555555555;
    Test::ExpectedAsserts = 1;
    error = f.GetSize(&size);
    EXPECT_EQUAL(Test::ExpectedAsserts, 0);
    EXPECT_EQUAL(error, ErrorInvalidOperation);
    EXPECT_EQUAL(size, 0);

    /* Open */
    error = f.Open("EmptyFixed3MB.vhd");
    EXPECT_EQUAL(error, ErrorOk);

    /* Initial file position should be 0 */
    offset = 0x5555555555555555;
    error = f.Tell(&offset);
    EXPECT_EQUAL(error, ErrorOk);
    EXPECT_EQUAL(offset, 0);

    /* Size of the file */
    size = 0x5555555555555555;
    error = f.GetSize(&size);
    EXPECT_EQUAL(error, ErrorOk);
    EXPECT_EQUAL(size, 3146240);

    for (i = 0; i < size; i += 127)
    {
        /* Seek to current offset */
        error = f.Seek(i);
        EXPECT_EQUAL(error, ErrorOk);

        /* Size should be unchanged */
        size = 0x5555555555555555;
        error = f.GetSize(&size);
        EXPECT_EQUAL(error, ErrorOk);
        EXPECT_EQUAL(size, 3146240);

        /* Position should be the same */
        offset = 0x5555555555555555;
        error = f.Tell(&offset);
        EXPECT_EQUAL(error, ErrorOk);
        EXPECT_EQUAL(offset, i);
    }
}

static
void
TestRead(void)
{
    System::File f;
    Error error;
    uint64_t offset, offset2;
    uint64_t size;
    unsigned i;
    unsigned char buffer[1024];
    unsigned bytesRead;
    bool isZero;
    VHD_FOOTER *footer;

    /* Open */
    error = f.Open("EmptyFixed3MB.vhd");
    EXPECT_EQUAL(error, ErrorOk);

    /* Get size */
    size = 0x5555555555555555;
    error = f.GetSize(&size);
    EXPECT_EQUAL(error, ErrorOk);
    EXPECT_EQUAL(size, 3146240);

    for (offset = 0; offset < size - 512; offset += sizeof(buffer))
    {
        /* Seek to current offset */
        error = f.Seek(offset);
        EXPECT_EQUAL(error, ErrorOk);

        /* Read a block */
        memset(buffer, 0x55, sizeof(buffer));
        bytesRead = 0x55555555;
        error = f.Read(buffer, sizeof(buffer), &bytesRead);
        EXPECT_EQUAL(error, ErrorOk);
        EXPECT_EQUAL(bytesRead, sizeof(buffer));

        isZero = true;
        for (i = 0; i < sizeof(buffer); i++)
        {
            isZero = isZero && buffer[i] == 0;
        }
        EXPECT(isZero);

        /* Position should be the same */
        offset = 0x5555555555555555;
        error = f.Tell(&offset2);
        EXPECT_EQUAL(error, ErrorOk);
        EXPECT_EQUAL(offset2, offset);
    }

    /* Read a block */
    memset(buffer, 0x55, sizeof(buffer));
    bytesRead = 0x55555555;
    error = f.Read(buffer, sizeof(buffer), &bytesRead);
    EXPECT_EQUAL(error, ErrorOk);
    EXPECT_EQUAL(bytesRead, sizeof(VHD_FOOTER));

    footer = (VHD_FOOTER *)buffer;
    EXPECT_EQUAL(footer->Cookie, VHD_FOOTER_COOKIE);
    EXPECT_EQUAL(footer->Features, VHD_FEATURE_RESERVED1);
    EXPECT_EQUAL(footer->FileFormatVersion, VHD_FILE_FORMAT_VERSION);
    EXPECT_EQUAL(footer->DataOffset, 0xffffffffffffffff);
    EXPECT_EQUAL(footer->TimeStamp, 0xffffffffffffffff);
    isZero = true;
    for (i = 0; i < sizeof(footer->Reserved1); i++)
    {
        isZero = isZero && footer->Reserved1[i] == 0;
    }
    EXPECT(isZero);

    /* Position should be the same */
    offset = 0x5555555555555555;
    error = f.Tell(&offset);
    EXPECT_EQUAL(error, ErrorOk);
    EXPECT_EQUAL(offset, size);
}

int
main(void)
{
    TestOpen();
    TestPosition();

    return Test::FinishTestRun();
}
