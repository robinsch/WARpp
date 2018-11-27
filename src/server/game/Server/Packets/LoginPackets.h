#ifndef LoginPackets_h__
#define LoginPackets_h__

#include "Opcodes.h"
#include "Packet.h"

namespace WorldPackets
{
    namespace Login
    {
        class LoginVerifyProtocol final : public ClientPacket
        {
        public:
            LoginVerifyProtocol(WorldPacket&& packet) : ClientPacket(CMSG_VERIFY_PROTOCOL, std::move(packet)) { }

            void Read() override;
        };

        class LoginVerifyProtocolReply final : public ServerPacket
        {
        public:
            LoginVerifyProtocolReply() : ServerPacket(SMSG_VERIFY_PROTOCOL) { }

            WorldPacket const* Write() override;

            uint32 Result;
        };

        class AuthSessionToken final : public ClientPacket
        {
        public:
            AuthSessionToken(WorldPacket&& packet) : ClientPacket(CMSG_AUTH_SESSION_TOKEN, std::move(packet)) { }

            void Read() override;
        };

        class AuthSessionTokenReply final : public ServerPacket
        {
        public:
            AuthSessionTokenReply() : ServerPacket(SMSG_VERIFY_PROTOCOL) { }

            WorldPacket const* Write() override;

            uint32 Result;
        };        
    }
}

#endif // LoginPackets_h__
