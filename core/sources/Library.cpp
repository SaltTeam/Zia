#if isLinux
#include <dlfcn.h>
#endif

#include <Windows.h>
#include "Library.hpp"

Library::Library(std::string const& libname) {
#if isLinux
    handler = dlopen(libname.c_str(), RTLD_LAZY);
    if (handler == NULL)
        std::cout << dlerror() << std::endl;
#else
    handler = LoadLibrary(libname.c_str());
    if (handler == NULL)
        std::cout << "unable to load dll" << std::endl;
#endif
}

Library::~Library() {
#if isLinux
    if (handler != NULL)
        dlclose(handler);
#endif
}

void* Library::loadSym(std::string const& symName) {
#if isLinux
    return dlsym(handler, symName.c_str())
#else
	return GetProcAddress((HINSTANCE)handler, symName.c_str());
#endif

}

void* Library::getHandler() {
    return handler;
}

bool Library::loadLibrary(std::string const& libName) {
#if isLinux
    handler = dlopen(libName.c_str(), RTLD_LAZY);
    if (handler == NULL)
        return false;
    return true;
#else
	return true;
#endif
}