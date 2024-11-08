#pragma once

#include "base_definitions.hpp"
#include "utils.hpp"

namespace parser_combinators {

template<typename... Ress>
using ConcatTuple = std::tuple<Ress...>;

template<slice Slice, typename Err>
struct ConcatParserHelper;

template<typename Res, slice Slice, typename Err, typename Other>
struct ConcatParser { };

template<slice Slice, typename Err, typename... Ress, typename... Parsers>
    requires (parser<Parsers, Ress, Slice, Err> && ...)
struct ConcatParser<ConcatTuple<Ress...>, Slice, Err, Wrapper<Parsers...>> {
    ConcatTuple<ParserWrap<Ress, Parsers>...> parsers;

public:
    ConcatParser(Parsers... parsers)
        : parsers(std::make_tuple(ParserWrap<Ress, Parsers>(parsers)...)) { }

    ParserRes<ConcatTuple<Ress...>, Slice, Err> operator()(Slice slice) const {
        return ConcatParserHelper<Slice, Err>::apply(slice, parsers);
    }

private:
};

template<slice Slice, typename Err>
struct ConcatParserHelper {
    template<typename TRes, typename TParser>
        requires parser<TParser, TRes, Slice, Err>
    static inline ParserRes<ConcatTuple<TRes>, Slice, Err> apply(Slice slice, ConcatTuple<ParserWrap<TRes, TParser>> parser) {
        ParserRes<TRes, Slice, Err> res = utils::head(parser).p(slice);
        
        if (res) {
                return Ok( std::make_tuple(res->result), res->used, res->rest );
        } else {
            return Error(res.error());
        }
    }

    template<typename TRes, typename... TRess, typename TParser, typename... TParsers>
        requires parser<TParser, TRes, Slice, Err> && (parser<TParsers, TRess, Slice, Err> && ...)
    static inline ParserRes<ConcatTuple<TRes, TRess...>, Slice, Err> apply(Slice slice, ConcatTuple<ParserWrap<TRes, TParser>, ParserWrap<TRess, TParsers>...> parsers) {
        auto head = utils::head(parsers).p;
        auto tail = utils::tail(parsers);

        ParserRes<TRes, Slice, Err> res1 = head(slice);

        if (res1) {
            ParserRes<ConcatTuple<TRess...>, Slice, Err> ress = ConcatParserHelper::apply(res1->rest, tail);

            if (ress) {
                Slice used = Slice::join_adjacent(res1->used, ress->used).value();
                return Ok( std::tuple_cat<ConcatTuple<TRes>, ConcatTuple<TRess...>>(std::make_tuple(res1->result), std::move(ress->result)), used, ress->rest );
            } else {
                return Error(ress.error());
            }
        } else {
            return Error(res1.error());
        }
    }
};

// final builder
template<
    typename Res1, typename... Ress, slice Slice, typename Err,
    template <typename, typename, typename, typename> typename Parser1, typename Other1,
    template <typename, typename, typename, typename> typename... Parsers, typename... Others
>
    requires
        parser<Parser1<Res1, Slice, Err, Other1>, Res1, Slice, Err> &&
        (parser<Parsers<Ress, Slice, Err, Others>, Ress, Slice, Err> && ...)

ConcatParser<
    ConcatTuple<Res1, Ress...>,
    Slice, Err,
    Wrapper<
        Parser1<Res1, Slice, Err, Other1>,
        Parsers<Ress, Slice, Err, Others>...
    >
> concat(
    Parser1<Res1, Slice, Err, Other1> parser1,
    Parsers<Ress, Slice, Err, Others>... parsers
) {
    return 
        ConcatParser<
            ConcatTuple<Res1, Ress...>,
            Slice, Err,
            Wrapper<
                Parser1<Res1, Slice, Err, Other1>,
                Parsers<Ress, Slice, Err, Others>...
            >
        >(parser1, parsers...);
}

}