#include "RealmMgr.h"



RealmMgr::RealmMgr()
{
}


RealmMgr::~RealmMgr()
{
}

RealmMgr* RealmMgr::instance()
{
    static RealmMgr instance;
    return &instance;
}

bool RealmMgr::AddRealm(Realm realm)
{
    realms.push_back(std::move(realm));
    return true;
}
