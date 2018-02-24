/*
** Socket.cpp for CPP_Simple_Networking_Library in /home/soszyn_h/Documents/CPP_Simple_Networking_Library/sources/Socket.cpp
**
** Made by Hugo SOSZYNSKI
** Login   <hugo.soszynski@epitech.eu>
**
** Started on  Wed Feb 15 19:29:53 2017 Hugo SOSZYNSKI
** Last update Wed Feb 15 19:29:53 2017 Hugo SOSZYNSKI
*/

#include <cerrno>
#include <iostream>
#include "network/Socket.hpp"

namespace mysocket {

    Socket::Socket(int domain, int type, int protocol)
            : _domain(domain), _type(type), _protocol(protocol), _socket(-1),
              _address(nullptr) {
        this->_socket = socket(domain, type, protocol);
		if (this->_socket == INVALID_SOCKET) {
			throw SocketException(strerror(errno));
		}
        if (domain == AF_INET) {
            this->_address = std::make_unique<InetAddr>();
        }
        else {
            closesocket(this->_socket);
            throw SocketException("ERROR: Socket type not implemented");
        }
    }

    Socket::Socket(int domain, int type, int protocol,
                   SOCKET fd, std::unique_ptr<InetAddr>& addr)
            : _domain(domain), _type(type), _protocol(protocol), _socket(fd),
              _address(addr.release()) {}

    Socket::~Socket() {
        closesocket(this->_socket);
    }

    void Socket::setAddress(unsigned short sinPort, std::string const& sinAddr) {
        _address->SetAddress(sinAddr);
        _address->SetPort(sinPort);
    }


    SOCKET Socket::getSocketFd() const { return this->_socket; }

    int Socket::Connect() {
        return connect(
                this->_socket,
                reinterpret_cast<SOCKADDR*>(&(this->_address->GetStruct())),
                this->_address->Size
        );
    }

    int Socket::Bind() {
        return bind(
                this->_socket,
                reinterpret_cast<SOCKADDR*>(&(this->_address->GetStruct())),
                this->_address->Size
        );
    }

    int Socket::Listen(int backlog) {
        return listen(this->_socket, backlog);
    }

    Socket* Socket::Accept() {
        SOCKET sock;
        SOCKADDR_IN addr{};
        socklen_t len = sizeof(addr);

        sock = accept(this->_socket, reinterpret_cast<SOCKADDR*>(&addr), &len);
        if (sock == -1)
            return nullptr;
        std::unique_ptr<InetAddr> ptr = std::make_unique<InetAddr>(addr);
        auto* client = new Socket(this->_domain, this->_type, this->_protocol,
                                  sock, ptr);
        return client;
    }

    unsigned long Socket::GetPeerRawAddress() {
        SOCKADDR_STORAGE addr{};
        socklen_t len{sizeof(addr)};

        getpeername(_socket, (struct sockaddr*)&addr, &len);
        auto *s = (SOCKADDR_IN*)&addr;
        return s->sin_addr.s_addr;
    }

    ssize_t Socket::Send(const void* buf, size_t length, int flags) {
        return send(_socket, reinterpret_cast<const char*>(buf), length, flags);
    }

    ssize_t Socket::Recv(void* buf, size_t maxLen, int flags) {
        return recv(_socket, reinterpret_cast<char*>(buf), maxLen, flags);
    }

    ssize_t Socket::SendTo(const void* msg, size_t len, unsigned int flags, InetAddr& to) {
        return sendto(_socket, reinterpret_cast<const char*>(msg), len, flags, reinterpret_cast<SOCKADDR*>(&to.GetStruct()), InetAddr::Size);
    }

    ssize_t Socket::RecvFrom(void* buf, size_t maxLen, unsigned int flags, InetAddr& from) {
        socklen_t len {InetAddr::Size};
        return recvfrom(_socket, reinterpret_cast<char*>(buf), maxLen, flags, reinterpret_cast<SOCKADDR*>(&from.GetStruct()), &len);
    }
}
