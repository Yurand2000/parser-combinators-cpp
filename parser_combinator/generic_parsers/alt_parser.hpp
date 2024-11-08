#pragma once
#include <variant>
#include <tuple>

#include "base_definitions.hpp"
#include "optional_parser.hpp"
#include "utils.hpp"

namespace parser_combinators {

// *****************
// alt parser
template <typename Res, slice Slice, typename Err, typename Other>
struct AltParser { };

template <slice Slice, typename Err>
struct AltParserHelper;

template <typename... Ress, slice Slice, typename Err, typename... Parsers>
    requires (parser<Parsers, Ress, Slice, Err> && ...)
struct AltParser<std::variant<Ress...>, Slice, Err, Wrapper<Parsers...>>
{
    std::tuple<ParserWrap<Ress, Parsers>...> parsers;

public:
    AltParser(Parsers... parsers)
        : parsers(std::make_tuple(ParserWrap<Ress, Parsers>(parsers)...)) { }

    ParserRes<std::variant<Ress...>, Slice, Err> operator()(Slice slice) const {
        return AltParserHelper<Slice, Err>::apply(slice, parsers, Wrapper<>());
    }
};

template <slice Slice, typename Err>
struct AltParserHelper {
    template<typename... LRess, typename Res, typename Parser>
        requires parser<Parser, Res, Slice, Err>
    static inline ParserRes<std::variant<LRess..., Res>, Slice, Err> apply(Slice slice, std::tuple<ParserWrap<Res, Parser>> parser, Wrapper<LRess...> _w) {
        ParserRes<Res, Slice, Err> res = utils::head(parser).p(slice);

        if (res) {
            return Ok(std::variant<LRess..., Res>(std::in_place_index<sizeof...(LRess)>, res->result), res->used, res->rest);
        } else {
            return Error(res.error());
        }
    }

    template<typename... LRess, typename Res, typename... RRess, typename Parser, typename... RParsers>
        requires parser<Parser, Res, Slice, Err> && (parser<RParsers, RRess, Slice, Err> && ...)
    static inline ParserRes<std::variant<LRess..., Res, RRess...>, Slice, Err> apply(Slice slice, std::tuple<ParserWrap<Res, Parser>, ParserWrap<RRess, RParsers>...> parsers, Wrapper<LRess...> _w) {
        auto head = optional(utils::head(parsers).p);
        auto tail = utils::tail(parsers);

        ParserRes<Option<Res>, Slice, Err> res = head(slice);
        if (!res) {
            return Error(res.error());
        }

        if (res->result) {
            return Ok(std::variant<LRess..., Res, RRess...>(std::in_place_index<sizeof...(LRess)>, res->result.value()), res->used, res->rest);
        } else {
            return AltParserHelper::apply(slice, tail, Wrapper<LRess..., Res>());
        }
    }
};

// final builder
template<
    typename... Ress, slice Slice, typename Err, typename... Others,
    template <typename, typename, typename, typename> typename... Parsers
>
    requires (parser<Parsers<Ress, Slice, Err, Others>, Ress, Slice, Err> && ...)
AltParser<std::variant<Ress...>, Slice, Err, Wrapper<Parsers<Ress, Slice, Err, Others>...>> alt(Parsers<Ress, Slice, Err, Others>... parsers)
{
    return AltParser<std::variant<Ress...>, Slice, Err, Wrapper<Parsers<Ress, Slice, Err, Others>...>>(parsers...);
}

// *****************
// alt same parser
template <typename Res, slice Slice, typename Err, typename Other>
struct AltSameParser { };

template <typename Res, slice Slice, typename Err>
struct AltSameParserHelper;

template <typename Res, slice Slice, typename Err, typename... Parsers>
    requires (parser<Parsers, Res, Slice, Err> && ...)
struct AltSameParser<Res, Slice, Err, Wrapper<Parsers...>>
{
    std::tuple<Parsers...> parsers;

public:
    AltSameParser(Parsers... parsers)
        : parsers(std::make_tuple(std::move(parsers)...)) { }

    ParserRes<Res, Slice, Err> operator()(Slice slice) const {
        return AltSameParserHelper<Res, Slice, Err>::apply(slice, parsers);
    }
};

template <typename Res, slice Slice, typename Err>
struct AltSameParserHelper {
    template<typename TParser>
        requires parser<TParser, Res, Slice, Err>
    static inline ParserRes<Res, Slice, Err> apply(Slice slice, std::tuple<TParser> parser) {
        return utils::head(parser)(slice);
    }

    template<typename TParser, typename... TParsers>
        requires parser<TParser, Res, Slice, Err> && (parser<TParsers, Res, Slice, Err> && ...)
    static inline ParserRes<Res, Slice, Err> apply(Slice slice, std::tuple<TParser, TParsers...> parsers) {
        auto result = utils::head(parsers)(slice);
        if (result) {
            return result;
        } else {
            return AltSameParserHelper::apply(slice, utils::tail(parsers));
        }
    }
};

// builder
template<
    typename Res, slice Slice, typename Err,
    template <typename, typename, typename, typename> typename Parser1, typename Other1,
    template <typename, typename, typename, typename> typename Parser2, typename Other2,
    template <typename, typename, typename, typename> typename... Parsers, typename... Others
>
    requires
        parser<Parser1<Res, Slice, Err, Other1>, Res, Slice, Err> &&
        parser<Parser2<Res, Slice, Err, Other2>, Res, Slice, Err> &&
        (parser<Parsers<Res, Slice, Err, Others>, Res, Slice, Err> && ...)

AltSameParser<Res, Slice, Err, Wrapper<
    Parser1<Res, Slice, Err, Other1>,
    Parser2<Res, Slice, Err, Other2>,
    Parsers<Res, Slice, Err, Others>...
>> alt_same(Parser1<Res, Slice, Err, Other1> parser1, Parser2<Res, Slice, Err, Other2> parser2, Parsers<Res, Slice, Err, Others>... parsers)
{
    return 
        AltSameParser<Res, Slice, Err, Wrapper<
            Parser1<Res, Slice, Err, Other1>,
            Parser2<Res, Slice, Err, Other2>,
            Parsers<Res, Slice, Err, Others>...
        >>(parser1, parser2, parsers...);
}

}