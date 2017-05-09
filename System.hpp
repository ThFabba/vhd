/*
 * System namespace declarations
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
#include "Error.hpp"
#include <cstdarg>
#include <cstdint>

#define Assert(cond) ((cond) ? (void)0 : System::AssertMessage(#cond, __FILE__, __LINE__))

namespace System
{

void AssertMessage(const char *Message, const char *File, int Line);

/*
 * Provides support for file I/O
 */
class File
{
public:
    union Handle
    {
        int Fd;
        void *P;
    };

    enum OpenFlags
    {
        OpenForRead = 0,
        OpenForWrite = 1,
        OpenCreate = 2
    };

    enum SeekOrigins
    {
        SeekSet,
        SeekCur,
        SeekEnd
    };

private:
    Handle m_Handle;

public:
    File();
    ~File()
    {
        if (Valid())
        {
            Close();
        }
    }

    bool Valid() const;

    Error Open(const char *Path, OpenFlags Flags = OpenForRead);
    void Close();

    Error GetSize(uint64_t *Size) const;
    Error Seek(uint64_t Position, SeekOrigins Origin = SeekSet) const;
    Error Tell(uint64_t *Position) const;

    Error Read(void *Buffer, unsigned Length, unsigned *BytesRead) const;
    Error Write(const void *Buffer, unsigned Length, unsigned *BytesWritten) const;

    Handle GetHandle() const;
};

/*
 * Represents the OS's logging facility
 */
class Logging
{
    Logging();

public:
    static void Log(const char *Format, ...)
        FORMAT_ATTR(printf, 1, 2)
    {
        va_list Args;

        va_start(Args, Format);
        LogV(Format, Args);
        va_end(Args);
    }
    static void LogV(const char *Format, va_list Args);
};

} /* namespace System */
