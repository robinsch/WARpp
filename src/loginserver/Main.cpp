#include "Server.h"
#include "RealmMgr.h"

int main()
{
    Server server{ "127.0.0.1", 18046 };

    std::cout << "Initializing Opcodes...\n";
    opcodeTable.Initialize();

    Realm realm;
    realm.Id = 1;
    realm.Name = "Ascension";
    realm.Host = "127.0.0.1";
    realm.Port = 18046;
    realm.MaxPopulation = 55;
    realm.Population = 0;
    realm.Status = RealmStatus::Offline;
    realm.Language = RealmLanguage::English;

    std::cout << "Register realm " << realm.Name << "\n";
    sRealmMgr->AddRealm(realm);

    std::cout << "Server started on Port: " << 18046 << "\n";
    server.Run();
}