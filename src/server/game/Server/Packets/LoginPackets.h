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
            AuthSessionTokenReply() : ServerPacket(SMSG_VERIFY_PROTOCOL) { }

            WorldPacket const* Write() override;

            uint32 Result;
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
            CharSummaryListReply() : ServerPacket(SMSG_CHAR_SUMMARY_LIST) { }

            WorldPacket const* Write() override;

            uint8 Unk1;
            uint8 Unk2;
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

            uint8 Id;
            std::string Name;
            std::string Host;
            uint32 Port;
            uint8 Population;
            uint32 MaxPopulation;
            uint8 PopulationStatus;
            uint8 Language;
            int8 Status;

            // Reapeat
            uint8 ServerId;
            std::string ServerName;
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

            uint8 Unk1;
            uint8 Unk2;
        };
    }
}

#endif // LoginPackets_h__
