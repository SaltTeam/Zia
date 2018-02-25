//
// Created by soszyn_h on 19/02/18.
//

#include "modStaticFiles/modStaticFiles.hpp"
#include "MIME_types.hpp"

using namespace std::literals::string_literals;

typedef void (* test_t)();

#define TEST_NAME "static_files"

void test1() {

    modStaticFiles module {};
    zia::api::HttpDuplex duplex {};

    duplex.req.uri = "/README.md";

    auto conf = zia::apipp::ConfElem()
            .set(zia::apipp::ConfMap())
            .set_at("basedir", "/home/soszyn_h/rendu/Zia/"s);

    module.config(conf.toBasicConfig());

    module.exec(duplex);

//    for (const auto& item: duplex.resp.headers) {
//        std::cout << item.first << "=" << item.second << std::endl;
//    }
//    std::cout << std::endl;
//
//    for (const auto& item: duplex.resp.body) {
//        std::cout << static_cast<char>(item);
//    }
//    std::cout << std::endl;
}

void test2() {

    modStaticFiles module {};
    zia::api::HttpDuplex duplex {};

    duplex.req.uri = "/README.md";

    module.exec(duplex);

//    for (const auto& item: duplex.resp.headers) {
//        std::cout << item.first << "=" << item.second << std::endl;
//    }
//    std::cout << std::endl;
//
//    for (const auto& item: duplex.resp.body) {
//        std::cout << static_cast<char>(item);
//    }
//    std::cout << std::endl;
}

void test3() {

    modStaticFiles module {};
    zia::api::HttpDuplex duplex {};

    duplex.req.uri = "/tototatatiti";

    auto conf = zia::apipp::ConfElem().set(zia::apipp::ConfMap());
    conf.set_at("basedir", "/home/soszyn_h/rendu/Zia/"s);

    module.config(conf.toBasicConfig());

    module.exec(duplex);

//    for (const auto& item: duplex.resp.headers) {
//        std::cout << item.first << "=" << item.second << std::endl;
//    }
//    std::cout << std::endl;
//
//    for (const auto& item: duplex.resp.body) {
//        std::cout << static_cast<char>(item);
//    }
//    std::cout << std::endl;
}

test_t tests[] = {
        test1,
        test2,
        test3,
        nullptr
};

int main() {
    auto ret = 0;
    std::cout << TEST_NAME << ':' << std::endl;
    for (int i = 0; tests[i] != nullptr; ++i) {
        std::cout << "\tTEST n°" /* << std::to_string(i) << std::endl */;
        try {
            tests[i]();
            std::cout << std::to_string(i) << ": PASSED" << std::endl;
        }
        catch (std::exception const& e) {
            std::cerr << e.what() << std::endl;
            std::cout << std::to_string(i) << ": FAILED" << std::endl;
            ret = 1;
        }
    }
    return ret;
}