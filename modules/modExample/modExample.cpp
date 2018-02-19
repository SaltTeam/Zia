//
// Created by soszyn_h on 13/02/18.
//

#ifdef WIN32
#include "../WinCompat/stdafx.h"
#include <Windows.h>
#endif

#include "sza-plus-plus/api/module.h"
#include "sza-plus-plus/api/pp/conf.hpp"
#include "modExample.hpp"

bool modExample::perform() {
    this->response->useStandardData();

    try {
        this->response
                ->setStatus(200, "OK")
                ->addHeader("Set-Cookie", "test")
                ->addHeader("Content-Length", std::to_string(this->conf.get_at("message")->get<std::string>().length()))
                ->setStandardData(this->conf.get_at("message")->get<std::string>());
    }
    catch (zia::apipp::Conf::InvalidAccess const& e) {
		(void)(e);
        this->response
                ->setStatus(500, "Internal Server Error")
                ->addHeader("Content-Length", "45")
                ->setStandardData("<h1>500</h1><br/><p>Internal Server Error</p>");
    }

    // Return true to continue the pipeline, false to send response to the client.
    return false;
}

extern "C" {
#ifdef WIN32
__declspec(dllexport) zia::api::Module* create() {
#else
zia::api::Module* create() {
#endif
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