#pragma once

#include "base_definitions.hpp"
#include "concat_parser.hpp"
#include "kleene_parsers.hpp"
#include "extra_parsers.hpp"

namespace parser_combinators {

template<slice Slice, typename Err, typename Res, typename Sep>
    Parser<std::vector<Res>, Slice, Err> separated_list_one(Parser<Res, Slice, Err> parser, Parser<Sep, Slice, Err> separator)
{
    auto separated_parser = second(drop(separator), parser);
    auto list_parser = concat(parser, kleene_star(separated_parser));

    return map(list_parser, 
        std::function([](std::tuple<Res, std::vector<Res>>&& results) -> std::vector<Res> {
        auto first = std::get<0>(results);
        auto rest = std::get<1>(results);

        rest.insert(rest.begin(), first);
        return rest;
    }));
}

template<slice Slice, typename Err, typename Res, typename Sep>
    Parser<std::vector<Res>, Slice, Err> separated_list_zero(Parser<Res, Slice, Err> parser, Parser<Sep, Slice, Err> separator)
{
    auto list_parser = optional(separated_list_one(parser, separator));

    return map(list_parser, [](Option<std::vector<Res>>&& results) -> std::vector<Res> {
        if (results) {
            return results.value();
        } else {
            return std::vector<Res>();
        }
    });
}

}