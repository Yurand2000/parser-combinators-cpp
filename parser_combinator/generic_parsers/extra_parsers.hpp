#pragma once

#include <iostream>
#include <functional>
#include <string>

#include "base_definitions.hpp"
#include "atomic_parsers.hpp"
#include "concat_parser.hpp"
#include "utils.hpp"

namespace parser_combinators {

template <
    slice Slice, typename Err,
    typename Res1, typename Res2, typename Res3,
    typename Other1, typename Other2, typename Other3,
    template <typename, typename, typename, typename> typename Parser1,
    template <typename, typename, typename, typename> typename Parser2,
    template <typename, typename, typename, typename> typename Parser3
>
    requires
        parser<Parser1<Res1, Slice, Err, Other1>, Res1, Slice, Err> &&
        parser<Parser2<Res2, Slice, Err, Other2>, Res2, Slice, Err> &&
        parser<Parser3<Res3, Slice, Err, Other3>, Res3, Slice, Err>
auto between(Parser1<Res1, Slice, Err, Other1> parser1, Parser2<Res2, Slice, Err, Other2> parser2, Parser3<Res3, Slice, Err, Other3> parser3)
{
    return map(concat(
        drop(parser1),
        parser2,
        drop(parser3)
    ), std::function([](ConcatTuple<unit, Res2, unit>&& value) -> Res2 {
        return std::get<1>(value);
    }));
}

template <
    slice Slice, typename Err,
    typename Res1, typename Res2,
    typename Other1, typename Other2,
    template <typename, typename, typename, typename> typename Parser1,
    template <typename, typename, typename, typename> typename Parser2
>
    requires
        parser<Parser1<Res1, Slice, Err, Other1>, Res1, Slice, Err> &&
        parser<Parser2<Res2, Slice, Err, Other2>, Res2, Slice, Err>
auto first(Parser1<Res1, Slice, Err, Other1> parser1, Parser2<Res2, Slice, Err, Other2> parser2)
{
    return map(concat(
        parser1,
        drop(parser2)
    ), std::function([](ConcatTuple<Res1, unit>&& value) -> Res1 {
        return std::get<0>(value);
    }));
}

template <
    slice Slice, typename Err,
    typename Res1, typename Res2,
    typename Other1, typename Other2,
    template <typename, typename, typename, typename> typename Parser1,
    template <typename, typename, typename, typename> typename Parser2
>
    requires
        parser<Parser1<Res1, Slice, Err, Other1>, Res1, Slice, Err> &&
        parser<Parser2<Res2, Slice, Err, Other2>, Res2, Slice, Err>
auto second(Parser1<Res1, Slice, Err, Other1> parser1, Parser2<Res2, Slice, Err, Other2> parser2)
{
    return map(concat(
        drop(parser1),
        parser2
    ), std::function([](ConcatTuple<unit, Res2>&& value) -> Res2 {
        return std::get<1>(value);
    }));
}

template <
    slice Slice, typename Err,
    typename Res1, typename Other1,
    template <typename, typename, typename, typename> typename Parser1
>
    requires parser<Parser1<Res1, Slice, Err, Other1>, Res1, Slice, Err>
auto debug(Parser1<Res1, Slice, Err, Other1> parser, std::string message)
{
    return map(parser,
        std::function([message](Res1&& value) -> Res1 {
            std::cout << message << std::endl;
            return value;
        })
    );
}

template <typename Res, slice Slice, typename Err, typename Other>
struct Parser { };

template <typename Res, slice Slice, typename Err, typename Fn>
    requires function<Fn, ParserRes<Res, Slice, Err>, Slice>
struct Parser<Res, Slice, Err, Wrapper<Fn>>
{
    Fn parser;

public:
    Parser(Fn parser) : parser(parser) { }

    ParserRes<Res, Slice, Err> operator()(Slice slice) const {
        return parser(slice);
    }
};

template <typename Res, slice Slice, typename Err>
Parser<Res, Slice, Err, Wrapper<std::function<ParserRes<Res, Slice, Err>(Slice)>>> make_parser(std::function<ParserRes<Res, Slice, Err>(Slice)> function) {
    return Parser<Res, Slice, Err, Wrapper<std::function<ParserRes<Res, Slice, Err>(Slice)>>>(function);
}

}