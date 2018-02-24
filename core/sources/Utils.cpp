
#include "Utils.hpp"
#include <iostream>
#include <iterator>
#include <sstream>

namespace core {
    std::string Utils::join(const std::vector<std::string> &elements, const std::string &separator) {
        switch (elements.size()) {
            case 0:
                return "";
            case 1:
                return elements[0];
            default:
                std::ostringstream os;
                std::copy(elements.begin(), elements.end() - 1,
                          std::ostream_iterator<std::string>(os, separator.c_str()));
                os << *elements.rbegin();
                return os.str();
        }
    }

    std::vector<std::string> Utils::split(const std::string &s, const char separator) {
        std::vector<std::string> result;

        std::string::const_iterator cur = s.begin();
        std::string::const_iterator beg = s.begin();
        bool added = false;
        while (cur < s.end()) {
            if (*cur == separator) {
                if (!added) {
                    added = true;
                    result.insert(result.end(), std::string(beg, cur));
                }
                beg = ++cur;
            } else {
                added = false;
                cur++;
            }
        }
        result.insert(result.end(), std::string(beg, cur));
        return result;
    }

    std::vector<std::string> Utils::split(const std::string &s, const std::string &separator) {
        std::vector<std::string> result;

        std::string::size_type cur = 0;
        std::string::size_type beg = 0;
        while (cur < s.length()) {
            if (s.substr(cur, separator.length()) == separator) {
                result.push_back(s.substr(beg, cur - beg));
                beg = cur + separator.length();
                cur = beg;
            } else {
                cur++;
            }
        }
        result.push_back(s.substr(beg));
        return result;
    }
}
