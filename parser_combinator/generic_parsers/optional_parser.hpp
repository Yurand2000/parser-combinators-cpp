#pragma once

#include "base_definitions.hpp"
#include "utils.hpp"

namespace parser_combinators {

template<typename Res, slice Slice, typename Err, typename Other>
struct OptionalParser { };

template<typename Res, slice Slice, typename Err, typename Parser>
    requires parser<Parser, Res, Slice, Err>
struct OptionalParser<Option<Res>, Slice, Err, Wrapper<Parser>>
{
    Parser parser;

    OptionalParser(Parser parser) : parser(parser) { }

    ParserRes<Option<Res>, Slice, Err> operator()(Slice slice) const {
        auto result = parser(slice);

        if (result) {
            return Ok( Option<Res>(result->result), result->used, result->rest );
        } else {
            return Ok( Option<Res>(), Slice::def(), slice );
        }
    }
};

template<typename Res, slice Slice, typename Err, template <typename, typename, typename, typename> typename Parser, typename Other>
    OptionalParser<Option<Res>, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>>> optional(Parser<Res, Slice, Err, Other> parser)
{
    return OptionalParser<Option<Res>, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>>>(parser);
}

template<typename Res, slice Slice, typename Err, typename Other>
struct OptionalOrValueParser { };

template<typename Res, slice Slice, typename Err, typename Parser>
    requires parser<Parser, Res, Slice, Err>
struct OptionalOrValueParser<Res, Slice, Err, Wrapper<Parser>>
{
    Parser parser;
    Res value;

    OptionalOrValueParser(Parser parser, Res value) : parser(parser), value(value) { }

    ParserRes<Res, Slice, Err> operator()(Slice slice) const {
        auto result = parser(slice);

        if (result) {
            return Ok( result->result, result->used, result->rest );
        } else {
            return Ok( value, Slice::def(), slice );
        }
    }
};

template<typename Res, slice Slice, typename Err, template <typename, typename, typename, typename> typename Parser, typename Other>
    OptionalOrValueParser<Res, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>>> optional_or_value(Parser<Res, Slice, Err, Other> parser, Res value)
{
    return OptionalOrValueParser<Res, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>>>(parser, value);
}

}