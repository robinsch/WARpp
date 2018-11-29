#include "Session.h"
#include "RealmMgr.h"

uint32 const SizeOfClientHeader = sizeof(uint8) + sizeof(uint8);
uint32 const SizeOfServerHeader = sizeof(uint8) + sizeof(uint8);

Session::Session(ip::tcp::socket socket_) : _socket(std::move(socket_)), _readBuffer(), _type(CONNECTION_TYPE_DEFAULT)
{
    _readBuffer.Resize(256);
    _headerBuffer.Resize(SizeOfClientHeader);
}

void Session::SendPacket(WorldPacket const * packet)
{
    if (packet->GetOpcode() == NULL_OPCODE)
        return;
    else if (packet->GetOpcode() == UNKNOWN_OPCODE)
        return;

    ServerOpcodeHandler const* handler = opcodeTable[static_cast<OpcodeServer>(packet->GetOpcode())];

    if (!handler)
    {
        std::cerr << "Prevented sending of opcode " << packet->GetOpcode() << "with non existing handler to";
        return;
    }

    // Default connection index defined in Opcodes.cpp table
    ConnectionType conIdx = handler->ConnectionIndex;

    // Override connection index
    if (packet->GetConnection() != CONNECTION_TYPE_DEFAULT)
    {
        conIdx = packet->GetConnection();
    }

    MessageBuffer buffer(4096);
    WritePacketToBuffer(*packet, buffer);

    if (buffer.GetActiveSize() > 0)
        _socket.async_write_some(boost::asio::buffer(buffer.GetReadPointer(), buffer.GetActiveSize()), std::bind(&Session::WriteHandler,
            this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Session::WritePacketToBuffer(WorldPacket const& packet, MessageBuffer & buffer)
{
    uint32 opcode = packet.GetOpcode();
    uint32 packetSize = uint32(packet.size());

    // Reserve space for buffer
    uint8* headerPos = buffer.GetWritePointer();
    buffer.WriteCompleted(SizeOfServerHeader);

    buffer.Write(packet.contents(), packet.size());

    PacketHeader header;
    header.Size = packetSize;
    header.Command = opcode;

    memcpy(headerPos, &header, SizeOfServerHeader);
}

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


