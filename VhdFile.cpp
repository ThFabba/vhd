/*
 * Vhd::File class implementation
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
#include "Vhd.hpp"

namespace Vhd
{

void File::GetCreator(uint32_t *Application, uint32_t *Version, uint32_t *Os) const
{
}

uint32_t File::GetCreationTime() const
{
    return 0;
}

DiskType File::GetType() const
{
    return DiskNone;
}

void File::GetUniqueId(struct UniqueId *Id) const
{
}

bool File::IsInSavedState() const
{
    return false;
}

File *File::GetParent() const
{
    return nullptr;
}

} /* namespace Vhd */
