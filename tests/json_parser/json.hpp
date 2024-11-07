#pragma once

#include <string>
#include "parser_combinator/parser.hpp"

namespace json {

// json data types
struct JsonValue;

struct JsonNull {};
using JsonBool = bool;
using JsonNumber = double;
using JsonChar = char;
using JsonString = std::string;

struct JsonObject {
    std::unordered_map<std::string, std::shared_ptr<JsonValue>> fields;

    JsonObject() {}
    JsonObject(std::vector<std::tuple<std::string, std::shared_ptr<JsonValue>>>&& values) {
        for (auto it = std::make_move_iterator(values.begin()), 
                  end = std::make_move_iterator(values.end());
                  it != end;
                  it++
        ) {
            fields.emplace(std::make_pair(std::get<0>(*it), std::get<1>(*it)));
        }
    }
};

struct JsonArray {
    std::vector<JsonValue> values;

    JsonArray() {}
    JsonArray(std::vector<JsonValue>&& values) : values(values) {}
};

struct JsonValue {
    std::variant<JsonNull, JsonBool, JsonNumber, JsonString, JsonObject, JsonArray> value;

    JsonValue() : value(std::in_place_index<0>, JsonNull()) {}
};

}

namespace json::parser {

parser_combinators::ParserRes<JsonValue, parser_combinators::string::string_slice, std::string> parse(std::string&& json);

}

