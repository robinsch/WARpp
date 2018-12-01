#include "Session.h"
#include "RealmMgr.h"

void Session::HandleVerifyProtocol(WorldPackets::Login::LoginVerifyProtocol& loginQuery)
{
    std::cout << "Received: CMSG_VERIFY_PROTOCOL" << '\n';

    WorldPackets::Login::LoginVerifyProtocolReply packet;
    char result[] = { 38, 2, 8, 0, 18, 16, 1, 83, 33, 77, 74, 4, 39, 183, 180, 89, 15, 62, 167, 157, 41, 233, 26, 16, 73, 24, 161, 42, 100, 225, 218, 189, 132, 217, 244, 138, 139, 60, 39, 32 };

    boost::asio::write(_socket, boost::asio::buffer(result));
}

void Session::HandleAuthSessionToken(WorldPackets::Login::AuthSessionToken& token)
{
    std::cout << "Received: CMSG_AUTH_SESSION_TOKEN" << '\n';

    WorldPackets::Login::AuthSessionTokenReply packet;
    char result[] = { 2, 6, 8, 0 };

    boost::asio::write(_socket, boost::asio::buffer(result));
}

void Session::HandleCharSummaryList(WorldPackets::Login::CharSummaryList& token)
{
    std::cout << "Received: CMSG_CHAR_SUMMARY_LIST" << '\n';

    WorldPackets::Login::CharSummaryListReply packet;
    char result[] = { 2, 8, 8, 0 };

    boost::asio::write(_socket, boost::asio::buffer(result));
}

void Session::HandleClusterList(WorldPackets::Login::ClusterList& list)
{
    std::cout << "Received: CMSG_CLUSTER_LIST" << '\n';

    WorldPackets::Login::ClusterListReply packet;
    packet.Id = 1;
    packet.Name = "Ascension";

    for (Realm const& realm : sRealmMgr->GetRealms())
    {
        packet.Id = 1;
        packet.Host = realm.Host;
        packet.Port = realm.Port;
        packet.Language = uint8(realm.Language);
        packet.MaxPopulation = realm.MaxPopulation;
        packet.Population = realm.Population;
        packet.PopulationStatus = 0;
        packet.Status = uint8(realm.Status);
        packet.ServerId = realm.Id;
        packet.ServerName = realm.Name;
    }

    SendPacket(packet.Write());
}

void Session::HandleAccPropList(WorldPackets::Login::AccPropList& list)
{
    std::cout << "Client request account property list" << '\n';

    WorldPackets::Login::AccPropListReply packet;
    char result[] = { 2, 12, 8, 0 };

    boost::asio::write(_socket, boost::asio::buffer(result));
}
