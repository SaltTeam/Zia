#ifdef WIN32
#include <Windows.h>
#endif
#include <iostream>
#include <core/includes/Core.hpp>
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


    ParserConfig parser;
    auto conf = parser.parse("./json_test");

    auto paths = conf["module_path"];
    auto virtualHosts = conf["virtualhost"];

    if (virtualHosts.getType() == zia::apipp::ConfElem::Type::Array)
    {
        for (auto &virtualHost : virtualHosts.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems)
        {
            for (auto &module : (*virtualHost)["modules"].get<std::shared_ptr<zia::apipp::ConfArray>>()->elems)
            {
            #ifdef WIN32
                STARTUPINFO si;
                PROCESS_INFORMATION pi;

                ZeroMemory( &si, sizeof(si) );
                si.cb = sizeof(si);
                ZeroMemory( &pi, sizeof(pi) );
                if (!CreateProcess
                        (
                            TEXT("Zia"),
                            NULL,NULL,NULL,FALSE,
                            CREATE_NEW_CONSOLE,
                            NULL,NULL,
                            &si,
                            &pi
                        )
                   )
                {
                    std::cout << "Unable to execute." << std::endl;
                }
            #else
                fork();
            #endif
                std::cout << (*module)["name"].get<std::string>() << std::endl;
            }
        }
    }

    init();

    Core::Core core;
    core.run();

    end();
    return 0;
}