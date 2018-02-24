
#pragma once

#include "Usings.hpp"
#include "ModNet.hpp"
#include "Pipeline.hpp"

//class NetTest : Net {
//public:
//    bool config(const zia::api::Conf &conf) override;
//
//    bool run(Callback cb) override;
//
//    bool send(zia::api::ImplSocket *sock, const Raw &resp) override;
//
//    bool stop() override;
//};

namespace Core {
    class Core {
    private:
        zia::api::Net *net;
        Pipeline pipeline;

    public:
        explicit Core(Pipeline const &value);
        void setNet(zia::api::Net *newNet);
        void run();

    private:
        void runPipeline(Raw raw, NetInfo netInfo);
    };
}
