//
// Created by soszyn_h on 13/02/18.
//

#ifdef WIN32
#include "stdafx.h"
#include <Windows.h>
#endif

#include <fstream>
#include "sza-plus-plus/api/module.h"
#include "modules/MIME_types.hpp"
#include "modStaticFiles.hpp"

bool modStaticFiles::perform() {
    this->response->useStandardData();

    std::string uri{};
    try {
        uri = this->conf.get_at("basedir").get<std::string>() + this->request->uri;
    }
    catch (...) {
        this->response
                ->setStatus(500, "Internal Server Error")
                ->addHeader("Content-Length", "45")
                ->addHeader("Content-Type", "text/html")
                ->setStandardData("<h1>500</h1><br/><p>Internal Server Error</p>");
        return false;
    }
    std::ifstream ifstream{};
    auto ss = std::ostringstream{};

    try {
        ifstream.open(uri, std::ifstream::in | std::ifstream::binary);
        if (!ifstream.is_open())
            throw std::exception();
        ss << ifstream.rdbuf();
        ifstream.close();
    }
    catch (...){
        this->response
                ->setStatus(404, "Not Found")
                ->addHeader("Content-Length", "33")
                ->addHeader("Content-Type", "text/html")
                ->setStandardData("<h1>404</h1><br/><p>Not Found</p>");
        return false;
    }
    this->response
            ->setStatus(200, "OK")
            ->addHeader("Content-Type", GetContentType(uri))
            ->addHeader("Content-Length", std::to_string(ss.str().length()))
            ->setStandardData(ss.str());
    return false;
}

std::string modStaticFiles::GetContentType(std::string const& name) {
    auto pos = name.find_last_of('.');
    auto ext = name.substr(pos, std::string::npos);
    auto type = mime_types.find(ext);
    if (type == mime_types.end())
        return std::string("text/plain");
    return type->second;
}

extern "C" {
#ifdef WIN32
__declspec(dllexport) zia::api::Module* create() {
#else
zia::api::Module* create() {
#endif
    zia::api::Module* mod = nullptr;
    try {
        mod = new modStaticFiles();
    }
    catch (...) {
        std::cerr << "Unable to create module: modStaticFiles" << std::endl;
    }
    return mod;
}
}