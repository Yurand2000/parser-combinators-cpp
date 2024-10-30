#pragma once

#include "base_definitions.hpp"
#include "concat_parser.hpp"
#include "kleene_parsers.hpp"

namespace parser_combinators {

template<slice Slice, typename Err, typename Res, typename Sep>
    Parser<std::vector<Res>, Slice, Err> separated_list_one(Parser<Res, Slice, Err> parser, Parser<Sep, Slice, Err> separator)
{
    auto drop_separator = drop(separator);
    
    auto separated_parser = map(concat(parser, drop_separator),
        [](std::tuple<Res, unit> tuple) -> Res {
            return std::tuple<Res, unit>::template get<0>(tuple);
        });

    auto list_parser = concat(parser, kleene_star(separated_parser));

    return map(list_parser, [](std::tuple<Res, std::vector<Res>> results) -> std::vector<Res> {
        auto first = std::tuple<Res, std::vector<Res>>::template get<0>(results);
        auto rest = std::tuple<Res, std::vector<Res>>::template get<1>(results);

        rest.insert(0, first);
        return rest;
    });
}

template<slice Slice, typename Err, typename Res, typename Sep>
    Parser<std::vector<Res>, Slice, Err> separated_list_zero(Parser<Res, Slice, Err> parser, Parser<Sep, Slice, Err> separator)
{
    auto list_parser = optional(separated_list_one(parser, separator));

    return map(list_parser, [](Option<std::vector<Res>> results) -> std::vector<Res> {
        if (results) {
            return results.value();
        } else {
            return std::vector<Res>();
        }
    });
}

}