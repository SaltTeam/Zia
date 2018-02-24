
#pragma once

#include <string>
#include <vector>

namespace core {
    class Utils {
    public:
        static std::string join(const std::vector<std::string> &elements, const std::string &separator);

        static std::vector<std::string> split(const std::string &s, const char separator);

        static std::vector<std::string> split(const std::string &s, const std::string &separator);
    };
}
