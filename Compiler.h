/*
 * Compiler declarations
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

#if defined(__GNUC__) || defined(__clang__)
#define FORMAT_ATTR(style, fmt_arg, first_arg) __attribute__((__format__(style, fmt_arg, first_arg)))
#else /* defined(__GNUC__) || defined(__clang__) */
#define FORMAT_ATTR(style, fmt_arg, first_arg)
#endif /* defined(__GNUC__) || defined(__clang__) */

#ifndef c_assert
#define c_assert(cond) typedef int c_assert[(cond) ? 1 : -1]
#endif

template<typename TTo, typename TFrom>
inline TTo narrowing_cast(TFrom value)
{
    return static_cast<TTo>(value);
}

template<typename TTo, typename TFrom>
inline TTo widening_cast(TFrom value)
{
    return value;
}

/* FIXME: This is apparently supported in VS2010+? In that case we don't need it here */
#if defined(__cplusplus) && defined(_MSC_VER) && _MSC_VER < 0
const struct nullptr_t
{
    nullptr_t();

    template<typename T>
    operator T *() const
    {
        return 0;
    }

    template<typename T>
    operator const T *() const
    {
        return 0;
    }
} nullptr;
#endif /* defined(__cplusplus) && defined(_MSC_VER) && _MSC_VER < 0 */
