#pragma once

#include <istream>

namespace parse {

void test_parse();

void parse(std::istream& in);
void parse_file(const std::string &filename);

}