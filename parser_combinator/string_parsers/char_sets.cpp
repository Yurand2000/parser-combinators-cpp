#include "char_sets.hpp"

#include <cctype>
#include <cstdlib>

namespace parser_combinators::string {

CharSetParser<char, string_slice, std::string, Wrapper<>>::CharSetParser(std::function<bool(char)> predicate, std::function<std::string(char)> error_fn)
    : predicate(predicate), error_fn(error_fn) { }

ParserRes<char, string_slice, std::string> CharSetParser<char, string_slice, std::string, Wrapper<>>::operator()(string_slice stream) const {
    auto stream_i = stream.begin;
        
    if (stream_i == stream.end) {
        return Error(std::format("empty stream"));
    } else if (predicate(*stream_i)) {            
        auto split = stream.split_at(stream_i + 1);
        return Ok(*stream_i, split.first, split.second);
    } else {
        return Error(error_fn(*stream_i));
    }
}

CharSetParser<char, string_slice, std::string, Wrapper<>> parse_alnum() {
    return CharSetParser<char, string_slice, std::string, Wrapper<>>(
        [](char ch) { return std::isalnum(ch) > 0; },
        [](char ch) { return std::format("alnum not found"); }
    );
}

CharSetParser<char, string_slice, std::string, Wrapper<>> parse_alpha() {
    return CharSetParser<char, string_slice, std::string, Wrapper<>>(
        [](char ch) { return std::isalpha(ch) > 0; },
        [](char ch) { return std::format("alpha not found"); }
    );
}

CharSetParser<char, string_slice, std::string, Wrapper<>> parse_lower() {
    return CharSetParser<char, string_slice, std::string, Wrapper<>>(
        [](char ch) { return std::islower(ch) > 0; },
        [](char ch) { return std::format("lower not found"); }
    );
}

CharSetParser<char, string_slice, std::string, Wrapper<>> parse_upper() {
    return CharSetParser<char, string_slice, std::string, Wrapper<>>(
        [](char ch) { return std::isupper(ch) > 0; },
        [](char ch) { return std::format("upper not found"); }
    );
}

CharSetParser<char, string_slice, std::string, Wrapper<>> parse_digit() {
    return CharSetParser<char, string_slice, std::string, Wrapper<>>(
        [](char ch) { return std::isdigit(ch) > 0; },
        [](char ch) { return std::format("digit not found"); }
    );
}

CharSetParser<char, string_slice, std::string, Wrapper<>> parse_digit_1_9() {
    return CharSetParser<char, string_slice, std::string, Wrapper<>>(
        [](char ch) { return std::isdigit(ch) > 0 && ch != '0'; },
        [](char ch) { return std::format("digit not found"); }
    );
}

CharSetParser<char, string_slice, std::string, Wrapper<>> parse_xdigit() {
    return CharSetParser<char, string_slice, std::string, Wrapper<>>(
        [](char ch) { return std::isxdigit(ch) > 0; },
        [](char ch) { return std::format("xdigit not found"); }
    );
}

CharSetParser<char, string_slice, std::string, Wrapper<>> parse_space() {
    return CharSetParser<char, string_slice, std::string, Wrapper<>>(
        [](char ch) { return std::isspace(ch) > 0; },
        [](char ch) { return std::format("space not found"); }
    );
}

}