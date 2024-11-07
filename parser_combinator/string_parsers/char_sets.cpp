#include "char_sets.hpp"

#include <cctype>
#include <cstdlib>

namespace parser_combinators::string {

using CharSetParser = Parser<char, string_slice, std::string>;

static CharSetParser parse_char_predicate(std::function<bool(char)> predicate, std::function<std::string(char)> error_fn) {
    return [predicate, error_fn](string_slice stream) -> ParserRes<char, string_slice, std::string> {
        auto stream_i = stream.begin;
        
        if (stream_i == stream.end) {
            return Error(std::format("empty stream"));
        } else if (predicate(*stream_i)) {            
            auto split = stream.split_at(stream_i + 1);
            return Ok(*stream_i, split.first, split.second);
        } else {
            return Error(error_fn(*stream_i));
        }
    };
}

Parser<char, string_slice, std::string> parse_alnum() {
    return parse_char_predicate([](char ch) { return std::isalnum(ch) > 0; }, [](char ch) {
        return std::format("alnum not found");
    });
}

Parser<char, string_slice, std::string> parse_alpha() {
    return parse_char_predicate([](char ch) { return std::isalpha(ch) > 0; }, [](char ch) {
        return std::format("alpha not found");
    });
}

Parser<char, string_slice, std::string> parse_lower() {
    return parse_char_predicate([](char ch) { return std::islower(ch) > 0; }, [](char ch) {
        return std::format("lower not found");
    });
}

Parser<char, string_slice, std::string> parse_upper() {
    return parse_char_predicate([](char ch) { return std::isupper(ch) > 0; }, [](char ch) {
        return std::format("upper not found");
    });
}

Parser<char, string_slice, std::string> parse_digit() {
    return parse_char_predicate([](char ch) { return std::isdigit(ch) > 0; }, [](char ch) {
        return std::format("digit not found");
    });
}

Parser<char, string_slice, std::string> parse_digit_1_9() {
    return parse_char_predicate([](char ch) { return std::isdigit(ch) > 0 && ch != '0'; }, [](char ch) {
        return std::format("digit not found");
    });
}

Parser<char, string_slice, std::string> parse_xdigit() {
    return parse_char_predicate([](char ch) { return std::isxdigit(ch) > 0; }, [](char ch) {
        return std::format("xdigit not found");
    });
}

Parser<char, string_slice, std::string> parse_space() {
    return parse_char_predicate([](char ch) { return std::isspace(ch) > 0; }, [](char ch) {
        return std::format("space not found");
    });
}

}