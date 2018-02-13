//
// Created by delacr_a on 13/02/18.
//

#ifndef ZIA_PASERCONFIG_HPP
#define ZIA_PASERCONFIG_HPP

#include "json.hpp"

class ParserConfig {

public:
    ParserConfig();
    ~ParserConfig();
    void parse(std::string path);

protected:
    std::string getFileContent(std::string path) const;
};


#endif //ZIA_PASERCONFIG_HPP
