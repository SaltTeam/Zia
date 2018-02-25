#pragma once

#include <network/Socket.hpp>
#include <openssl/ossl_typ.h>
#include <openssl/ssl.h>
#include "Usings.hpp"

namespace module {
    class ZiaSocket : public zia::api::ImplSocket {
    protected:
        SSL* ssl;

    public:
        explicit ZiaSocket(SSL* ssl)
                : ssl(ssl)
        {}

        void sendMessage(std::string &string) override {
            SSL_write(ssl, string.c_str(), string.length());
        }

        std::string receiveMessage() override {
            std::string msg;
            char buf[1024];
            ssize_t ret;
            while ((ret = SSL_read(ssl, buf, 1024)) > 0)
                msg += buf;
            return (ret < 0) ? nullptr : msg;
        }
    };

}