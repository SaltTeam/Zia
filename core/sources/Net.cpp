#include <ZiaSocket.hpp>
#include "Net.hpp"

void module::NetMod::setSelect() {
    _select.clearExceptFd();
    _select.clearReadFd();
    _select.clearWriteFd();
    _select.addReadFd(_socket.getSocketFd());
}

std::string module::NetMod::httpRead(std::unique_ptr<mysocket::Socket> socket) {
    std::string msg;
    char buf[1024];
    ssize_t ret;
    while ((ret = socket->Recv(buf, 1024, MSG_WAITALL)) > 0)
        msg += buf;
    return (ret < 0) ? nullptr : msg;
}

void module::NetMod::handleConnection(Callback cb) {
    std::string msg;
    NetInfo inf;
    if (_select.isFdSetRead(_socket.getSocketFd())) {
        std::unique_ptr<mysocket::Socket> client(_socket.Accept());
        if (!client)
            return;
        if ((msg = httpRead(&client)) == nullptr)
            return;
        Raw rawMsg;
        for (auto &c: msg) rawMsg.push_back(static_cast<std::byte>(c));
        inf.sock = new ZiaSocket(client);
        cb(rawMsg, inf);
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
}

bool module::NetMod::config(const zia::api::Conf &conf) {
    return true;
}

bool module::NetMod::stop() {
    return true;
}