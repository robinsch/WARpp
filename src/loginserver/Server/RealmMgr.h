#ifndef RealmMgr_H
#define RealmMgr_H

#include "Define.h"

#include <unordered_map>

enum class RealmLanguage : uint8
{
    Unknown             = 0,
    English             = 1,
    French              = 2,
    German              = 3,
    Italian             = 4,
    Spanish             = 5,
    Korean              = 6,
    Chinese             = 7,
    ChineseTraditional  = 8,
    Japanese            = 9,
    Russian             = 10
};

enum class RealmStatus : uint8
{
    Online              = 1,
    Offline             = 2
};

struct Realm
{
    uint8 Id;
    std::string Name;
    std::string Host;
    uint32 Port;
    RealmLanguage Language;
    uint32 MaxPopulation;
    uint8 Population;
    RealmStatus Status;
};

using RealmVec = std::vector<Realm>;

class RealmMgr
{
private:
    RealmMgr();
    ~RealmMgr();

public:
    static RealmMgr* instance();
    bool AddRealm(Realm realm);

    RealmVec& GetRealms() { return realms; }

private:
    RealmVec realms;
};

#define sRealmMgr RealmMgr::instance()

#endif

