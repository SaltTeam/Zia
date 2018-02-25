#pragma once

#include <iostream>
#include "sza-plus-plus/api/pp/conf.hpp"
#include "sza-plus-plus/api/pp/net.hpp"
#include "sza-plus-plus/api/pp/http.hpp"
#include "sza-plus-plus/api/pp/module.hpp"

class modPhpCgi : public zia::apipp::Module {
public:
    ~modPhpCgi() override = default;

    bool perform() override;
};