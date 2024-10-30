#pragma once

#include "base_definitions.hpp"

namespace parser_combinators {

template<typename Res, slice Slice, typename Err>
    Parser<Option<Res>, Slice, Err> optional(Parser<Res, Slice, Err> parser)
{
    return [parser](Slice slice) -> ParserRes<Option<Res>, Slice, Err> {
        ParserRes<Res, Slice, Err> result = parser(slice);

        if (result) {
            return Ok( Option<Res>(result->result), result->used, result->rest );
        } else {
            return Ok( Option<Res>(), Slice::def(), slice );
        }
    };
}

}