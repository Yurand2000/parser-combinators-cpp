#pragma once

#include "base_definitions.hpp"
#include "concat_parser.hpp"
#include "kleene_parsers.hpp"
#include "extra_parsers.hpp"
#include "utils.hpp"

namespace parser_combinators {

template<
    typename Res, typename Sep, slice Slice, typename Err,
    typename Other, typename OtherSep,
    template <typename, typename, typename, typename> typename Parser,
    template <typename, typename, typename, typename> typename ParserSep
>
    requires 
        parser<Parser<Res, Slice, Err, Other>, Res, Slice, Err> &&
        parser<ParserSep<Sep, Slice, Err, OtherSep>, Sep, Slice, Err>
auto separated_list_one(Parser<Res, Slice, Err, Other> parser, ParserSep<Sep, Slice, Err, OtherSep> separator)
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

template<
    typename Res, typename Sep, slice Slice, typename Err,
    typename Other, typename OtherSep,
    template <typename, typename, typename, typename> typename Parser,
    template <typename, typename, typename, typename> typename ParserSep
>
    requires 
        parser<Parser<Res, Slice, Err, Other>, Res, Slice, Err> &&
        parser<ParserSep<Sep, Slice, Err, OtherSep>, Sep, Slice, Err>
auto separated_list_zero(Parser<Res, Slice, Err, Other> parser, ParserSep<Sep, Slice, Err, OtherSep> separator)
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