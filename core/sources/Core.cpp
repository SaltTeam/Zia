
#include "Core.hpp"

//bool NetTest::config(const zia::api::Conf &conf) {
//    return true;
//}
//
//bool NetTest::run(Net::Callback cb) {
//    while (true) {
//        std::string msg;
//        std::cin >> msg;
//        std::cout << "Receive message : " << msg << std::endl;
//        Raw rawMsg;
//        for (auto &c: msg) rawMsg.push_back(static_cast<std::byte>(c));
//        cb(rawMsg, NetInfo());
//    }
//}
//
//bool NetTest::send(zia::api::ImplSocket *sock, const Raw &resp) {
//    std::string msg;
//    std::transform(resp.begin(), resp.end(), std::back_inserter(msg),
//                   [](auto c) { return static_cast<char>(c); });
//    std::cout << "Send message : " << msg << std::endl;
//    return true;
//}
//
//bool NetTest::stop() {
//    return true;
//}

namespace Core {

    void Core::Core::run() {
        auto f = std::function<void(Raw, NetInfo)>(
                std::bind(&Core::runPipeline, this, std::placeholders::_1, std::placeholders::_2));
        net.run(f);
    }

    void Core::runPipeline(Raw req, NetInfo netInfo) {
        std::string msg;
        std::transform(req.begin(), req.end(), std::back_inserter(msg),
                       [](auto c) { return static_cast<char>(c); });
        std::cout << "Pipeline with:\n" << msg << std::endl;

        Raw resp;
        std::transform(msg.begin(), msg.end(), std::back_inserter(resp),
                       [](auto c) { return static_cast<std::byte>(c); });
        net.send(netInfo.sock, resp);
    }
}