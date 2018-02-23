#include <network/Addr.hpp>
#include <iostream>
#include <core/includes/Core.hpp>
#include <wait.h>
#include "ParserConfig.hpp"

static void init() {
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif // WIN32
}

static void end() {
#ifdef WIN32
    WSACleanup();
#endif // WIN32
}

int main() {

    init();

    ParserConfig parser;
    auto conf = parser.parse("./config.json");

    auto paths = conf["module_path"];
    auto virtualHosts = conf["virtualhost"];

    if (virtualHosts.getType() == zia::apipp::ConfElem::Type::Array)
    {
        for (auto &virtualHost : virtualHosts.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems)
        {
            for (auto &module : (*virtualHost)["modules"].get<std::shared_ptr<zia::apipp::ConfArray>>()->elems)
            {
            #ifdef WIN32
            #else
                pid_t pid = fork();
                if (pid == -1)
                {

                }
                else if (pid == 0)
                {


                    Core::Core core;
                    core.run();

                    end();
                    exit(0);
                }
                else
                {
                    int wstatus;
                    waitpid(pid, &wstatus, WCONTINUED);
                }
            #endif
                std::cout << (*module)["name"].get<std::string>() << std::endl;
            }
        }
    }

    end();
    return 0;
}