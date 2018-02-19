//
// Created by soszyn_h on 13/02/18.
//

#ifndef ZIA_MODEXAMPLE_HPP
# define ZIA_MODEXAMPLE_HPP


#include <iostream>
#include "sza-plus-plus/api/pp/conf.hpp"
#include "sza-plus-plus/api/pp/net.hpp"
#include "sza-plus-plus/api/pp/http.hpp"
#include "sza-plus-plus/api/pp/module.hpp"

class modStaticFiles : public zia::apipp::Module {
public:
    ~modStaticFiles() override = default;

    bool perform() override;

private:
    static std::string GetContentType(std::string const& name);
};


#endif // ZIA_MODEXAMPLE_HPP
