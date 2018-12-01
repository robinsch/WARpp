#include "AuthServer.h"

void AuthServer::Run()
{
    _accept();
    service.run();
}

void AuthServer::_accept()
{
    acceptor.async_accept(socket, [this](boost::system::error_code ec)
    {
        if (!ec)
            std::make_shared<Session>(std::move(socket))->AsyncRead();

        _accept();
    });
}
