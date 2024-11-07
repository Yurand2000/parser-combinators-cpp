#pragma once

#include "../generic_parsers/mod.hpp"
#include "string_slice.hpp"

namespace parser_combinators::string {

Parser<std::vector<char>, string_slice, std::string> parse_whitespaces();

}