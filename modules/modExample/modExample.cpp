//
// Created by soszyn_h on 13/02/18.
//

#include "sza-plus-plus/api/module.h"
#include "modExample.hpp"

bool modExample::perform() {
    this->response->useStandardData();

    try {
        this->response
                ->setStatus(200, "OK")
                ->addHeader("Set-Cookie", "test")
                ->setStandardData(this->conf.get_at("message")->get<std::string>());
    }
    catch (zia::apipp::Conf::InvalidAccess const& e) {
        this->response
                ->setStatus(500, "Internal Server Error")
                ->setStandardData("<h1>500</h1><br/><p>Internal Server Error</p>");
    }

    // Return true to continue the pipeline, false to send response to the client.
    return false;
}

extern "C" {
zia::api::Module* create() {
    zia::api::Module* mod = nullptr;
    try {
        mod = new modExample();
    }
    catch (...) {
        std::cerr << "Unable to create module: modExample" << std::endl;
    }
    return mod;
}
}