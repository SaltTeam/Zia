#pragma once

#include <atomic>
#include <openssl/ssl.h>
#include <openssl/err.h>
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
        mysocket::Select _select; /*! Select for the network */
        SOCKET sock;
        SSL_CTX* ctx;

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
        void threadLaunching(SSL* ssl, Callback cb);

        /*!
         * \brief read on the socket
         *
         * @param socket place where we read the informations
         * @return the string that contains what was readed
         */
        std::string httpRead(SSL* ssl);

    public:
        /*!
         * \brief Ctor
         *
         */
        NetMod(unsigned short port = 8080){
            SSL_library_init();
            OpenSSL_add_all_algorithms();
            SSL_load_error_strings();
            auto method = TLSv1_2_server_method();
            ctx = SSL_CTX_new(method);
            SSL_CTX_use_certificate_file(ctx, "mycert.pem", SSL_FILETYPE_PEM);
            SSL_CTX_use_PrivateKey_file(ctx, "mycert.pem", SSL_FILETYPE_PEM);
            struct sockaddr_in addr;
            sock = socket(PF_INET, SOCK_STREAM, 0);
            bzero(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;
            bind(sock, (struct sockaddr*)&addr, sizeof(addr));
            listen(sock, 42);
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