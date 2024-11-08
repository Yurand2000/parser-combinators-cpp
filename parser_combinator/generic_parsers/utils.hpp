#pragma once

#include <iostream>
#include <tuple>
#include <utility>

// Tuple Utilities
// https://stackoverflow.com/a/24046437

namespace parser_combinators::utils {

template < typename T , typename... Ts >
auto head( std::tuple<T,Ts...> t )
{
   return  std::get<0>(t);
}

template < std::size_t... Ns , typename... Ts >
auto __tail_impl( std::index_sequence<Ns...> , std::tuple<Ts...> t )
{
   return  std::make_tuple( std::get<Ns+1u>(t)... );
}

template < typename... Ts >
auto tail( std::tuple<Ts...> t )
{
   return  __tail_impl( std::make_index_sequence<sizeof...(Ts) - 1u>() , t );
}

}

// other utility structures
namespace parser_combinators {

template <typename... Args>
struct Wrapper { };

template<typename Res, typename Parser>
struct ParserWrap {
   Parser p;

public:
   ParserWrap(Parser parser) : p(parser) { }
};

}