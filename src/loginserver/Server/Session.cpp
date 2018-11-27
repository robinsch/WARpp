#include "Session.h"

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
    uint32 packetSize = packet.size();

    // Reserve space for buffer
    uint8* headerPos = buffer.GetWritePointer();
    buffer.WriteCompleted(SizeOfServerHeader);

    buffer.Write(packet.contents(), packet.size());

    packetSize += 1 /*opcode*/;

    PacketHeader header;
    header.Size = packetSize;
    header.Command = opcode;

    memcpy(headerPos, &header, SizeOfServerHeader);
}

void Session::HandleVerifyProtocol(WorldPackets::Login::LoginVerifyProtocol& loginQuery)
{
    std::cout << "Client connected" << '\n';

    WorldPackets::Login::LoginVerifyProtocolReply packet;
    char result[] = { 38, 2, 8, 0, 18, 16, 1, 83, 33, 77, 74, 4, 39, 183, 180, 89, 15, 62, 167, 157, 41, 233, 26, 16, 73, 24, 161, 42, 100, 225, 218, 189, 132, 217, 244, 138, 139, 60, 39, 32 };

    boost::asio::write(_socket, boost::asio::buffer(result));
}

void Session::HandleAuthSessionToken(WorldPackets::Login::AuthSessionToken& token)
{
    std::cout << "Client request auth session token" << '\n';

    WorldPackets::Login::LoginVerifyProtocolReply packet;
    char result[] = { 2, 6, 8, 0 };

    boost::asio::write(_socket, boost::asio::buffer(result));
}


