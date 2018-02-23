//
// Created by delacr_a on 13/02/18.
//

#ifndef ZIA_PASERCONFIG_HPP
#define ZIA_PASERCONFIG_HPP

#include <variant>

#include "json.hpp"
#include "sza-plus-plus/api/pp/conf.hpp"

class ParserConfig {

public:
    ParserConfig();
    ~ParserConfig();
    zia::apipp::ConfElem parse(std::string const &path);

protected:
    std::string getFileContent(std::string const &path) const;
	template <typename T>
	void putValueInElem(T value, zia::apipp::ConfElem &elem, std::string const &key = "") const;
	void manageValue(nlohmann::json::value_type &value, zia::apipp::ConfElem &elem, std::string const &key = "") const;
	void chooseCorrectFunction(nlohmann::json::value_type &value, zia::apipp::ConfElem &elem, std::string const &key = "") const;
	void putValues(nlohmann::json::array_t array, zia::apipp::ConfElem &elem, std::string const & key = "") const;
	void putValues(nlohmann::json::object_t object, zia::apipp::ConfElem &elem, std::string const & key = "") const;
};


#endif //ZIA_PASERCONFIG_HPP
