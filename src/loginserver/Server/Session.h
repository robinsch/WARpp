#ifndef Session_h__
#define Session_h__

#include "MessageBuffer.h"
#include "Define.h"
#include "ByteConverter.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "LoginPackets.h"
#include "Server.h"

#include <memory>
#include <boost\asio.hpp>
#include <iostream>

namespace ip = boost::asio::ip;

class Session : public std::enable_shared_from_this<Session>
{
public:

    Session(ip::tcp::socket socket_);

    void AsyncRead()
    {
        _readBuffer.Normalize();
        _readBuffer.EnsureFreeSpace();
        _socket.async_read_some(boost::asio::buffer(_readBuffer.GetWritePointer(), _readBuffer.GetRemainingSpace()),
            std::bind(&Session::ReadHandlerInternal, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

    void ReadHandlerInternal(const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        if (error)
            return;

        _readBuffer.WriteCompleted(bytes_transferred);
        ReadHandler();
    }

    struct PacketHeader
    {
        uint8 Size;
        uint8 Command;
    };

    bool ReadHeaderHandler()
    {
        PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());
        _packetBuffer.Resize(header->Size);
        return true;
    }

    void ReadHandler()
    {
        MessageBuffer& packet = GetReadBuffer();
        while (packet.GetActiveSize() > 0)
        {
            if (_headerBuffer.GetRemainingSpace() > 0)
            {
                // need to receive the header
                std::size_t readHeaderSize = std::min(packet.GetActiveSize(), _headerBuffer.GetRemainingSpace());
                _headerBuffer.Write(packet.GetReadPointer(), readHeaderSize);
                packet.ReadCompleted(readHeaderSize);

                if (_headerBuffer.GetRemainingSpace() > 0)
                {
                    // Couldn't receive the whole header this time.
                    break;
                }

                // We just received nice new header
                if (!ReadHeaderHandler())
                    return;
            }

            // We have full read header, now check the data payload
            if (_packetBuffer.GetRemainingSpace() > 0)
            {
                // need more data in the payload
                std::size_t readDataSize = std::min(packet.GetActiveSize(), _packetBuffer.GetRemainingSpace());
                _packetBuffer.Write(packet.GetReadPointer(), readDataSize);
                packet.ReadCompleted(readDataSize);

                if (_packetBuffer.GetRemainingSpace() > 0)
                {
                    // Couldn't receive the whole data this time.
                    break;
                }
            }

            // just received fresh new payload
            bool result = ReadDataHandler();
            _headerBuffer.Reset();
            if (!result)
                return;
        }

        AsyncRead();
    }

    bool ReadDataHandler()
    {
        PacketHeader* header = reinterpret_cast<PacketHeader*>(_headerBuffer.GetReadPointer());
        OpcodeClient opcode = static_cast<OpcodeClient>(header->Command);

        WorldPacket* packet = new WorldPacket(opcode, std::move(_packetBuffer), GetConnectionType());

        ClientOpcodeHandler const* opHandle = opcodeTable[static_cast<OpcodeClient>(packet->GetOpcode())];
        if (!opHandle)
        {
            std::cerr << "Received unimplemented Opcode: " << packet->GetOpcode() << " with size: " << packet->size() << '\n';
            return true;
        }

        opHandle->Call(this, *packet);

        return true;
    }

    void WriteHandler(boost::system::error_code error, std::size_t transferedBytes)
    {
        if (!error)
        {
        }
        else
            return;
    }

    void SendPacket(WorldPacket const* packet);
    void WritePacketToBuffer(WorldPacket const& packet, MessageBuffer& buffer);

    void HandleVerifyProtocol(WorldPackets::Login::LoginVerifyProtocol& loginQuery);
    void HandleAuthSessionToken(WorldPackets::Login::AuthSessionToken& token);
    void HandleCharSummaryList(WorldPackets::Login::CharSummaryList& list);
    void HandleClusterList(WorldPackets::Login::ClusterList& list);
    void HandleAccPropList(WorldPackets::Login::AccPropList& list);

    ConnectionType GetConnectionType() const { return _type; }

private:
    ip::tcp::socket _socket;

    ConnectionType _type;

    MessageBuffer& GetReadBuffer() { return _readBuffer; }

    MessageBuffer _readBuffer;
    MessageBuffer _headerBuffer;
    MessageBuffer _packetBuffer;
};

#endif