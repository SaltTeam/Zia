
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

			response->statusCode = 200;
			response->statusReason = "OK";

            pipeline.run(request, response, netInfo);

			if (response->headers["Content-length"].empty())
				response->headers["Content-length"].push_back(std::to_string(response->body.length()));
			if (response->headers["Connection"].empty())
				response->headers["Connection"].push_back("close");
			if (response->headers["Content-type"].empty())
				response->headers["Content-type"].push_back("text/plain");

			Raw resp = core::Processing::createResponse(response);

			std::string msg;
            std::transform(resp.begin(), resp.end(), std::back_inserter(msg),
                           [](auto &c) { return static_cast<char>(c); });

            net->send(netInfo.sock, resp);
        } catch (std::exception &e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }
}