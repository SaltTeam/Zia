#ifdef WIN32
#include "stdafx.h"
#include <Windows.h>
#endif

#include <fstream>
#include <cstring>
#include <unistd.h>
#include <exception>
#include <wait.h>
#include "sza-plus-plus/api/module.h"
#include "Utils.hpp"
#include "modules/MIME_types.hpp"
#include "modPhpCgi.hpp"

bool modPhpCgi::perform() {
    /*
     * Consctruct the exec cmd
     */
    char **sysCline = nullptr;
    char **sysEnv = nullptr;

    std::vector<std::string> aArgs;
    aArgs.emplace_back("/usr/bin/php-cgi");
    aArgs.emplace_back("");

    sysCline = new char*[aArgs.size() + 1];
    for (int i=0; i<aArgs.size(); i++) {
        sysCline[i] = new char[aArgs[i].size()+1];
        strncpy(sysCline[i], aArgs[i].c_str(), aArgs[i].size()+1);
    }
    sysCline[aArgs.size()] = nullptr;

    // Construct environment
    std::vector<std::string> aEnv;

    aEnv.emplace_back("GATEWAY_INTERFACE=CGI/1.1");
    aEnv.emplace_back("SERVER_PROTOCOL=HTTP/1.1");
    aEnv.emplace_back("QUERY_STRING=test=querystring");
    aEnv.emplace_back("REDIRECT_STATUS=200");
    aEnv.push_back("SCRIPT_FILENAME=" + this->conf.get_at("basedir").get<std::string>() + this->request->uri);
    aEnv.emplace_back("REDIRECT=true");
    aEnv.emplace_back("REQUEST_METHOD=POST");
    aEnv.emplace_back("CONTENT_TYPE=application/x-www-form-urlencoded;charset=utf-8");
    aEnv.push_back("CONTENT_LENGTH="+ std::to_string(this->request->body.length()));
    sysEnv = new char*[aEnv.size() + 1];
    for (int i=0; i<aEnv.size(); i++) {
        sysEnv[i] = new char[aEnv[i].size()+1];
        strncpy(sysEnv[i], aEnv[i].c_str(), aEnv[i].size() + 1);
    }
    sysEnv[aEnv.size()] = nullptr;
    /*
     * End construct
     */

    /*
     * Construct the program exec
     */
    int pipeForStdIn[2];
    int pipeForStdOut[2];
    pid_t pid;
    int res;
    int status;

    pipe(pipeForStdIn);
    pipe(pipeForStdOut);

    if ((pid = fork()) == -1)
        throw std::runtime_error("error can't do a fork");

    if (pid > 0) {
        //parent exec

        close(pipeForStdIn[0]);
        close(pipeForStdOut[1]);

        // ecrire sur l'entrée standard du child
        write(pipeForStdIn[1], this->request->body.c_str(), this->request->body.size());

        // lire l'output du child
        std::string response;
        char buf[1025];
        ssize_t ret;
        while ((ret = read(pipeForStdOut[0], buf, 1024)) >= 0) {
            if (ret != 1024) {
                buf[ret] = '\0';
               response += buf;
                break;
            }
            buf[1024] = '\0';
           response += buf;
        }
        if (ret == -1)
            throw std::runtime_error("error on read result of php-cgi action");


        // parse cgi response
        auto lines = core::Utils::split(response, "\r\n");

        unsigned i;
        for (i = 0; i < lines.size() && !lines[i].empty(); i++) {
            auto h = core::Utils::split(lines[i], ": ");
            auto values = core::Utils::split(h[1], ',');
            for (const auto &val: values)
                this->response->headers[h[0]].push_back(val);
        }
        this->response->statusCode = 200;
        this->response->statusReason = "OK";

        std::vector<std::string> vecBody;
        for (i += 1; i < lines.size(); i++) {
            vecBody.push_back(lines[i]);
        }
        std::string body = core::Utils::join(vecBody, "\r\n");
        this->response->body = body;

        // wait le child
        waitpid(pid, &status, 0);

    } else {

        //child exec
        close(pipeForStdIn[1]);
        close(pipeForStdOut[0]);

        dup2(pipeForStdIn[0], 0);
        dup2(pipeForStdOut[1], 1);

        res = execve(sysCline[0], sysCline, sysEnv);
    }
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
        mod = new modPhpCgi();
    }
    catch (...) {
        std::cerr << "Unable to create module: modPhpCgi" << std::endl;
    }
    return mod;
}
}