#include <thread>
#include <functional>
#define NET
#include "ZiaSocket.hpp"
#include "ModNet.hpp"

bool module::NetMod::run(Callback cb) {
    for (;;) {
        setSelect();
        std::cout << "will wait" << std::endl;
        _select.wait();
        std::cout << "wait terminated" << std::endl;
        handleConnection(cb);
    }
}

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
    while ((ret = socket->Recv(buf, 1024, 0)) >= 0) {
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

void module::NetMod::threadLaunching(mysocket::Socket* _client, Callback cb) {
    std::cout << "thread is launched" << std::endl;
    std::string msg;
    NetInfo inf;
    std::unique_ptr<mysocket::Socket> client(_client);

    if ((msg = httpRead(client)).empty())
        return;
    std::cout << "has read new stuff" << std::endl;
    Raw rawMsg;
    for (auto &c: msg) rawMsg.push_back(static_cast<std::byte>(c));
    inf.sock = new ZiaSocket(client);
    std::cout << "Callback is called" << std::endl;
    cb(rawMsg, inf);
}

void module::NetMod::handleConnection(Callback cb) {
    std::cout << "enter handle connect" << std::endl;
    if (_select.isFdSetRead(_socket.getSocketFd())) {
        auto* client = _socket.Accept();
        if (client == nullptr)
            return;
        std::cout << "new client created" << std::endl;
        std::thread t(&module::NetMod::threadLaunching, std::ref(*this), client, cb);
        t.detach();
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
