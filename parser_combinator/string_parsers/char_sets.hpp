#pragma once

#include "../generic_parsers/mod.hpp"
#include "string_slice.hpp"

namespace parser_combinators::string {

Parser<char, string_slice, std::string> parse_alnum();
Parser<char, string_slice, std::string> parse_alpha();
Parser<char, string_slice, std::string> parse_lower();
Parser<char, string_slice, std::string> parse_upper();
Parser<char, string_slice, std::string> parse_digit();
Parser<char, string_slice, std::string> parse_digit_1_9();
Parser<char, string_slice, std::string> parse_xdigit();
Parser<char, string_slice, std::string> parse_space();

}