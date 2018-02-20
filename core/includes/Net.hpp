#pragma once

#include <atomic>
#include "network/Socket.hpp"
#include "network/Select.hpp"
#include "Usings.hpp"

#ifdef NET
std::atomic_bool stop(false);
#else
extern std::atomic_bool stop;
#endif

/// \namespace module
namespace module {
    /// \class NetMod
    class NetMod : public Net {

    protected:
        mysocket::Socket _socket;
        mysocket::Select _select;

    private:
        void setSelect();

        void handleConnection(Callback cb);

        void threadLaunching(mysocket::Socket* socket, Callback cb);

        std::string httpRead(std::unique_ptr<mysocket::Socket>& socket);

    public:
        NetMod() : _socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), _select() {
            _socket.setAddress(41008, "127.0.0.1");
            _socket.Bind();
            _socket.Listen(42);
        }

        bool config(const zia::api::Conf &conf) override;

        bool run(Callback cb) override;

        bool send(zia::api::ImplSocket *sock, const Raw &resp) override;

        bool stop() override;
    };
}