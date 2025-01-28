#include "driver.h"
#include <iostream>
#include "parser.hpp"

int DBDriver::parse(const std::string &file)
{
  location.initialize(&file);
  scan_begin(file);
  yy::parser parse(*this);
  int res = parse();
  scan_end();
  return res;
}
