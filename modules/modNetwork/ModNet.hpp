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

/*! \namespace module
 *
 * namespace regrouping all of the modules that will be use by the core
 */
namespace module {
    /*! \class NetMod
     *
     * class for the network module
     */
    class NetMod : public Net {

    protected:
        mysocket::Socket _socket; /*! Socket for the network */
        mysocket::Select _select; /*! Select for the network */

    private:

        /*!
         * \brief set the select for the network
         */
        void setSelect();

        /*!
         * \brief handle the connection
         *
         * @param cb callback that will be executed when this new thread start
         */
        void handleConnection(Callback cb);

        /*!
         * \brief launch the thread
         *
         * @param socket socket that will be use for the network
         * @param cb callback that will be executed when this new thread start
         */
        void threadLaunching(mysocket::Socket* socket, Callback cb);

        /*!
         * \brief read on the socket
         *
         * @param socket place where we read the informations
         * @return the string that contains what was readed
         */
        std::string httpRead(std::unique_ptr<mysocket::Socket>& socket);

    public:
        /*!
         * \brief Ctor
         *
         */
        NetMod(unsigned short port = 8080) : _socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), _select() {
            _socket.setAddress(port, "0.0.0.0");
            _socket.Bind();
            _socket.Listen(42);
        }

        /*!
         * \brief check the config
         *
         * @param conf the config that will be checked
         * @return true if the config is ok false otherwise
         */
        bool config(const zia::api::Conf &conf) override;

        /*!
         * \brief run the mod
         *
         * @param cb callback that will be executed when this new thread start
         * @return true if the run is ok false otherwise
         */
        virtual bool run(Callback cb);

        /*!
         * \brief send the response
         *
         * @param sock socket on which it will be sent
         * @param resp the response that will be sent
         * @return true if message successfully sent false otherwise
         */
        bool send(zia::api::ImplSocket *sock, const Raw &resp) override;

         /*!
         * \brief stop the network
         *
         * @return true if stop correctly false otherwise
         */
        bool stop() override;
    };
}