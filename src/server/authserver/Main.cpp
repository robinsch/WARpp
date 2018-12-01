#include "AuthServer.h"
#include "RealmMgr.h"

#include <iostream>

int main()
{
    std::cout << "Starting WARpp Authentication Server...\n";
    AuthServer auth(("127.0.0.1", 18046));

    std::cout << "Initializing Opcodes...\n";
    opcodeTable.Initialize();
    std::cout << "Initialized Opcodes.\n";

    Realm realm;
    realm.Id = 1;
    realm.Name = "Ascension";
    realm.Host = "127.0.0.1";
    realm.Port = 18047;
    realm.MaxPopulation = 500;
    realm.Population = 0;
    realm.Status = RealmStatus::Online;
    realm.Language = RealmLanguage::English;

    sRealmMgr->AddRealm(realm);

    std::cout << "Server started!\n";
    auth.Run();
}