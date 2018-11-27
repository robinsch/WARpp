#include "Server.h"

void Server::Run()
{
    _accept();
    service.run();
}

void Server::_accept()
{
    acceptor.async_accept(socket, [this](boost::system::error_code ec)
    {
        if (!ec)
            std::make_shared<Session>(std::move(socket))->AsyncRead();

        _accept();
    });
}
