//
// Created by delacr_a on 13/02/18.
//

#include <fstream>
#include <iostream>
#include <string>

#include "ParserConfig.hpp"

using namespace zia::apipp;
using namespace nlohmann;


//<editor-fold>			CONSTRUCTOR AND DESTRUCTOR

ParserConfig::ParserConfig() {

}

ParserConfig::~ParserConfig() {

}
//</editor-fold>


//<editor-fold>			UTILS

std::string cleanString(std::string value)
{
	if (value.at(0) == '"')
		value.erase(0);
	if (value.at(value.length() - 1) == '"')
		value.erase(value.length() - 1);
	return value;
}
//</editor-fold>


//<editor-fold>			METHODS

template<typename T>
void ParserConfig::putValueInElem(T value, zia::apipp::ConfElem &elem, const std::string &key) const
{
	if (elem.getType() == ConfElem::Type::Array)
		elem.push(ConfElem().set(value));
	else if (elem.getType() == ConfElem::Type::Map)
		elem.set_at(key, ConfElem().set(value));
	else
		elem.set(value);
}

void ParserConfig::manageValue(json::value_type &value, zia::apipp::ConfElem &elem, std::string const &key) const
{
	if (value.is_string())
	{
		value = cleanString(value.get<std::string>());
		putValueInElem(value.get<std::string>(), elem, key);
	}
	else if (value.is_boolean())
		putValueInElem(value.get<bool>(), elem, key);
	else if (value.is_number_integer())
		putValueInElem(value.get<long long int>(), elem, key);
	else if (value.is_number_float())
		putValueInElem(value.get<double>(), elem, key);
}

void ParserConfig::putValues(json::object_t object, ConfElem &elem, std::string const & key) const
{
	std::shared_ptr<ConfElem> map;
	map = std::make_shared<ConfElem>();
	map->set(ConfMap());

	if (elem.getType() == ConfElem::Type::Array)
		elem.push(map);
	else if (elem.getType() == ConfElem::Type::Map)
		elem.set_at(key, map);
	else
		elem = *map;

	for (auto &value : object)
		chooseCorrectFunction(value.second, *map, value.first);
}

void ParserConfig::putValues(json::array_t array, ConfElem &elem, std::string const & key) const
{
	std::shared_ptr<ConfElem> array_;
	array_ = std::make_shared<ConfElem>();
	array_->set(ConfArray());

	if (elem.getType() == ConfElem::Type::Array)
		elem.push(array_);
	else if (elem.getType() == ConfElem::Type::Map)
		elem.set_at(key, array_);
	else
		elem = *array_;

	for (auto &value : array)
		chooseCorrectFunction(value, *array_);
}

void ParserConfig::chooseCorrectFunction(json::value_type &value, zia::apipp::ConfElem &elem, const std::string &key) const
{
	if (value.is_null())
	{ }
	else if (value.is_object())
		putValues(value.get<json::object_t>(), elem, key);
	else if (value.is_array())
		putValues(value.get<json::array_t>(), elem, key);
	else
		manageValue(value, elem, key);
}

ConfElem ParserConfig::parse(std::string const &path)
{
    json parsed;
	ConfElem conf;

	try {
		parsed = json::parse(getFileContent(path));
	}
	catch (std::exception &e) {
		return conf;
	}

	chooseCorrectFunction(parsed, conf);

	std::cout << conf << std::endl;

	return conf;
}

std::string ParserConfig::getFileContent(std::string const &path) const
{
    std::ifstream file;
    std::string content;
    std::string line;

    file.open(path);
    if (file && file.is_open())
    {
		while (getline(file, line))
		    content += line + "\n";
		file.close();
    }
    else
		std::cerr << "Cannot open file '" + path + "'" << std::endl;

    return content;
}
//</editor-fold>
