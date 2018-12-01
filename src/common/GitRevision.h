/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GITREVISION_H__
#define __GITREVISION_H__

#include "Define.h"

namespace GitRevision
{
    WAR_COMMON_API char const* GetHash();
    WAR_COMMON_API char const* GetDate();
    WAR_COMMON_API char const* GetBranch();
    WAR_COMMON_API char const* GetCMakeCommand();
    WAR_COMMON_API char const* GetCMakeVersion();
    WAR_COMMON_API char const* GetHostOSVersion();
    WAR_COMMON_API char const* GetBuildDirectory();
    WAR_COMMON_API char const* GetSourceDirectory();
    WAR_COMMON_API char const* GetMySQLExecutable();
    WAR_COMMON_API char const* GetFullDatabase();
    WAR_COMMON_API char const* GetHotfixesDatabase();
    WAR_COMMON_API char const* GetFullVersion();
    WAR_COMMON_API char const* GetCompanyNameStr();
    WAR_COMMON_API char const* GetLegalCopyrightStr();
    WAR_COMMON_API char const* GetFileVersionStr();
    WAR_COMMON_API char const* GetProductVersionStr();
}

#endif
