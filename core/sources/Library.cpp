#ifdef WIN_32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include "Library.hpp"

Library::Library(std::string const& libName) {
#ifdef WIN_32
    handler = LoadLibrary(libName.c_str());
    if (handler == NULL)
        std::cout << "unable to load dll" << std::endl;
#else
    handler = dlopen(libName.c_str(), RTLD_LAZY);
    if (handler == NULL)
        std::cout << dlerror() << std::endl;
#endif
}

Library::~Library() {
#ifdef WIN_32
	if (handler != NULL)
		FreeLibrary((HINSTANCE)(handler));
#else
    if (handler != NULL)
        dlclose(handler);
#endif
}

void* Library::loadSym(std::string const& symName) {
#ifdef WIN_32
	return GetProcAddress((HINSTANCE)handler, symName.c_str());
#else
    return dlsym(handler, symName.c_str());
#endif
}

void* Library::getHandler() {
    return handler;
}

bool Library::loadLibrary(std::string const& libName) {
#ifdef WIN_32
	handler = LoadLibrary(libName.c_str());
	if (handler == NULL)
		return false;
#else
    handler = dlopen(libName.c_str(), RTLD_LAZY);
    if (handler == NULL)
        return false;
    return true;
#endif
}