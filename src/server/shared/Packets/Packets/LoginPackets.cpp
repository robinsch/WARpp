#include "LoginPackets.h"

WorldPacket const* WorldPackets::Login::LoginVerifyProtocolReply::Write()
{
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Login::AuthSessionTokenReply::Write()
{
    _worldPacket << Result;
    _worldPacket << Unk2;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Login::CharSummaryListReply::Write()
{
    _worldPacket << Unk1;
    _worldPacket << Unk2;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Login::AccPropListReply::Write()
{
    _worldPacket << Unk1;
    _worldPacket << Unk2;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Login::ClusterListReply::Write()
{
    // Header
    _worldPacket << uint8(8);
    _worldPacket << uint8(0);
    _worldPacket << uint8(18);
    _worldPacket << uint8(52);

    _worldPacket << uint8(8);
    _worldPacket << Id;

    _worldPacket << uint8(18);
    _worldPacket << Name;

    _worldPacket << uint8(26);
    _worldPacket << Host;

    _worldPacket << uint8(32);
    _worldPacket << Port;

    _worldPacket << uint8(48);
    _worldPacket << MaxPopulation;
    
    _worldPacket << uint8(56);
    _worldPacket << PopulationStatus;

    _worldPacket << uint8(64);
    _worldPacket << Language;

    _worldPacket << uint8(72);
    _worldPacket << Status;

    _worldPacket << uint8(82);
    _worldPacket << uint8(2 + sizeof(ServerId) + sizeof(uint8) + ServerName.length());

    _worldPacket << uint8(8);
    _worldPacket << ServerId;

    _worldPacket << uint8(18);
    _worldPacket << ServerName;

    return &_worldPacket;
}
