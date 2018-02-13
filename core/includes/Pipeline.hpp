
#pragma once

#include "Usings.hpp"

namespace Core {
    class Pipeline {
    private:
        std::vector<Module *> modules;

    public:
        explicit Pipeline(std::vector<Module *> &modules);

        void run(HttpDuplex &httpDuplex);
    };
}
