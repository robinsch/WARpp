#include "Session.h"
#include "RealmMgr.h"
#include "Log.h"

void WorldPackets::Login::LoginVerifyProtocol::Read()
{
    _worldPacket >> ProtocolVersion;
    _worldPacket >> ProductId;
    _worldPacket >> PublicKey;
}

void Session::HandleVerifyProtocol(WorldPackets::Login::LoginVerifyProtocol& data)
{
    WAR_LOG_INFO("auth.opcode", "Recieved: CMSG_VERIFY_PROTOCOL");
    WorldPackets::Login::LoginVerifyProtocolReply packet;
    WAR_LOG_INFO("auth.opcode", "ProtocolVersion: %u", data.ProtocolVersion);
    WAR_LOG_INFO("auth.opcode", "ProductId: %u", data.ProductId);
    WAR_LOG_INFO("auth.opcode", "PublicKey: %u", data.PublicKey);

    char result[] = { 38, 2, 8, 0, 18, 16, 1, 83, 33, 77, 74, 4, 39, 183, 180, 89, 15, 62, 167, 157, 41, 233, 26, 16, 73, 24, 161, 42, 100, 225, 218, 189, 132, 217, 244, 138, 139, 60, 39, 32 };

    boost::asio::write(_socket, boost::asio::buffer(result));
}

void Session::HandleAuthSessionToken(WorldPackets::Login::AuthSessionToken& data)
{
    WAR_LOG_INFO("auth.opcode", "Recieved: CMSG_AUTH_SESSION_TOKEN");
    WorldPackets::Login::AuthSessionTokenReply packet;
    SendPacket(packet.Write());
}

void Session::HandleCharSummaryList(WorldPackets::Login::CharSummaryList& data)
{
    WAR_LOG_INFO("auth.opcode", "Recieved: CMSG_CHAR_SUMMARY_LIST");
    WorldPackets::Login::CharSummaryListReply packet;
    SendPacket(packet.Write());
}

void Session::HandleClusterList(WorldPackets::Login::ClusterList& data)
{
    WAR_LOG_INFO("auth.opcode", "Recieved: CMSG_CLUSTER_LIST");
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

void Session::HandleAccPropList(WorldPackets::Login::AccPropList& data)
{
    WAR_LOG_INFO("auth.opcode", "Recieved: CMSG_ACC_PROP_LIST");
    WorldPackets::Login::AccPropListReply packet;
    SendPacket(packet.Write());
}

void Session::HandleAccProperties(WorldPackets::Login::AccProperties& data)
{
    WAR_LOG_INFO("auth.opcode", "Recieved: CMSG_ACC_PROPERTIES");
}
