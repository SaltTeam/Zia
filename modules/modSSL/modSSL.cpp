#ifdef WIN32
#include "../WinCompat/stdafx.h"
#include <Windows.h>
#endif

#include <thread>
#include <functional>
#define NET
#include "ZiaSocket.hpp"
#include "modSSL.hpp"

void module::NetMod::setSelect() {
    _select.clearExceptFd();
    _select.clearReadFd();
    _select.clearWriteFd();
    _select.addReadFd(sock);
}

std::string module::NetMod::httpRead(SSL* ssl) {
    std::string msg;
    char buf[1025];
    ssize_t ret;
    while ((ret = SSL_read(ssl, buf, 1024)) > 0) {
        if (ret != 1024) {
            buf[ret] = '\0';
            msg += buf;
            break;
        }
        buf[1024] = '\0';
        msg += buf;
    }
    return (ret < 0) ? nullptr : msg;
}

void module::NetMod::threadLaunching(SSL* ssl, Callback cb) {
    std::string msg;
    NetInfo inf;

    if (SSL_accept(ssl) == -1)
        return;
    if ((msg = httpRead(ssl)).empty())
        return;
    Raw rawMsg;
    for (auto &c: msg) rawMsg.push_back(static_cast<std::byte>(c));
    inf.sock = new ZiaSocket(ssl);
    cb(rawMsg, inf);
    auto client = SSL_get_fd(ssl);
    SSL_free(ssl);
    closesocket(client);
}

void module::NetMod::handleConnection(Callback cb) {

    if (_select.isFdSetRead(sock)) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        auto client = accept(sock, (struct sockaddr*)&addr, &len);
        if (!client)
            return;
        auto ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);
        std::thread t(&module::NetMod::threadLaunching, std::ref(*this), ssl, cb);
        t.detach();
    }
}

bool module::NetMod::run(Callback cb) {
    for (;;) {
        setSelect();
        _select.wait();
        handleConnection(cb);
    }
}

bool module::NetMod::send(zia::api::ImplSocket *sock, const Raw &resp) {
    std::string msg;
    std::transform(resp.begin(), resp.end(), std::back_inserter(msg),
                   [](auto c) { return static_cast<char>(c); });
    sock->sendMessage(msg);
    return true;
}

bool module::NetMod::config(const zia::api::Conf &conf) {
    return true;
}

bool module::NetMod::stop() {
    return true;
}

extern "C" {
#ifdef WIN32
__declspec(dllexport) zia::api::Net* create(unsigned short port) {
#else
module::NetMod* create(unsigned short port) {
#endif
    module::NetMod* mod = nullptr;
    try {
        mod = new module::NetMod(port);
    }
    catch (...) {
        std::cerr << "Unable to create module: modExample" << std::endl;
    }
    return mod;
}
}