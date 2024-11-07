#pragma once
#include <concepts>
#include <functional>
#include <optional>
#include <expected>

namespace parser_combinators {

template <typename T>
concept slice =
    std::copy_constructible<T> &&
requires(T a, T b)
{
    T::def();
    T::join_adjacent(a, b);
};

struct unit {};
template <typename T> using Option = std::optional<T>;
template <typename T, typename E> using Result = std::expected<T, E>;
template <typename T> using Error = std::unexpected<T>;

template <typename Res, slice Slice> struct ParserResOk {
    Res result;
    Slice used;
    Slice rest;
};

template <typename Res, slice Slice> using Ok = ParserResOk<Res, Slice>;

template <typename Res, slice Slice, typename Err>
using ParserRes = Result<ParserResOk<Res, Slice>, Err>;

template <typename Res, slice Slice, typename Err>
using Parser = std::function<ParserRes<Res, Slice, Err>(Slice)>;

}