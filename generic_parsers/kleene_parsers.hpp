#pragma once

#include "base_definitions.hpp"
#include "optional_parser.hpp"

namespace parser_combinators {

template<typename Res, slice Slice, typename Err>
    Parser<std::vector<Res>, Slice, Err> kleene_star(Parser<Res, Slice, Err> parser)
{
    Parser<Option<Res>, Slice, Err> opt_parser = optional(parser);

    return [opt_parser, parser](Slice slice) -> ParserRes<std::vector<Res>, Slice, Err> {
        auto out_vector = std::vector<Res>();
        Slice used = Slice::def();
        Slice rest = slice;
        bool exit = false;

        do {
            ParserRes<Option<Res>, Slice, Err> result = opt_parser(rest);
            if (result && result->result) {                
                out_vector.push_back(result->result.value());
                used = Slice::join_adjacent(used, result->used).value();
                rest = result->rest;
            } else if (!result) {
                return Error(result.error());
            } else {
                exit = true;
            }
        } while (!exit);

        return Ok( out_vector, used, rest );
    };
}

template <typename... Args>
auto list_zero(Args&&... args) -> decltype(f(std::forward<Args>(args)...)) {
  return kleene_star(std::forward<Args>(args)...);
}

template<typename Res, slice Slice, typename Err>
    Parser<std::vector<Res>, Slice, Err> kleene_one(Parser<Res, Slice, Err> parser)
{
    Parser<std::vector<Res>, Slice, Err> star_parser = kleene_star(parser);

    return [star_parser, parser](Slice slice) -> ParserRes<std::vector<Res>, Slice, Err> {
        ParserRes<Res, Slice, Err> result = parser(slice);

        if (result) {
            ParserRes<std::vector<Res>, Slice, Err> star_result = star_parser(result->rest);
            star_result->result.insert(star_result->result.begin(), result->result);
            star_result->used = Slice::join_adjacent(result->used, star_result->used).value();
            return star_result;
        } else {
            return Error(result.error());
        }
    };
}

template <typename... Args>
auto list_one(Args&&... args) -> decltype(f(std::forward<Args>(args)...)) {
  return kleene_one(std::forward<Args>(args)...);
}

}