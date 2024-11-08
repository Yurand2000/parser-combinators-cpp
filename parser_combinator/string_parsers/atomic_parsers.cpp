#include "atomic_parsers.hpp"

namespace parser_combinators::string {

StringParser<std::string, string_slice, std::string, Wrapper<>>::StringParser(std::string string) : string(string) { }

ParserRes<std::string, string_slice, std::string> StringParser<std::string, string_slice, std::string, Wrapper<>>::operator()(string_slice stream) const {
    auto stream_i = stream.begin;
    auto token_i = this->string.cbegin();

    while (token_i != this->string.cend()) {
        if (stream_i == stream.end || *token_i != *stream_i) {
            return Error(std::format("\"{}\" string not found", this->string));
        }
        token_i++;
        stream_i++;
    }

    auto split = stream.split_at(stream_i);
    return Ok(this->string, split.first, split.second);
}

StringParser<std::string, string_slice, std::string, Wrapper<>> parse_string(std::string string) {
    return StringParser<std::string, string_slice, std::string, Wrapper<>>(string);
}

CharParser<char, string_slice, std::string, Wrapper<>>::CharParser(char token) : token(token) { }

ParserRes<char, string_slice, std::string> CharParser<char, string_slice, std::string, Wrapper<>>::operator()(string_slice stream) const {
    auto stream_i = stream.begin;

    if (stream_i != stream.end && *stream_i == token) {
        stream_i++;
        
        auto split = stream.split_at(stream_i);
        return Ok(token, split.first, split.second);
    } else {
        return Error(std::format("char '{}' not found", token));
    }
}

CharParser<char, string_slice, std::string, Wrapper<>> parse_char(char token) {
    return CharParser<char, string_slice, std::string, Wrapper<>>(token);
}

NotCharParser<char, string_slice, std::string, Wrapper<>>::NotCharParser(char token) : token(token) { }

ParserRes<char, string_slice, std::string> NotCharParser<char, string_slice, std::string, Wrapper<>>::operator()(string_slice stream) const {
    auto stream_i = stream.begin;

    if (stream_i != stream.end && *stream_i != token) {
        auto ch = *stream_i;
        stream_i++;
        
        auto split = stream.split_at(stream_i);
        return Ok(ch, split.first, split.second);
    } else {
        return Error(std::format("char '{}' found", token));
    }
}

NotCharParser<char, string_slice, std::string, Wrapper<>> parse_not_char(char token) {
    return NotCharParser<char, string_slice, std::string, Wrapper<>>(token);
}

}