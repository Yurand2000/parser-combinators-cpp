#pragma once

#include <string>
#include <format>

#include "string_slice.hpp"
#include "../generic_parsers/mod.hpp"

namespace parser_combinators::string {

template <typename Res, slice Slice, typename Err, typename Other> struct StringParser {};
template <typename Res, slice Slice, typename Err, typename Other> struct CharParser {};
template <typename Res, slice Slice, typename Err, typename Other> struct NotCharParser {};

template <>
struct StringParser<std::string, string_slice, std::string, Wrapper<>> {
private:
    std::string string;

public:
    StringParser(std::string string);

    ParserRes<std::string, string_slice, std::string> operator()(string_slice) const;
};

template <>
struct CharParser<char, string_slice, std::string, Wrapper<>> {
private:
    char token;

public:
    CharParser(char token);

    ParserRes<char, string_slice, std::string> operator()(string_slice) const;
};

template <>
struct NotCharParser<char, string_slice, std::string, Wrapper<>> {
private:
    char token;

public:
    NotCharParser(char token);

    ParserRes<char, string_slice, std::string> operator()(string_slice) const;
};

StringParser<std::string, string_slice, std::string, Wrapper<>>  parse_string(std::string token);
CharParser<char, string_slice, std::string, Wrapper<>>    parse_char(char token);
NotCharParser<char, string_slice, std::string, Wrapper<>> parse_not_char(char token);

}