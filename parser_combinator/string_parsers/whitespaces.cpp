#include "whitespaces.hpp"

#include "char_sets.hpp"

namespace parser_combinators::string {

Parser<std::vector<char>, string_slice, std::string> parse_whitespaces() {
    return std::function(kleene_one(parse_space()));
}

}