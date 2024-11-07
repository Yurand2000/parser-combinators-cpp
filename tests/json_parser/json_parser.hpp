#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

#include "json.hpp"

namespace json::parser {

using JsonNumberIntegral = uint64_t;
using JsonNumberFraction = uint64_t;
using JsonNumberExponent = uint64_t;
using JsonObjectField = std::tuple<std::string, std::shared_ptr<JsonValue>>;

// conversion functions
JsonNumberExponent parse_json_number_exponent(std::tuple<bool, std::vector<char>>&& exponent);
JsonNumberFraction parse_json_number_fraction(std::vector<char>&& fraction);
JsonNumberIntegral parse_json_number_integral(std::tuple<bool, std::vector<char>>&& integral);
JsonNumber parser_json_number(std::tuple<JsonNumberIntegral, JsonNumberFraction, JsonNumberExponent>&& data);

char parse_unicode_char(std::tuple<parser_combinators::unit, char, char, char, char>&& value);
std::string parse_string_from_vector(std::vector<char>&& chars);

JsonValue parse_json_value_variant(std::variant<JsonString, JsonNumber, JsonObject, JsonArray, JsonBool, JsonNull>&& value);
JsonArray parse_json_array(std::variant<std::vector<JsonValue>, parser_combinators::unit>&& value);
std::shared_ptr<JsonValue> make_shared_json_value(JsonValue&& value);
JsonObject parse_json_object_fields(std::vector<JsonObjectField>&& value);

}