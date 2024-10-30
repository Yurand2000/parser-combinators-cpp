#pragma once
#include <string>
#include <memory>
#include <tuple>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <format>

namespace parser_combinators::string {

struct string_slice {
public:
    std::shared_ptr<std::string> string;
    std::string::const_iterator begin;
    std::string::const_iterator end;

public:
    string_slice(std::string&& str) {
        this->string = std::make_shared<std::string>(std::move(str));
        this->begin = this->string->cbegin();
        this->end = this->string->cend();
    }

    std::pair<string_slice, string_slice> split_at(std::string::const_iterator iter) const {
        auto pre = string_slice(*this);
        auto post = string_slice(*this);

        pre.end = iter;
        post.begin = iter;
        return { pre, post };
    }

    static Option<string_slice> join_adjacent(string_slice left, string_slice right) {
        auto joined = string_slice(left);

        if (left.string == string_slice::def().string) {
            return Option(right);
        } else if (right.string == string_slice::def().string) {
            return Option(left);
        } else if (left.string != right.string) {
            return Option<string_slice>();
        }

        if (left.end == right.begin) {
            joined.end = right.end;
            return Option(joined);
        } else {
            return Option<string_slice>();
        }
    }

    static string_slice def() {
        static string_slice def = string_slice("");
        return def;
    }
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
