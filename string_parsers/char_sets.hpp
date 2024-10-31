#pragma once
#include <cctype>
#include <cstdlib>

#include "atomic_parsers.hpp"

namespace parser_combinators::string {

Parser<char, string_slice, std::string> parse_char_predicate(std::function<bool(char)> predicate, std::function<std::string(char)> error_fn) {
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

static const auto parse_alnum = 
    parse_char_predicate([](char ch) { return std::isalnum(ch) > 0; }, [](char ch) {
        return std::format("alnum not found");
    });

static const auto parse_alpha = 
    parse_char_predicate([](char ch) { return std::isalpha(ch) > 0; }, [](char ch) {
        return std::format("alpha not found");
    });

static const auto parse_lower = 
    parse_char_predicate([](char ch) { return std::islower(ch) > 0; }, [](char ch) {
        return std::format("lower not found");
    });

static const auto parse_upper = 
    parse_char_predicate([](char ch) { return std::isupper(ch) > 0; }, [](char ch) {
        return std::format("upper not found");
    });

static const auto parse_digit = 
    parse_char_predicate([](char ch) { return std::isdigit(ch) > 0; }, [](char ch) {
        return std::format("digit not found");
    });

static const auto parse_digit_1_9 = 
    parse_char_predicate([](char ch) { return std::isdigit(ch) > 0 && ch != '0'; }, [](char ch) {
        return std::format("digit not found");
    });

static const auto parse_xdigit = 
    parse_char_predicate([](char ch) { return std::isxdigit(ch) > 0; }, [](char ch) {
        return std::format("xdigit not found");
    });

static const auto parse_space = 
    parse_char_predicate([](char ch) { return std::isspace(ch) > 0; }, [](char ch) {
        return std::format("space not found");
    });

}