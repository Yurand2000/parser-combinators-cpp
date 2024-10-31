#pragma once

#include <string>
#include <format>

#include "string_slice.hpp"
#include "../generic_parsers/mod.hpp"

namespace parser_combinators::string {

Parser<std::string, string_slice, std::string> parse_string(std::string token) {
    return [token](string_slice stream) -> ParserRes<std::string, string_slice, std::string> {
        auto stream_i = stream.begin;
        auto token_i = token.cbegin();

        while (token_i != token.cend()) {
            if (stream_i == stream.end || *token_i != *stream_i) {
                return Error(std::format("\"{}\" token not found", token));
            }
            token_i++;
            stream_i++;
        }

        auto split = stream.split_at(stream_i);
        return Ok(token, split.first, split.second);
    };
}

Parser<char, string_slice, std::string> parse_char(char token) {
    return [token](string_slice stream) -> ParserRes<char, string_slice, std::string> {
        auto stream_i = stream.begin;

        if (stream_i != stream.end && *stream_i == token) {
            stream_i++;
            
            auto split = stream.split_at(stream_i);
            return Ok(token, split.first, split.second);
        } else {
            return Error(std::format("char '{}' not found", token));
        }
    };
}

Parser<char, string_slice, std::string> parse_not_char(char token) {
    return [token](string_slice stream) -> ParserRes<char, string_slice, std::string> {
        auto stream_i = stream.begin;

        if (stream_i != stream.end && *stream_i != token) {
            stream_i++;
            
            auto split = stream.split_at(stream_i);
            return Ok(token, split.first, split.second);
        } else {
            return Error(std::format("char '{}' found", token));
        }
    };
}

}