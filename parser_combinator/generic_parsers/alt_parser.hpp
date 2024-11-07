#pragma once
#include <variant>

#include "base_definitions.hpp"
#include "optional_parser.hpp"

namespace parser_combinators {

// support class
template <typename... LRes> struct alt_support {};
template <slice Slice, typename Err, typename...> struct alt_builder {};

// base case
template<slice Slice, typename Err, typename... LRes, typename Res>
struct alt_builder<Slice, Err, alt_support<LRes...>, Res> {
    Parser<std::variant<LRes..., Res>, Slice, Err> operator()(Parser<Res, Slice, Err> parser)
    {
        return [parser](Slice slice) -> ParserRes<std::variant<LRes..., Res>, Slice, Err> {
            return parser(slice)
                .transform([](Ok<Res, Slice> res) {
                    return Ok(std::variant<LRes..., Res>(std::in_place_index<sizeof...(LRes)>, res.result), res.used, res.rest);
                });
        };
    }
};

// recursive case
template<slice Slice, typename Err, typename Res, typename... RRes, typename... LRes>
struct alt_builder<Slice, Err, alt_support<LRes...>, Res, RRes...> {
    Parser<std::variant<LRes..., Res, RRes...>, Slice, Err> operator()(Parser<Res, Slice, Err> parser, Parser<RRes, Slice, Err>... parsers)
    {
        auto rparsers = alt_builder<Slice, Err, alt_support<LRes..., Res>, RRes...>()(parsers...);
        auto opt_parser = optional(parser);

        return [rparsers, opt_parser](Slice slice) -> ParserRes<std::variant<LRes..., Res, RRes...>, Slice, Err> {
            auto res = opt_parser(slice);
            if (!res) {
                return Error(res.error());
            }

            if (res->result) {
                return Ok(std::variant<LRes..., Res, RRes...>(std::in_place_index<sizeof...(LRes)>, res->result.value()), res->used, res->rest);
            } else {
                return rparsers(slice);
            }
        };
    }
};

// final builder
template<slice Slice, typename Err, typename... Res>
    Parser<std::variant<Res...>, Slice, Err> alt(Parser<Res, Slice, Err>... parsers)
{
    return alt_builder<Slice, Err, alt_support<>, Res...>()(parsers...);
}

// base case
template<slice Slice, typename Err, typename Res>
Parser<Res, Slice, Err> _alt_same(Parser<Res, Slice, Err> parser)
{
    return parser;
}

// recursive case
template<slice Slice, typename Err, typename Res, typename... Parsers>
Parser<Res, Slice, Err> _alt_same(Parser<Res, Slice, Err> parser, Parsers... parsers)
{
    auto alt_parsers = _alt_same(parsers...);

    return [parser, alt_parsers](Slice slice) -> ParserRes<Res, Slice, Err> {
        auto res = parser(slice);

        if (res) {
            return res;
        } else {
            return alt_parsers(slice);
        }
    };
}

// final builder
template<slice Slice, typename Err, typename Res, typename... Parsers>
    Parser<Res, Slice, Err> alt_same(Parser<Res, Slice, Err> parser1, Parser<Res, Slice, Err> parser2, Parsers... parsers)
{
    return _alt_same(parser1, parser2, parsers...);
}

}