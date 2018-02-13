
#include "Pipeline.hpp"

namespace Core {

    Pipeline::Pipeline(std::vector<Module *> &modules)
            : modules(modules) {
    }

    void Core::Pipeline::run(HttpDuplex &httpDuplex) {
        for (Module *mod : modules) {
            mod->exec(httpDuplex);
        }
    }
}
