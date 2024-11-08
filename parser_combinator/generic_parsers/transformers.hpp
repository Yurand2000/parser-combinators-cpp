#pragma once

#include <concepts>

#include "base_definitions.hpp"
#include "utils.hpp"

namespace parser_combinators {

// drop parser
template<typename Res, slice Slice, typename Err, typename Other>
struct DropParser {};

template<typename Res, slice Slice, typename Err, typename Parser>
    requires parser<Parser, Res, Slice, Err>
struct DropParser<unit, Slice, Err, Wrapper<Parser, Res>>
{
    Parser parser;

public:
    DropParser(Parser parser) : parser(parser) {}

    ParserRes<unit, Slice, Err> operator()(Slice slice) const {
        ParserRes<Res, Slice, Err> res = parser(slice);

        if (res) {
            return Ok(unit(), res->used, res->rest);
        } else {
            return Error(res.error());
        }
    }
};

template<typename Res, slice Slice, typename Err, template <typename, typename, typename, typename> typename Parser, typename Other>
    requires parser<Parser<Res, Slice, Err, Other>, Res, Slice, Err>
DropParser<unit, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>, Res>> drop(Parser<Res, Slice, Err, Other> parser)
{
    return DropParser<unit, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>, Res>>(parser);
}

// value parser
template<typename Res, slice Slice, typename Err, typename Other>
struct ValueParser { };

template< typename Res, typename Res2, slice Slice, typename Err, typename Parser>
    requires parser<Parser, Res, Slice, Err>
struct ValueParser<Res2, Slice, Err, Wrapper<Parser, Res>>
{
    Parser parser;
    Res2 value;

public:
    ValueParser(Parser parser, Res2 value) : parser(parser), value(value) {}

    ParserRes<Res2, Slice, Err> operator()(Slice slice) const {
        ParserRes<Res, Slice, Err> res = parser(slice);

        if (res) {
            return Ok(value, res->used, res->rest);
        } else {
            return Error(res.error());
        }
    }
};

template<typename Res1, typename Res2, slice Slice, typename Err, template <typename, typename, typename, typename> typename Parser, typename Other>
    requires parser<Parser<Res1, Slice, Err, Other>, Res1, Slice, Err>
ValueParser<Res2, Slice, Err, Wrapper<Parser<Res1, Slice, Err, Other>, Res1>> value(Parser<Res1, Slice, Err, Other> parser, Res2 value)
{
    return ValueParser<Res2, Slice, Err, Wrapper<Parser<Res1, Slice, Err, Other>, Res1>>(parser, value);
}

// map parser
template<typename Res, slice Slice, typename Err, typename Other>
struct MapParser { };

template<typename Res, typename Res2, slice Slice, typename Err, typename Parser, typename Fn>
    requires parser<Parser, Res, Slice, Err> && function<Fn, Res2, Res&&>
struct MapParser<Res2, Slice, Err, Wrapper<Parser, Fn, Res>>
{
    Parser parser;
    Fn mapping_fn;

public:
    MapParser(Parser parser, Fn mapping_fn) : parser(parser), mapping_fn(mapping_fn) {}

    ParserRes<Res2, Slice, Err> operator()(Slice slice) const {
        ParserRes<Res, Slice, Err> res = parser(slice);

        if (res) {
            return Ok(mapping_fn(std::move(res->result)), res->used, res->rest);
        } else {
            return Error(res.error());
        }
    }
};

template<typename Res, typename Res2, slice Slice, typename Err, template <typename, typename, typename, typename> typename Parser, typename Other>
    requires parser<Parser<Res, Slice, Err, Other>, Res, Slice, Err>
    MapParser<Res2, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>, std::function<Res2(Res&&)>, Res>> map(Parser<Res, Slice, Err, Other> parser, std::function<Res2(Res&&)> mapping_fn)
{
    return MapParser<Res2, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>, std::function<Res2(Res&&)>, Res>>(parser, mapping_fn);
}

}