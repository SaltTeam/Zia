#include <iostream>
#include <core/includes/Core.hpp>

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

    Core::Core core;
    core.run();

    end();
    return 0;
}