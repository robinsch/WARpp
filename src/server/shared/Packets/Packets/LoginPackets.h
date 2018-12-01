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

            void Read() override {};
        };

        class LoginVerifyProtocolReply final : public ServerPacket
        {
        public:
            LoginVerifyProtocolReply() : ServerPacket(SMSG_VERIFY_PROTOCOL) { }

            WorldPacket const* Write() override;
        };

        class AuthSessionToken final : public ClientPacket
        {
        public:
            AuthSessionToken(WorldPacket&& packet) : ClientPacket(CMSG_AUTH_SESSION_TOKEN, std::move(packet)) { }

            void Read() override {};
        };

        class AuthSessionTokenReply final : public ServerPacket
        {
        public:
            AuthSessionTokenReply() : ServerPacket(SMSG_AUTH_SESSION_TOKEN, 2) { }

            WorldPacket const* Write() override;

            uint32 Result = 8;
            uint32 Unk2 = 0;
        };

        class CharSummaryList final : public ClientPacket
        {
        public:
            CharSummaryList(WorldPacket&& packet) : ClientPacket(CMSG_CHAR_SUMMARY_LIST, std::move(packet)) { }

            void Read() override {};
        };

        class CharSummaryListReply final : public ServerPacket
        {
        public:
            CharSummaryListReply() : ServerPacket(SMSG_CHAR_SUMMARY_LIST, 2) { }

            WorldPacket const* Write() override;

            uint8 Unk1 = 8;
            uint8 Unk2 = 0;
        };

        class ClusterList final : public ClientPacket
        {
        public:
            ClusterList(WorldPacket&& packet) : ClientPacket(CMSG_CLUSTER_LIST, std::move(packet)) { }

            void Read() override {};
        };

        class ClusterListReply final : public ServerPacket
        {
        public:
            ClusterListReply() : ServerPacket(SMSG_CLUSTER_LIST) { }

            WorldPacket const* Write() override;

            uint8 Id = 0;
            std::string Name = "";
            std::string Host = "127.0.0.1";
            uint32 Port = 18047;
            uint8 Population = 0;
            uint32 MaxPopulation = 0;
            uint8 PopulationStatus = 1;
            uint8 Language = 1;
            int8 Status = 1;

            // Reapeat
            uint8 ServerId = 1;
            std::string ServerName = "";
        };

        class AccPropList final : public ClientPacket
        {
        public:
            AccPropList(WorldPacket&& packet) : ClientPacket(CMSG_ACC_PROP_LIST, std::move(packet)) { }

            void Read() override {};
        };

        class AccPropListReply final : public ServerPacket
        {
        public:
            AccPropListReply() : ServerPacket(SMSG_ACC_PROP_LIST) { }

            WorldPacket const* Write() override;

            uint8 Unk1 = 8;
            uint8 Unk2 = 0;
        };
    }
}

#endif // LoginPackets_h__
