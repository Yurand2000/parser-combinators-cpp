#pragma once

#include "base_definitions.hpp"

namespace parser_combinators {

template<typename Res, slice Slice, typename Err>
    Parser<Res, Slice, Err> atom(Res v)
{
    return [v](Slice slice) -> ParserRes<Res, Slice, Err> {
        return Ok(v, Slice::def(), slice);
    };
}

template<typename Res, slice Slice, typename Err>
    Parser<Res, Slice, Err> fail(Err error)
{
    return [error](Slice slice) -> ParserRes<Res, Slice, Err> {
        return Error(error);
    };
}

}