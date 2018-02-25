#include "network/Addr.hpp"
#include "ParserConfig.hpp"
#include "core/includes/Core.hpp"
#include "Library.hpp"

#ifdef WIN32
#include <io.h>
#else
#include <wait.h>
#include <list>
#endif

typedef zia::api::Module *(*moduleEntryPoint)();

typedef module::NetMod *(*netEntryPoint)(unsigned short);

static void init() {
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif // WIN32
}

static void end() {
#ifdef WIN32
    WSACleanup();
#endif // WIN32
}

std::string const
getCorrectPathOfModule(std::string const &name, std::vector<std::shared_ptr<zia::apipp::ConfElem>> paths) {
    std::string res;

    for (auto &it: paths) {
        if ((*it).getType() == zia::apipp::ConfElem::Type::String) {
#ifdef WIN32

            if (_access(((*it).get<std::string>() + "/" + name + ".dll").c_str(), 06) == 0)
                return (*it).get<std::string>() + "/" + name + ".dll";
#else
            if (access(((*it).get<std::string>() + "/" + "lib" + name + ".so").c_str(), F_OK) == 0)
                return (*it).get<std::string>() + "/" + "lib" + name + ".so";
#endif
        }
    }

    return res;
}

void runCoreWithCorrectNet(std::vector<Module *> &modules, zia::apipp::ConfElem const &paths, zia::apipp::ConfElem const &virtualHost)
{
	Core::Core core = Core::Core(Core::Pipeline(modules));
	std::string path;
	if (virtualHost["network"].get<std::string>() == "netSsl") {
		path = getCorrectPathOfModule("modSSL",
									  paths.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems);
	} else {
		path = getCorrectPathOfModule("modNetwork",
									  paths.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems);
	}

	if (!path.empty()) {
		Library lib = Library(path);
		auto ptr = reinterpret_cast<netEntryPoint>(lib.loadSym("create"));
		auto net = ptr(static_cast<unsigned short>(virtualHost["port"].get<long long int>()));
		core.setNet(net);
		core.run();
	}
}

void loadModulesAndRun(zia::apipp::ConfElem const &paths, zia::apipp::ConfElem const &virtualHost)
{
	std::vector<Module *> modules;
	for (auto &module : (virtualHost["modules"]).get<std::shared_ptr<zia::apipp::ConfArray>>()->elems)
	{
		std::string path = getCorrectPathOfModule((*module)["name"].get<std::string>(),
												  paths.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems);
		if (path.empty())
			continue;
		Library library = Library(path);
		auto ptr = reinterpret_cast<moduleEntryPoint>(library.loadSym("create"));
		auto mod = ptr();
#ifndef WIN32
		if (mod == nullptr)
			break;
#endif
		mod->config((*module)["settings"].toBasicConfig());
		modules.push_back(mod);
	}

	runCoreWithCorrectNet(modules, paths, virtualHost);
}

int main() {

    init();

    ParserConfig parser;
    auto conf = parser.parse("./config.json");

    auto paths = conf["module_path"];
    auto virtualHosts = conf["virtualhost"];

    if (virtualHosts.getType() == zia::apipp::ConfElem::Type::Array &&
        paths.getType() == zia::apipp::ConfElem::Type::Array) {
#ifdef WIN32
        auto &virtualHost = (virtualHosts.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems).front();
		try {
			loadModulesAndRun(paths, *virtualHost);
		}
		catch (std::exception &e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}
#else //LINUX
		std::list<pid_t> childs;
        for (auto &virtualHost : virtualHosts.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems) {
            pid_t pid = fork();
			childs.push_back(pid);
            if (pid == -1)
                std::cout << "Could not fork." << std::endl;
            else if (pid == 0) {
				try {
					loadModulesAndRun(paths, *virtualHost);
				}
				catch (std::exception &e) {
					std::cout << "Exception: " << e.what() << std::endl;
				}
                exit(0);
            }
        }
		for (auto &pid : childs)
		{
			int wstatus;
			waitpid(pid, &wstatus, WCONTINUED);
		}
#endif
    }

    end();
    return 0;
}