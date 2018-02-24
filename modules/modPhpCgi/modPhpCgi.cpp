#include <fstream>
#include <cstring>
#include <unistd.h>
#include <exception>
#include "sza-plus-plus/api/module.h"
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
    aArgs.emplace_back("/test.php");

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
    aEnv.emplace_back("REDIRECT=true");
    aEnv.emplace_back("REQUEST_METHOD=POST");
    aEnv.emplace_back("CONTENT_TYPE=application/x-www-form-urlencoded;charset=utf-8");
    aEnv.emplace_back("SCRIPT_FILENAME=/test.php");
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
    auto body = new char[this->request->body.size() + 1];
    strncpy(body, this->request->body.c_str(), this->request->body.size() + 1);
    char *echocmd[] = {"echo", body, nullptr};
    int pipeForStdIn[2];
    pid_t pid;
    int res;

    pipe(pipeForStdIn);

    if ((pid = fork()) == -1)
        throw "error can't do a fork";

    if (pid > 0) {
        dup2(pipeForStdIn[1], 1);
        close(pipeForStdIn[0]);
        res = execvp(echocmd[0], echocmd);
    } else {
        dup2(pipeForStdIn[0], 0);
        close(pipeForStdIn[1]);
        res = execve(sysCline[0], sysCline, sysEnv);
    }
    close(pipeForStdIn[1]);


    //Actually it should write on the output the body response.
}