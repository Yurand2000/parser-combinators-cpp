#pragma once

#include "../generic_parsers/mod.hpp"
#include "string_slice.hpp"

namespace parser_combinators::string {

template <typename Res, slice Slice, typename Err, typename Other> struct CharSetParser {};

template <>
struct CharSetParser<char, string_slice, std::string, Wrapper<>> {
private:
    std::function<bool(char)> predicate;
    std::function<std::string(char)> error_fn;

public:
    CharSetParser(std::function<bool(char)>, std::function<std::string(char)>);

    ParserRes<char, string_slice, std::string> operator()(string_slice) const;
};

CharSetParser<char, string_slice, std::string, Wrapper<>> parse_alnum();
CharSetParser<char, string_slice, std::string, Wrapper<>> parse_alpha();
CharSetParser<char, string_slice, std::string, Wrapper<>> parse_lower();
CharSetParser<char, string_slice, std::string, Wrapper<>> parse_upper();
CharSetParser<char, string_slice, std::string, Wrapper<>> parse_digit();
CharSetParser<char, string_slice, std::string, Wrapper<>> parse_digit_1_9();
CharSetParser<char, string_slice, std::string, Wrapper<>> parse_xdigit();
CharSetParser<char, string_slice, std::string, Wrapper<>> parse_space();

}