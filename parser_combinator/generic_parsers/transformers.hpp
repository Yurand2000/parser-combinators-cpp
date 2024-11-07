#pragma once

#include <concepts>

#include "base_definitions.hpp"

namespace parser_combinators {

template<typename Res, slice Slice, typename Err>
    Parser<unit, Slice, Err> drop(Parser<Res, Slice, Err> parser)
{
    return [parser](Slice slice) -> ParserRes<unit, Slice, Err> {
        return parser(slice)
            .transform([](Ok<Res, Slice> result) -> Ok<unit, Slice> {
                return Ok(unit(), result.used, result.rest);
            });
    };
}

template<typename Res1, typename Res2, slice Slice, typename Err>
    Parser<Res2, Slice, Err> value(Parser<Res1, Slice, Err> parser, Res2 value)
{
    return [parser, value](Slice slice) -> ParserRes<Res2, Slice, Err> {
        return parser(slice)
            .transform([value](Ok<Res1, Slice> result) -> Ok<Res2, Slice> {
                return Ok(value, result.used, result.rest);
            });
    };
}


template<typename Res1, typename Res2, slice Slice, typename Err>
    Parser<Res2, Slice, Err> map(Parser<Res1, Slice, Err> parser, std::function<Res2(Res1&&)> mapping_fn)
{
    return [parser, mapping_fn](Slice slice) -> ParserRes<Res2, Slice, Err> {
        return parser(slice)
            .transform([mapping_fn](Ok<Res1, Slice> result) -> Ok<Res2, Slice> {
                return Ok(mapping_fn(std::move(result.result)), result.used, result.rest);
            });
    };
}

}