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

#include "DatabaseLoader.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "Log.h"

#include <mysqld_error.h>

DatabaseLoader::DatabaseLoader(std::string const& logger, uint32 const defaultUpdateMask)
    : _logger(logger)
{
}

template <class T>
DatabaseLoader& DatabaseLoader::AddDatabase(DatabaseWorkerPool<T>& pool, std::string const& name)
{
    bool updatesEnabledForThis = false;
    _open.push([this, name, &pool]() -> bool
    {
        std::string const dbString = sConfigMgr->GetStringDefault(name + "DatabaseInfo", "");
        if (dbString.empty())
        {
            WAR_LOG_ERROR(_logger, "Database %s not specified in configuration file!", name.c_str());
            return false;
        }

        uint8 const asyncThreads = uint8(sConfigMgr->GetIntDefault(name + "Database.WorkerThreads", 1));
        if (asyncThreads < 1 || asyncThreads > 32)
        {
            WAR_LOG_ERROR(_logger, "%s database: invalid number of worker threads specified. "
                "Please pick a value between 1 and 32.", name.c_str());
            return false;
        }

        uint8 const synchThreads = uint8(sConfigMgr->GetIntDefault(name + "Database.SynchThreads", 1));

        pool.SetConnectionInfo(dbString, asyncThreads, synchThreads);
        if (uint32 error = pool.Open())
        {
            // If the error wasn't handled quit
            if (error)
            {
                WAR_LOG_ERROR("sql.driver", "\nDatabasePool %s NOT opened. There were errors opening the MySQL connections. Check your SQLDriverLogFile "
                    "for specific errors. Read wiki at https://www.trinitycore.info/display/tc/TrinityCore+Home", name.c_str());

                return false;
            }
        }
        // Add the close operation
        _close.push([&pool]
        {
            pool.Close();
        });
        return true;
    });

    _prepare.push([this, name, &pool]() -> bool
    {
        if (!pool.PrepareStatements())
        {
            WAR_LOG_ERROR(_logger, "Could not prepare statements of the %s database, see log for details.", name.c_str());
            return false;
        }
        return true;
    });

    return *this;
}

bool DatabaseLoader::Load()
{
    if (!OpenDatabases())
        return false;

    if (!PrepareStatements())
        return false;

    return true;
}

bool DatabaseLoader::OpenDatabases()
{
    return Process(_open);
}

bool DatabaseLoader::PrepareStatements()
{
    return Process(_prepare);
}

bool DatabaseLoader::Process(std::queue<Predicate>& queue)
{
    while (!queue.empty())
    {
        if (!queue.front()())
        {
            // Close all open databases which have a registered close operation
            while (!_close.empty())
            {
                _close.top()();
                _close.pop();
            }

            return false;
        }

        queue.pop();
    }
    return true;
}

template WAR_DATABASE_API
DatabaseLoader& DatabaseLoader::AddDatabase<LoginDatabaseConnection>(DatabaseWorkerPool<LoginDatabaseConnection>&, std::string const&);
template WAR_DATABASE_API
DatabaseLoader& DatabaseLoader::AddDatabase<CharacterDatabaseConnection>(DatabaseWorkerPool<CharacterDatabaseConnection>&, std::string const&);
template WAR_DATABASE_API
DatabaseLoader& DatabaseLoader::AddDatabase<HotfixDatabaseConnection>(DatabaseWorkerPool<HotfixDatabaseConnection>&, std::string const&);
template WAR_DATABASE_API
DatabaseLoader& DatabaseLoader::AddDatabase<WorldDatabaseConnection>(DatabaseWorkerPool<WorldDatabaseConnection>&, std::string const&);
