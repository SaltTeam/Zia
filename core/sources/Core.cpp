
#include "Core.hpp"

#include "Processing.hpp"

namespace Core {

    void Core::Core::run() {
        auto f = std::function<void(Raw, NetInfo)>(
                std::bind(&Core::runPipeline, this, std::placeholders::_1, std::placeholders::_2));
        net.run(f);
    }

    void Core::runPipeline(Raw req, NetInfo netInfo) {
        try {
            std::string msg;
            std::transform(req.begin(), req.end(), std::back_inserter(msg),
                           [](auto c) { return static_cast<char>(c); });
            Request request = core::Processing::parseRequest(req);
        } catch (std::exception &e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }
}