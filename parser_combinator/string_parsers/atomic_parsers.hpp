#pragma once

#include <string>
#include <format>

#include "string_slice.hpp"
#include "../generic_parsers/mod.hpp"

namespace parser_combinators::string {

Parser<std::string, string_slice, std::string> parse_string(std::string token);
Parser<char, string_slice, std::string> parse_char(char token);
Parser<char, string_slice, std::string> parse_not_char(char token);

}