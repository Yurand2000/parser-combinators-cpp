#pragma once

#include <iostream>
#include <string>

#include "base_definitions.hpp"
#include "atomic_parsers.hpp"
#include "concat_parser.hpp"

namespace parser_combinators {

template <typename Res1, typename Res2, typename Res3, slice Slice, typename Err>
    Parser<Res2, Slice, Err> between(Parser<Res1, Slice, Err> parser1, Parser<Res2, Slice, Err> parser2, Parser<Res3, Slice, Err> parser3)
{
    return map(concat(
        parser1,
        parser2,
        parser3
    ), std::function([](std::tuple<Res1, Res2, Res3>&& value) -> Res2 {
        return std::get<1>(value);
    }));
}

template <typename Res1, typename Res2, slice Slice, typename Err>
    Parser<Res1, Slice, Err> first(Parser<Res1, Slice, Err> parser1, Parser<Res2, Slice, Err> parser2)
{
    return map(concat(
        parser1,
        parser2
    ), std::function([](std::tuple<Res1, Res2>&& value) -> Res1 {
        return std::get<0>(value);
    }));
}

template <typename Res1, typename Res2, slice Slice, typename Err>
    Parser<Res2, Slice, Err> second(Parser<Res1, Slice, Err> parser1, Parser<Res2, Slice, Err> parser2)
{
    return map(concat(
        parser1,
        parser2
    ), std::function([](std::tuple<Res1, Res2>&& value) -> Res2 {
        return std::get<1>(value);
    }));
}

template <typename Res, slice Slice, typename Err>
    Parser<Res, Slice, Err> debug(Parser<Res, Slice, Err> parser, std::string message)
{
    return map(parser,
        std::function([message](Res&& value) -> Res {
            std::cout << message << std::endl;
            return value;
        })
    );
}

}