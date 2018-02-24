#ifdef WIN32
#include "../WinCompat/stdafx.h"
#include <Windows.h>
#endif

#include <thread>
#include <functional>
#define NET
#include "ZiaSocket.hpp"
#include "ModNet.hpp"

void module::NetMod::setSelect() {
    _select.clearExceptFd();
    _select.clearReadFd();
    _select.clearWriteFd();
    _select.addReadFd(_socket.getSocketFd());
}

std::string module::NetMod::httpRead(std::unique_ptr<mysocket::Socket>& socket) {
    std::string msg;
    char buf[1025];
    ssize_t ret;
    while ((ret = socket->Recv(buf, 1024, 0)) > 0) {
        if (ret != 1024) {
            buf[ret] = '\0';
            msg += buf;
            break;
        }
        std::cout << "turn" << std::endl;
        buf[1024] = '\0';
        msg += buf;
    }
    return (ret < 0) ? nullptr : msg;
}

void module::NetMod::threadLaunching(mysocket::Socket* _client, Callback cb) {
    std::string msg;
    NetInfo inf;
    std::unique_ptr<mysocket::Socket> client(_client);

    if ((msg = httpRead(client)).empty())
        return;
    Raw rawMsg;
    for (auto &c: msg) rawMsg.push_back(static_cast<std::byte>(c));
    inf.sock = new ZiaSocket(client);
    cb(rawMsg, inf);
}

void module::NetMod::handleConnection(Callback cb) {

    if (_select.isFdSetRead(_socket.getSocketFd())) {
        auto* client = _socket.Accept();
        if (!client)
            return;
        std::thread t(&module::NetMod::threadLaunching, std::ref(*this), client, cb);
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
zia::api::Net* create(unsigned short port) {
#endif
    zia::api::Net* mod = nullptr;
    try {
        mod = new module::NetMod(port);
    }
    catch (...) {
        std::cerr << "Unable to create module: modExample" << std::endl;
    }
    return mod;
}
}