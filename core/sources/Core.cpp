
#include "Core.hpp"

#include "Processing.hpp"

namespace Core {

    Core::Core(Pipeline const &value)
    : pipeline(value) {

    }

    void Core::Core::run() {
        auto f = std::function<void(Raw, NetInfo)>(
                std::bind(&Core::runPipeline, this, std::placeholders::_1, std::placeholders::_2));
        net->run(f);
    }

    void Core::setNet(module::NetMod *newNet) {
        net = newNet;
    }

    void Core::runPipeline(Raw req, NetInfo netInfo) {
        try {
            RequestPtr request = core::Processing::parseRequest(req);
            ResponsePtr response = std::make_shared<Response>(*request);

            pipeline.run(request, response, netInfo);

            Raw resp = core::Processing::createResponse(response);

            std::string msg;
            std::transform(resp.begin(), resp.end(), std::back_inserter(msg),
                           [](auto &c) { return static_cast<char>(c); });
            std::cout << msg << std::endl;

            net->send(netInfo.sock, resp);
        } catch (std::exception &e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }
}