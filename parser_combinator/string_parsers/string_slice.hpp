#pragma once
#include <string>
#include <memory>
#include <tuple>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <format>

#include "../generic_parsers/mod.hpp"

namespace parser_combinators::string {

struct string_slice {
public:
    std::shared_ptr<std::string> string;
    std::string::const_iterator begin;
    std::string::const_iterator end;

public:
    string_slice(std::string&& str);
    std::pair<string_slice, string_slice> split_at(std::string::const_iterator iter) const;
    static Option<string_slice> join_adjacent(string_slice left, string_slice right);
    static string_slice def();
};

}

template<>
struct std::formatter<parser_combinators::string::string_slice, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();

        if (*it != '}')
            throw std::format_error("Invalid format args for string_slice.");

        return it;
    }

    template<class FmtContext>
    FmtContext::iterator format(parser_combinators::string::string_slice s, FmtContext& ctx) const
    {
        std::ostringstream out;

        for (auto it = s.begin; it != s.end; it++) {
            out << *it;
        }

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};
