#pragma once

#include <iostream>

class Library {
private:
    void *handler;

public:
    Library() {};
    Library(std::string libname);
    ~Library();

    void    *getHandler() const;
    void    *loadSym(std::string symName);
    bool    loadLibrary(std::string libName);
};
