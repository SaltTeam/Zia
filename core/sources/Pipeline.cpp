
#include "Pipeline.hpp"

namespace Core {

    Pipeline::Pipeline(std::vector<Module *> &modules)
            : modules(modules) {
    }

    void Core::Pipeline::run(RequestPtr &request, ResponsePtr &response, NetInfo &netInfo) {
        for (Module *mod : modules) {
            if (auto modpp = dynamic_cast<PPModule *>(mod)) {
                auto ret = modpp->smartExec(request, response, netInfo);
                if (ret == false)
                    return;
            } else {
                HttpDuplex httpDuplex = zia::apipp::createBasicHttpDuplex(request, response, netInfo);
                mod->exec(httpDuplex);
                response = Response::fromBasicHttpDuplex(httpDuplex);
                request = Request::fromBasicHttpDuplex(httpDuplex);
            }
        }
    }
}
