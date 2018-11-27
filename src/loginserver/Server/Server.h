#ifndef Server_h__
#define Server_h__

#include "Session.h"

#include <string>
#include <memory>
#include <boost\asio.hpp>

namespace ip = boost::asio::ip;

class Server
{
public:
    Server(std::string host_, uint16_t port_) : host(host_), port(port_),
        endpoint(ip::address::from_string(host), port), socket(service),
        acceptor(service, endpoint) { }

    void Run();

private:
    uint16 port;
    std::string host;
    boost::asio::io_service service;
    ip::tcp::endpoint endpoint;
    ip::tcp::socket socket;
    ip::tcp::acceptor acceptor;

    void _accept();
};

#endif