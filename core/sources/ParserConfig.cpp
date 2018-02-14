//
// Created by delacr_a on 13/02/18.
//

#include <fstream>
#include <iostream>
#include <string>
#include "ParserConfig.hpp"


//<editor-fold>			CONSTRUCTOR AND DESTRUCTOR

ParserConfig::ParserConfig() {

}

ParserConfig::~ParserConfig() {

}
//</editor-fold>



//<editor-fold>			METHODS

void ParserConfig::parse(std::string path)
{
    std::string file_content;
    nlohmann::json parsed;

    file_content = getFileContent(path);
	if (file_content.empty())
		return;
	parsed = nlohmann::json::parse(file_content);
	for (auto it : parsed["module"])
		std::cout << it << std::endl;
}

std::string ParserConfig::getFileContent(std::string path) const
{
    std::ifstream file;
    std::string content = "";
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
