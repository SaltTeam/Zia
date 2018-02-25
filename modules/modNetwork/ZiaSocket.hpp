#pragma once

#include <network/Socket.hpp>
#include "Usings.hpp"

namespace module {
    class ZiaSocket : public zia::api::ImplSocket {
    protected:
        std::unique_ptr<mysocket::Socket> _socket;

    public:
        explicit ZiaSocket(std::unique_ptr<mysocket::Socket>& socket) {
            _socket = std::move(socket);
        }

        void sendMessage(std::string &string) override {
            _socket->Send(string.c_str(), string.length(), 0);
        }

        std::string receiveMessage() override {
            std::string msg;
            char buf[1024];
            ssize_t ret;
            while ((ret = _socket->Recv(buf, 1024, MSG_WAITALL)) > 0)
                msg += buf;
            return (ret < 0) ? nullptr : msg;
        }
    };

}