#ifndef _OPCODES_H
#define _OPCODES_H

#include "Define.h"

class WorldPacket;
class Session;

#include <string>

enum ConnectionType : int8
{
    CONNECTION_TYPE_REALM       = 0,
    CONNECTION_TYPE_INSTANCE    = 1,
    MAX_CONNECTION_TYPES,

    CONNECTION_TYPE_DEFAULT     = -1
};

enum OpcodeMisc : uint16
{
    MAX_OPCODE                                        = 0x3FFF,
    NUM_OPCODE_HANDLERS                               = (MAX_OPCODE + 1),
    UNKNOWN_OPCODE                                    = 0xFFFF,
    NULL_OPCODE                                       = 0xBADD
};

enum OpcodeClient : uint16
{
    CMSG_VERIFY_PROTOCOL        = 0x01,
    CMSG_AUTH_SESSION_TOKEN     = 0x05,
};

enum OpcodeServer : uint16
{
    SMSG_VERIFY_PROTOCOL        = 0x02,
    SMSG_AUTH_SESSION_TOKEN     = 0x06,
};

/// Player state
enum SessionStatus
{
    STATUS_AUTHED = 0,                                      // Player authenticated (_player == NULL, m_playerRecentlyLogout = false or will be reset before handler call, m_GUID have garbage)
    STATUS_LOGGEDIN,                                        // Player in game (_player != NULL, m_GUID == _player->GetGUID(), inWorld())
    STATUS_TRANSFER,                                        // Player transferring to another map (_player != NULL, m_GUID == _player->GetGUID(), !inWorld())
    STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT,                    // _player != NULL or _player == NULL && m_playerRecentlyLogout && m_playerLogout, m_GUID store last _player guid)
    STATUS_NEVER,                                           // Opcode not accepted from client (deprecated or server side only)
    STATUS_UNHANDLED                                        // Opcode not handled yet
};

enum PacketProcessing
{
    PROCESS_INPLACE = 0,                                    //process packet whenever we receive it - mostly for non-handled or non-implemented packets
    PROCESS_THREADUNSAFE,                                   //packet is not thread-safe - process it in World::UpdateSessions()
    PROCESS_THREADSAFE                                      //packet is thread-safe - process it in Map::Update()
};

class OpcodeHandler
{
public:
    OpcodeHandler(char const* name, SessionStatus status) : Name(name), Status(status) { }
    virtual ~OpcodeHandler() { }

    char const* Name;
    SessionStatus Status;
};

class ClientOpcodeHandler : public OpcodeHandler
{
public:
    ClientOpcodeHandler(char const* name, SessionStatus status, PacketProcessing processing)
        : OpcodeHandler(name, status), ProcessingPlace(processing) { }

    virtual void Call(Session* session, WorldPacket& packet) const = 0;

    PacketProcessing ProcessingPlace;
};

class ServerOpcodeHandler : public OpcodeHandler
{
public:
    ServerOpcodeHandler(char const* name, SessionStatus status, ConnectionType conIdx)
        : OpcodeHandler(name, status), ConnectionIndex(conIdx) { }

    ConnectionType ConnectionIndex;
};

class OpcodeTable
{
public:
    OpcodeTable();

    OpcodeTable(OpcodeTable const&) = delete;
    OpcodeTable& operator=(OpcodeTable const&) = delete;

    ~OpcodeTable();

    void Initialize();

    ClientOpcodeHandler const* operator[](OpcodeClient index) const
    {
        return _internalTableClient[index];
    }

    ServerOpcodeHandler const* operator[](OpcodeServer index) const
    {
        return _internalTableServer[index];
    }

private:
    template<typename Handler, Handler HandlerFunction>
    void ValidateAndSetClientOpcode(OpcodeClient opcode, char const* name, SessionStatus status, PacketProcessing processing);

    void ValidateAndSetServerOpcode(OpcodeServer opcode, char const* name, SessionStatus status, ConnectionType conIdx);

    ClientOpcodeHandler* _internalTableClient[NUM_OPCODE_HANDLERS];
    ServerOpcodeHandler* _internalTableServer[NUM_OPCODE_HANDLERS];
};

extern OpcodeTable opcodeTable;

/// Lookup opcode name for human understandable logging
std::string GetOpcodeNameForLogging(OpcodeClient opcode);
std::string GetOpcodeNameForLogging(OpcodeServer opcode);

#endif