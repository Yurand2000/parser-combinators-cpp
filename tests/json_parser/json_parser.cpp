#include <cmath>

#include <functional>

#include "parser_combinator/parser.hpp"
#include "json.hpp"
#include "json_parser.hpp"

using namespace parser_combinators;
using namespace parser_combinators::string;

namespace json::parser {

// whitespace
static const auto json_whitespace =
    drop(list_zero(alt_same(parse_char(' '), parse_char('\n'), parse_char('\r'), parse_char('\t'))));

static const auto json_exponent = map(second(
    alt_same(parse_char('E'), parse_char('e')),
    concat(
        alt_same(
            value(parse_char('+'), true),
            value(parse_char('-'), false),
            atom<bool, string_slice, std::string>(true)
        ),
        list_one(parse_digit())
    )
), std::function(parse_json_number_exponent));

static const auto json_fraction = map(second(
    parse_char('.'),
    list_one(parse_digit())
), std::function(parse_json_number_fraction));

static const auto json_number_integral = map(concat(
    alt_same(
        value(parse_char('-'), false),
        atom<bool, string_slice, std::string>(true)
    ),
    alt_same(
        value(parse_char('0'), std::vector<char>({'0'})),
        map(
            concat(parse_digit_1_9(), list_zero(parse_digit())),
            std::function([](std::tuple<char, std::vector<char>>&& value) {
                auto first = std::get<0>(value);
                auto rest = std::get<1>(value);
                rest.insert(rest.begin(), first);
                return rest;
            })
        )
    )
), std::function(parse_json_number_integral));

static const auto json_number = map(concat(
    json_number_integral,
    optional_or_value(json_fraction, JsonNumberFraction(0)),
    optional_or_value(json_exponent, JsonNumberExponent(0))
), std::function(parser_json_number));

// string
static const auto json_char = alt_same(
    parse_not_char('"'),
    second(
        parse_char('\\'),
        alt_same(
            parse_char('"'),
            parse_char('\\'),
            parse_char('/'),
            value(parse_char('b'), '\b'),
            value(parse_char('f'), '\f'),
            value(parse_char('n'), '\n'),
            value(parse_char('r'), '\r'),
            value(parse_char('t'), '\t'),
            map(concat(
                drop(parse_char('u')),
                parse_xdigit(),
                parse_xdigit(),
                parse_xdigit(),
                parse_xdigit()
            ), std::function(parse_unicode_char)),
            fail<char, string_slice, std::string>(std::format("found invalid escape sequence"))
        )
    )
);

static const auto json_string = between(
    parse_char('"'),
    map(list_zero(json_char), std::function(parse_string_from_vector)),
    parse_char('"')
);

// other values
static const auto json_bool = alt_same(
    value(parse_string("true"), true),
    value(parse_string("false"), false)
);

static const auto json_null =
    value(parse_string("null"), JsonNull());

// values and objects
static ParserRes<JsonValue, string_slice, std::string> __json_value(string_slice slice);
static ParserRes<JsonArray, string_slice, std::string> __json_array(string_slice slice);
static ParserRes<JsonObject, string_slice, std::string> __json_object(string_slice slice);

static const auto json_value = make_parser(std::function(
    [](string_slice slice) -> ParserRes<JsonValue, string_slice, std::string> { return __json_value(slice); }));
static const auto json_array = make_parser(std::function(
    [](string_slice slice) -> ParserRes<JsonArray, string_slice, std::string> { return __json_array(slice); }));
static const auto json_object = make_parser(std::function(
    [](string_slice slice) -> ParserRes<JsonObject, string_slice, std::string> { return __json_object(slice); }));

static ParserRes<JsonValue, string_slice, std::string> __json_value(string_slice slice) {
    return between(
        json_whitespace,
        map(alt(
            json_string,
            json_number,
            json_object,
            json_array,
            json_bool,
            json_null
        ), std::function(parse_json_value_variant)),
        json_whitespace
    )(slice);
}

static ParserRes<JsonArray, string_slice, std::string> __json_array(string_slice slice) {
    return map(between(
        parse_char('['),
        alt(
            separated_list_one(json_value, parse_char(',')),
            json_whitespace
        ),
        parse_char(']')
    ), std::function(parse_json_array))(slice);
}

static ParserRes<JsonObject, string_slice, std::string> __json_object(string_slice slice) {
    auto object_item_parser = 
        concat(
            between(json_whitespace, json_string, json_whitespace),
            second(parse_char(':'), map(json_value, std::function(make_shared_json_value)))
        );

    return map(between(
        parse_char('{'),
        alt_same(
            separated_list_one(object_item_parser, parse_char(',')),
            value(json_whitespace, std::vector<JsonObjectField>())
        ),
        parse_char('}')
    ), std::function(parse_json_object_fields))(slice);
}

ParserRes<JsonValue, string_slice, std::string> parse(std::string&& json) {
    auto slice = string_slice(std::move(json));
    return json_value(slice);
}

}