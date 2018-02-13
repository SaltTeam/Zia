#include <dlfcn.h>
#include "Library.hpp"

Library::Library(std::string libname) {
    handler = dlopen(libname.c_str(), RTLD_LAZY);
    if (handler == NULL)
        std::cout << dlerror() << std::endl;
}

Library::~Library() {
    if (handler != NULL)
        dlclose(handler);
}

void* Library::loadSym(std::string symName) {
    return dlsym(handler, symName.c_str())
}

void* Library::getHandler() const {
    return handler;
}

bool Library::loadLibrary(std::string libName) {
    handler = dlopen(libName.c_str(), RTLD_LAZY);
    if (handler == NULL)
        return false;
    return true;
}