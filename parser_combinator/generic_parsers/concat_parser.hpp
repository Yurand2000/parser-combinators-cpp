#pragma once

#include "base_definitions.hpp"

namespace parser_combinators {

// base case
template<slice Slice, typename Err, typename Res>
    Parser<std::tuple<Res>, Slice, Err> _concat(Parser<Res, Slice, Err> parser)
{
    return [parser](Slice slice) -> ParserRes<std::tuple<Res>, Slice, Err> {
        return
            parser(slice)
            .transform([](Ok<Res, Slice> res) -> Ok<std::tuple<Res>, Slice> {
                return Ok( std::make_tuple(res.result), res.used, res.rest );
            });
    };
}

// recursive case
template<slice Slice, typename Err, typename Res1, typename... Res>
    Parser<std::tuple<Res1, Res...>, Slice, Err> _concat(Parser<Res1, Slice, Err> parser1, Parser<Res, Slice, Err>... parsers)
{
    auto tuple_parsers = _concat(parsers...);
    return [parser1, tuple_parsers](Slice slice) -> ParserRes<std::tuple<Res1, Res...>, Slice, Err> {
        return
            parser1(slice)
            .and_then([tuple_parsers](Ok<Res1, Slice> res1) -> ParserRes<std::tuple<Res1, Res...>, Slice, Err> {
                ParserRes<std::tuple<Res...>, Slice, Err> ress = tuple_parsers(res1.rest);
                if (ress) {
                    Slice used = Slice::join_adjacent(res1.used, ress->used).value();
                    return Ok( std::tuple_cat<std::tuple<Res1>, std::tuple<Res...>>(std::tuple(res1.result), std::move(ress->result)), used, ress->rest );
                } else {
                    return Error(ress.error());
                }
            });
    };
}

// final builder
template<slice Slice, typename Err, typename Res1, typename... Res>
    Parser<std::tuple<Res1, Res...>, Slice, Err> concat(Parser<Res1, Slice, Err> parser1, Parser<Res, Slice, Err>... parsers)
{
    return _concat(parser1, parsers...);
}

}