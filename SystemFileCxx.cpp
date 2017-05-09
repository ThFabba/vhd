/*
 * System::File class implementation using standard C++
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
#include "SystemCxx.hpp"
#include <cerrno>
#include <cstdio>

namespace System
{

static File::Handle HandleFromFILE(std::FILE *P)
{
    File::Handle h;
    h.P = P;
    return h;
}

static FILE *FILEFromHandle(File::Handle H)
{
    return static_cast<FILE *>(H.P);
}

File::File() :
    m_Handle(HandleFromFILE(nullptr))
{
}

bool
File::Valid() const
{
    return m_Handle.P != nullptr;
}

Error
File::Open(
    const char *Path,
    OpenFlags Flags)
{
    const char *mode;

    if (Valid())
    {
        Close();
    }

    Assert(!Valid());

    if (Flags & OpenForWrite)
    {
        if (Flags & OpenCreate)
        {
            mode = "wb+";
        }
        else
        {
            mode = "rb+";
        }
    }
    else
    {
        if (Flags & OpenCreate)
        {
            Assert(!( (Flags & OpenCreate) && !(Flags & OpenForWrite)) );
            mode = "wb+";
        }
        else
        {
            mode = "rb";
        }
    }

    m_Handle.P = std::fopen(Path, mode);
    if (m_Handle.P == nullptr)
    {
        Assert(!Valid());
        return ErrorFromErrno(errno);
    }

    Assert(Valid());
    return ErrorOk;
}

void
File::Close()
{
    Assert(Valid());
    if (m_Handle.P)
    {
        std::fclose(FILEFromHandle(m_Handle));
    }
    m_Handle.P = nullptr;
    Assert(!Valid());
}

Error
File::GetSize(
    uint64_t *Size) const
{
    Error error;
    uint64_t oldPos;
    uint64_t fileSize;

    *Size = 0;

    if (!Valid())
    {
        Assert(Valid());
        return ErrorInvalidOperation;
    }

    error = Tell(&oldPos);
    if (error)
    {
        return error;
    }

    error = Seek(0, SeekEnd);
    if (error)
    {
        return error;
    }

    error = Tell(&fileSize);
    (void)Seek(oldPos, SeekSet);
    if (error)
    {
        return error;
    }

    *Size = fileSize;
    return ErrorOk;
}

Error
File::Seek(
    uint64_t Position,
    SeekOrigins Origin) const
{
    int ret;
    int origin;

    if (!Valid())
    {
        Assert(Valid());
        return ErrorInvalidOperation;
    }

    switch (Origin)
    {
        case SeekSet:
            origin = SEEK_SET;
            break;
        case SeekCur:
            origin = SEEK_CUR;
            break;
        case SeekEnd:
            origin = SEEK_END;
            break;
        default:
            Assert(Origin == SeekSet || Origin == SeekCur || Origin == SeekEnd);
            return ErrorInvalidParameter;
    }

#if defined(_WIN32)
    ret = ::_fseeki64(FILEFromHandle(m_Handle), static_cast<int64_t>(Position), origin);
#else /* defined(_WIN32) */
    if (Position > LONG_MAX)
    {
        return ErrorInvalidParameter;
    }

    ret = std::fseek(FILEFromHandle(m_Handle), Position, origin);
#endif /* defined(_WIN32) */
    if (ret != 0)
    {
        return ErrorUnknown;
    }

    return ErrorOk;
}

Error
File::Tell(
    uint64_t *Position) const
{
    int64_t pos;

    *Position = 0;

    if (!Valid())
    {
        Assert(Valid());
        return ErrorInvalidOperation;
    }

#ifdef _WIN32
    pos = ::_ftelli64(FILEFromHandle(m_Handle));
#else
    pos = std::ftell(FILEFromHandle(m_Handle));
#endif
    if (pos < 0)
    {
        return ErrorUnknown;
    }

    *Position = static_cast<uint64_t>(pos);
    return ErrorOk;
}

Error
File::Read(
    void *Buffer,
    unsigned Length,
    unsigned *BytesRead) const
{
    size_t bytes;

    *BytesRead = 0;

    if (!Valid())
    {
        Assert(Valid());
        return ErrorInvalidOperation;
    }

    bytes = fread(Buffer, 1, Length, FILEFromHandle(m_Handle));
    if (bytes != Length)
    {
        Assert(bytes < Length);
        if (std::ferror(FILEFromHandle(m_Handle)))
        {
            return ErrorUnknown;
        }
    }

    *BytesRead = bytes;
    return ErrorOk;
}

Error
File::Write(
    const void *Buffer,
    unsigned Length,
    unsigned *BytesWritten) const
{
    size_t bytes;

    *BytesWritten = 0;

    if (!Valid())
    {
        Assert(Valid());
        return ErrorInvalidOperation;
    }

    bytes = fwrite(Buffer, 1, Length, FILEFromHandle(m_Handle));
    if (bytes != Length)
    {
        Assert(bytes < Length);
        if (std::ferror(FILEFromHandle(m_Handle)))
        {
            return ErrorUnknown;
        }
    }

    *BytesWritten = bytes;
    return ErrorOk;
}

File::Handle
File::GetHandle(void) const
{
    Assert(Valid());
    return m_Handle;
}


} /* namespace System */
