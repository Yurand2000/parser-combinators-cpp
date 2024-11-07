#include <cmath>

#include "parser_combinator/parser.hpp"
#include "json.hpp"
#include "json_parser.hpp"

using namespace parser_combinators;
using namespace parser_combinators::string;

namespace json::parser {

JsonNumberExponent parse_json_number_exponent(std::tuple<bool, std::vector<char>>&& exponent) {
    auto sign = std::get<0>(exponent);
    auto digits = std::get<1>(exponent);

    std::string digits_string(digits.begin(), digits.end());
    JsonNumberIntegral number = std::atoll(digits_string.c_str());
    if (sign) {
        return number;
    } else {
        return -number;
    }
}

JsonNumberFraction parse_json_number_fraction(std::vector<char>&& fraction) {
    std::string digits_string(fraction.begin(), fraction.end());
    return std::atoll(digits_string.c_str());
}

JsonNumberIntegral parse_json_number_integral(std::tuple<bool, std::vector<char>>&& integral) {
    auto sign = std::get<0>(integral);
    auto digits = std::get<1>(integral);

    std::string digits_string(digits.begin(), digits.end());
    JsonNumberIntegral number = std::atoll(digits_string.c_str());
    if (sign) {
        return number;
    } else {
        return -number;
    }
}

JsonNumber parser_json_number(std::tuple<JsonNumberIntegral, JsonNumberFraction, JsonNumberExponent>&& data) {
    auto integral = std::get<0>(data);
    auto fraction = std::get<1>(data);
    auto exponent = std::get<2>(data);

    return (JsonNumber(integral) +
        (JsonNumber(fraction) / std::pow(10, std::ceil(std::log10(JsonNumber(fraction))))))
        * std::pow(JsonNumber(10), JsonNumber(exponent));

    throw std::runtime_error("not implemented");
}

char parse_unicode_char(std::tuple<unit, char, char, char, char>&& value) {
    throw std::runtime_error("not implemented");
}

std::string parse_string_from_vector(std::vector<char>&& chars) {
    return std::string(chars.begin(), chars.end());
}

JsonValue parse_json_value_variant(std::variant<JsonString, JsonNumber, JsonObject, JsonArray, JsonBool, JsonNull>&& value) {
    auto out_value = JsonValue(); // JsonNull, JsonBool, JsonNumber, JsonString, JsonObject, JsonArray
    switch (value.index())
    {
    case 0:
        out_value.value.emplace<3>(std::get<0>(value));
        break;
    case 1:
        out_value.value.emplace<2>(std::get<1>(value));
        break;
    case 2:
        out_value.value.emplace<4>(std::get<2>(value));
        break;
    case 3:
        out_value.value.emplace<5>(std::get<3>(value));
        break;
    case 4:
        out_value.value.emplace<1>(std::get<4>(value));
        break;
    case 5:
        out_value.value.emplace<0>(std::get<5>(value));
        break;            
    default:
        throw std::runtime_error("unexpected variant state");
    }

    return out_value;
}

JsonArray parse_json_array(std::variant<std::vector<JsonValue>, unit>&& value) {
    if (value.index() == 0) {
        return JsonArray(std::move(std::get<0>(value)));
    } else {
        return JsonArray();
    }
}

std::shared_ptr<JsonValue> make_shared_json_value(JsonValue&& value) {
    return std::make_shared<JsonValue>(value);
}

JsonObject parse_json_object_fields(std::vector<JsonObjectField>&& value) {
    return JsonObject(std::move(value));
}

}