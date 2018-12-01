#ifndef AuthServer_h__
#define AuthServer_h__

#include "Session.h"

#include <string>
#include <memory>
#include <boost\asio.hpp>

struct HostConnectionInfo
{
    HostConnectionInfo(std::string ip, uint16 port) : IP(ip), Port(port) {};
    std::string IP;
    uint16 Port;
};

namespace ip = boost::asio::ip;

class AuthServer
{
public:
    AuthServer(uint16 port_) : port(port_),
        endpoint(ip::address::from_string("127.0.0.1"), port), socket(service),
        acceptor(service, endpoint) { }

    void Run();

private:
    uint16 port;
    boost::asio::io_service service;
    ip::tcp::endpoint endpoint;
    ip::tcp::socket socket;
    ip::tcp::acceptor acceptor;

    void _accept();
};

#endif