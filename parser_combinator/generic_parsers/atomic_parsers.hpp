#pragma once

#include "base_definitions.hpp"
#include "utils.hpp"

namespace parser_combinators {

template<typename Res, slice Slice, typename Err, typename Other>
struct AtomParser
{
    Res v;

public:
    AtomParser(Res v) : v(v) {}

    ParserRes<Res, Slice, Err> operator()(Slice slice) const {
        return Ok(v, Slice::def(), slice);
    }
};

template<typename Res, slice Slice, typename Err>
AtomParser<Res, Slice, Err, Wrapper<>> atom(Res v) {
    return AtomParser<Res, Slice, Err, Wrapper<>>(v);
}

template<typename Res, slice Slice, typename Err, typename Other>
struct FailParser
{
    Err error;

public:
    FailParser(Err error) : error(error) {}

    ParserRes<Res, Slice, Err> operator()(Slice) const {
        return Error(error);
    }
};

template<typename Res, slice Slice, typename Err>
FailParser<Res, Slice, Err, Wrapper<>> fail(Err error) {
    return FailParser<Res, Slice, Err, Wrapper<>>(error);
}

}