/*
 * System namespace implementation using standard C++
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

namespace System
{

Error
ErrorFromErrno(
    int Errno)
{
    switch (Errno)
    {
        case 0:
            return ErrorOk;
        case EDOM:
        case ERANGE:
        case EINVAL:
            return ErrorInvalidParameter;
        case ENOMEM:
            return ErrorInsufficientResources;
        case EPERM:
        case EACCES:
            return ErrorAccessDenied;
        case ENOENT:
            return ErrorFileNotFound;
        case EEXIST:
            return ErrorFileExists;
        case ENOTDIR:
            return ErrorNotADirectory;
        case EISDIR:
            return ErrorIsADirectory;
        case ENOSPC:
            return ErrorNoSpace;

        default:
            Logging::Log("[ErrorForErrno] no mapping for errno %d", Errno);
            return ErrorUnknown;
    }
}

} /* namespace System */
