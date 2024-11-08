#pragma once

#include "base_definitions.hpp"
#include "optional_parser.hpp"
#include "utils.hpp"

namespace parser_combinators {

template <typename T>
using KleeneStarList = std::vector<T>;

template<typename Res, slice Slice, typename Err, typename Other>
struct KleeneStarParser { };

template<typename Res, slice Slice, typename Err, typename Parser>
    requires parser<Parser, Res, Slice, Err>
struct KleeneStarParser<KleeneStarList<Res>, Slice, Err, Wrapper<Parser>>
{
    OptionalParser<Option<Res>, Slice, Err, Wrapper<Parser>> opt_parser;

public:
    KleeneStarParser(Parser parser) : opt_parser(optional(parser)) { }

    ParserRes<KleeneStarList<Res>, Slice, Err> operator()(Slice slice) const {
        auto out_vector = KleeneStarList<Res>();
        Slice used = Slice::def();
        Slice rest = slice;
        bool exit = false;

        do {
            auto result = opt_parser(rest);
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
    }
};

template<typename Res, slice Slice, typename Err, template <typename, typename, typename, typename > typename Parser, typename Other>
    KleeneStarParser<KleeneStarList<Res>, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>>> kleene_star(Parser<Res, Slice, Err, Other> parser)
{
    return KleeneStarParser<KleeneStarList<Res>, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>>>(parser);
}

template <typename... Args>
auto list_zero(Args&&... args) -> decltype(kleene_star(std::forward<Args>(args)...)) {
  return kleene_star(std::forward<Args>(args)...);
}

template<typename Res, slice Slice, typename Err, typename Other>
struct KleeneOneParser { };

template<typename Res, slice Slice, typename Err, typename Parser>
    requires parser<Parser, Res, Slice, Err>
struct KleeneOneParser<KleeneStarList<Res>, Slice, Err, Wrapper<Parser>>
{
    Parser parser;
    KleeneStarParser<KleeneStarList<Res>, Slice, Err, Wrapper<Parser>> star_parser;

public:
    KleeneOneParser(Parser parser) : parser(parser), star_parser(kleene_star(parser)) { }

    ParserRes<KleeneStarList<Res>, Slice, Err> operator()(Slice slice) const {
        auto result = parser(slice);

        if (result) {
            auto star_result = star_parser(result->rest);
            star_result->result.insert(star_result->result.begin(), result->result);
            star_result->used = Slice::join_adjacent(result->used, star_result->used).value();
            return star_result;
        } else {
            return Error(result.error());
        }
    }
};

template<typename Res, slice Slice, typename Err, template <typename, typename, typename, typename > typename Parser, typename Other>
    KleeneOneParser<KleeneStarList<Res>, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>>> kleene_one(Parser<Res, Slice, Err, Other> parser)
{
    return KleeneOneParser<KleeneStarList<Res>, Slice, Err, Wrapper<Parser<Res, Slice, Err, Other>>>(parser);
}

template <typename... Args>
auto list_one(Args&&... args) -> decltype(kleene_one(std::forward<Args>(args)...)) {
  return kleene_one(std::forward<Args>(args)...);
}

}