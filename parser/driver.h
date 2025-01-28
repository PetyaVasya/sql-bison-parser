#pragma once

#include <string>
#include <map>
#include "db/result.h"
#include "location.hh"

struct DBDriver
{
    std::optional<Result> result;
    yy::location location;

    DBDriver() = default;
    
    int parse(const std::string &file);
    void scan_begin(const std::string &file);
    void scan_end();
};