#include "LoginPackets.h"

void WorldPackets::Login::LoginVerifyProtocol::Read()
{
}

WorldPacket const* WorldPackets::Login::LoginVerifyProtocolReply::Write()
{
    return &_worldPacket;
}

void WorldPackets::Login::AuthSessionToken::Read()
{
}

WorldPacket const* WorldPackets::Login::AuthSessionTokenReply::Write()
{
    return &_worldPacket;
}
