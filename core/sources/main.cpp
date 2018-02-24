#include "network/Addr.hpp"
#include "ParserConfig.hpp"
#include "core/includes/Core.hpp"
#include "Library.hpp"
#include <wait.h>

typedef zia::api::Module*(*moduleEntryPoint)() ;
typedef module::NetMod*(*netEntryPoint)(unsigned short) ;

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

std::string const getCorrectPathOfModuleLinux(std::string const &name, std::vector<std::shared_ptr<zia::apipp::ConfElem>> paths)
{
	std::string res;

	for (auto &it: paths)
	{
		if ((*it).getType() == zia::apipp::ConfElem::Type::String)
		{
			if (access(((*it).get<std::string>() + "/" + name + ".so").c_str(), F_OK) == 0)
				return (*it).get<std::string>() + "/" + name + ".so";
		}
	}

	return res;
}

int main() {

    init();

    ParserConfig parser;
    auto conf = parser.parse("./config.json");

    auto paths = conf["module_path"];
    auto virtualHosts = conf["virtualhost"];

    if (virtualHosts.getType() == zia::apipp::ConfElem::Type::Array && paths.getType() == zia::apipp::ConfElem::Type::Array)
    {
        for (auto &virtualHost : virtualHosts.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems)
        {
			#ifdef WIN32

			#else //LINUX
				pid_t pid = fork();
				if (pid == -1)
					std::cout << "Could not fork." << std::endl;
				else if (pid == 0)
				{
					std::vector<Module *> modules;

					for (auto &module : (*virtualHost)["modules"].get<std::shared_ptr<zia::apipp::ConfArray>>()->elems)
					{
						std::string path = getCorrectPathOfModuleLinux((*module)["name"].get<std::string>(),
																	   paths.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems);
						if (path.empty())
							continue;
						Library library = Library(path);
						auto ptr = reinterpret_cast<moduleEntryPoint>(library.loadSym("create"));
						modules.push_back(ptr());
						std::cout << path << std::endl;
					}

					Core::Core core = Core::Core(Core::Pipeline(modules));
                    if ((*virtualHost)["network"].get<std::string>() == "netSsl") {

                    }
                    else {
                        std::string path = getCorrectPathOfModuleLinux("libmodNetwork",
                                                                       paths.get<std::shared_ptr<zia::apipp::ConfArray>>()->elems);
                        if (!path.empty()){
                            Library lib = Library(path);
                            auto ptr = reinterpret_cast<netEntryPoint>(lib.loadSym("create"));
                            auto net = ptr(static_cast<unsigned short>((*virtualHost)["port"].get<long long int>()));
                            core.setNet(net);
                            std::cout << "net: " << net << std::endl;
                        }
                    }
                    core.run();
					exit(0);
				}
				else
				{
					int wstatus;
					waitpid(pid, &wstatus, WCONTINUED);
				}

			#endif
        }
    }

    end();
    return 0;
}