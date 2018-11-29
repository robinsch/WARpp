/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Opcodes.h"
#include "Session.h"

#include <iomanip>
#include <sstream>

template<class PacketClass, void(Session::*HandlerFunction)(PacketClass&)>
class PacketHandler : public ClientOpcodeHandler
{
public:
    PacketHandler(char const* name, SessionStatus status, PacketProcessing processing) : ClientOpcodeHandler(name, status, processing) { }

    void Call(Session* session, WorldPacket& packet) const override
    {
        PacketClass nicePacket(std::move(packet));
        nicePacket.Read();
        (session->*HandlerFunction)(nicePacket);
        //session->LogUnprocessedTail(nicePacket.GetRawPacket());
    }
};

template<void(Session::*HandlerFunction)(WorldPacket&)>
class PacketHandler<WorldPacket, HandlerFunction> : public ClientOpcodeHandler
{
public:
    PacketHandler(char const* name, SessionStatus status, PacketProcessing processing) : ClientOpcodeHandler(name, status, processing) { }

    void Call(Session* session, WorldPacket& packet) const override
    {
        (session->*HandlerFunction)(packet);
        //session->LogUnprocessedTail(&packet);
    }
};

OpcodeTable opcodeTable;

template<typename T>
struct get_packet_class
{
};

template<typename PacketClass>
struct get_packet_class<void(Session::*)(PacketClass&)>
{
    using type = PacketClass;
};

OpcodeTable::OpcodeTable()
{
    memset(_internalTableClient, 0, sizeof(_internalTableClient));
    memset(_internalTableServer, 0, sizeof(_internalTableServer));
}

OpcodeTable::~OpcodeTable()
{
    for (uint16 i = 0; i < NUM_OPCODE_HANDLERS; ++i)
    {
        delete _internalTableClient[i];
        delete _internalTableServer[i];
    }
}

template<typename Handler, Handler HandlerFunction>
void OpcodeTable::ValidateAndSetClientOpcode(OpcodeClient opcode, char const* name, SessionStatus status, PacketProcessing processing)
{
    if (uint32(opcode) == NULL_OPCODE)
    {
        //TC_LOG_ERROR("network", "Opcode %s does not have a value", name);
        return;
    }

    if (uint32(opcode) >= NUM_OPCODE_HANDLERS)
    {
        //TC_LOG_ERROR("network", "Tried to set handler for an invalid opcode %d", opcode);
        return;
    }

    if (_internalTableClient[opcode] != NULL)
    {
        //TC_LOG_ERROR("network", "Tried to override client handler of %s with %s (opcode %u)", opcodeTable[opcode]->Name, name, opcode);
        return;
    }

    _internalTableClient[opcode] = new PacketHandler<typename get_packet_class<Handler>::type, HandlerFunction>(name, status, processing);
}

void OpcodeTable::ValidateAndSetServerOpcode(OpcodeServer opcode, char const* name, SessionStatus status, ConnectionType conIdx)
{
    if (uint32(opcode) == NULL_OPCODE)
    {
        //TC_LOG_ERROR("network", "Opcode %s does not have a value", name);
        return;
    }

    if (uint32(opcode) >= NUM_OPCODE_HANDLERS)
    {
        //TC_LOG_ERROR("network", "Tried to set handler for an invalid opcode %d", opcode);
        return;
    }

    if (conIdx >= MAX_CONNECTION_TYPES)
    {
        //TC_LOG_ERROR("network", "Tried to set invalid connection type %u for opcode %s", conIdx, name);
        return;
    }

    if (_internalTableServer[opcode] != NULL)
    {
        //TC_LOG_ERROR("network", "Tried to override server handler of %s with %s (opcode %u)", opcodeTable[opcode]->Name, name, opcode);
        return;
    }

    _internalTableServer[opcode] = new ServerOpcodeHandler(name, status, conIdx);
}

/// Correspondence between opcodes and their names
void OpcodeTable::Initialize()
{
#define DEFINE_HANDLER(opcode, status, processing, handler) \
    ValidateAndSetClientOpcode<decltype(handler), handler>(opcode, #opcode, status, processing)

    DEFINE_HANDLER(CMSG_VERIFY_PROTOCOL,                                STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &Session::HandleVerifyProtocol);
    DEFINE_HANDLER(CMSG_AUTH_SESSION_TOKEN,                             STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &Session::HandleAuthSessionToken);
    DEFINE_HANDLER(CMSG_CHAR_SUMMARY_LIST,                              STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &Session::HandleCharSummaryList);
    DEFINE_HANDLER(CMSG_CLUSTER_LIST,                                   STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &Session::HandleClusterList);  
    DEFINE_HANDLER(CMSG_ACC_PROP_LIST,                                  STATUS_LOGGEDIN,  PROCESS_THREADUNSAFE, &Session::HandleAccPropList);    

#undef DEFINE_HANDLER

#define DEFINE_SERVER_OPCODE_HANDLER(opcode, status, con) \
    static_assert(status == STATUS_NEVER || status == STATUS_UNHANDLED, "Invalid status for server opcode"); \
    ValidateAndSetServerOpcode(opcode, #opcode, status, con)

    DEFINE_SERVER_OPCODE_HANDLER(SMSG_VERIFY_PROTOCOL,                       STATUS_NEVER,        CONNECTION_TYPE_DEFAULT);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_AUTH_SESSION_TOKEN,                    STATUS_NEVER,        CONNECTION_TYPE_DEFAULT);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CHAR_SUMMARY_LIST,                     STATUS_NEVER,        CONNECTION_TYPE_DEFAULT);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_CLUSTER_LIST,                          STATUS_NEVER,        CONNECTION_TYPE_DEFAULT);
    DEFINE_SERVER_OPCODE_HANDLER(SMSG_ACC_PROP_LIST,                         STATUS_NEVER,        CONNECTION_TYPE_DEFAULT);

#undef DEFINE_SERVER_OPCODE_HANDLER
};

template<typename T>
inline std::string GetOpcodeNameForLoggingImpl(T id)
{
    uint32 opcode = uint32(id);
    std::ostringstream ss;
    ss << '[';

    if (static_cast<uint32>(id) < NUM_OPCODE_HANDLERS)
    {
        if (OpcodeHandler const* handler = opcodeTable[id])
            ss << handler->Name;
        else
            ss << "UNKNOWN OPCODE";
    }
    else
        ss << "INVALID OPCODE";

    ss << " 0x" << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << opcode << std::nouppercase << std::dec << " (" << opcode << ")]";
    return ss.str();
}

std::string GetOpcodeNameForLogging(OpcodeClient opcode)
{
    return GetOpcodeNameForLoggingImpl(opcode);
}

std::string GetOpcodeNameForLogging(OpcodeServer opcode)
{
    return GetOpcodeNameForLoggingImpl(opcode);
}
