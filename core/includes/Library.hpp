#pragma once

#include <iostream>

class Library {
private:
    void *handler;

public:
    Library() {};
    Library(std::string const& libname);
    ~Library();

    void    *getHandler();
    void    *loadSym(std::string const& symName);
    bool    loadLibrary(std::string const& libName);
};
