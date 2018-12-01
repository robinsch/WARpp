#include "AuthServer.h"
#include "Config.h"
#include "DatabaseEnv.h"
#include "DatabaseLoader.h"
#include "GitRevision.h"
#include "Log.h"
#include "MySQLThreading.h"
#include "RealmMgr.h"
#include "Timer.h"

#include <boost/filesystem/operations.hpp>
#include <iostream>

namespace fs = boost::filesystem;

#ifndef _WAR_AUTH_CONFIG
#define _WAR_AUTH_CONFIG "authserver.conf"
#endif

bool StartDB();
void StopDB();

/// Initialize connection to the database
bool StartDB()
{
    MySQL::Library_Init();

    // Load databases
    DatabaseLoader loader("server.authserver", DatabaseLoader::DATABASE_NONE);
    loader
        .AddDatabase(LoginDatabase, "Login");

    if (!loader.Load())
        return false;

    WAR_LOG_INFO("server.authserver", "Started auth database connection pool.");
    sLog->SetRealmId(0); // Enables DB appenders when realm is set.
    return true;
}

int main(int argc, char** argv)
{
    auto configFile = fs::absolute(_WAR_AUTH_CONFIG);
    std::string configService;

#if WAR_PLATFORM == WAR_PLATFORM_WINDOWS
    //if (configService.compare("install") == 0)
    //    return WinServiceInstall() ? 0 : 1;
    //else if (configService.compare("uninstall") == 0)
    //    return WinServiceUninstall() ? 0 : 1;
    //else if (configService.compare("run") == 0)
    //    return WinServiceRun() ? 0 : 1;
#endif

    std::string configError;
    if (!sConfigMgr->LoadInitial(configFile.generic_string(),
        std::vector<std::string>(argv, argv + argc),
        configError))
    {
        printf("Error in config file: %s\n", configError.c_str());
        return 1;
    }

    sLog->Initialize(nullptr);

    // Initialize the database connection
    if (!StartDB())
        return 1;

    ///- Server startup begin
    uint32 startupBegin = getMSTime();

    WAR_LOG_INFO("server.loading", "Starting WARpp Authentication Server (Rev: %s)", GitRevision::GetFullVersion());
    AuthServer auth(("127.0.0.1", 18046));

    WAR_LOG_INFO("server.loading", "Initializing Opcodes...");
    opcodeTable.Initialize();

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_REALMLIST);
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    WAR_LOG_INFO("server.loading", "Loading realmlist...");
    if (!result)
    {
        WAR_LOG_INFO("server.loading", ">> Loaded 0 realms. DB table `realmlist` is empty.");
        return 0;
    }

    do
    {
        Field* fields = result->Fetch();

        Realm realm;
        realm.Id            = fields[0].GetUInt32();
        realm.Name          = fields[1].GetString();
        realm.Host          = fields[2].GetString();
        realm.Port          = fields[3].GetUInt16();
        realm.MaxPopulation = fields[4].GetUInt32();
        realm.Population    = fields[5].GetUInt8();
        realm.Status        = RealmStatus(fields[6].GetUInt8());
        realm.Language      = RealmLanguage(fields[7].GetUInt8());

        sRealmMgr->AddRealm(realm);
    } while (result->NextRow());

    uint32 startupDuration = GetMSTimeDiffToNow(startupBegin);

    WAR_LOG_INFO("server.loading", "Authentication Server initialized in %u minutes %u seconds", (startupDuration / 60000), ((startupDuration % 60000) / 1000));
    printf("\a");                                       // \a = Alert

    auth.Run();
}