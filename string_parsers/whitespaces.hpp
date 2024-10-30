#pragma once

#include "atomic_parsers.hpp"
#include "char_sets.hpp"

namespace parser_combinators::string {

static const auto parse_whitespaces = kleene_one(parse_space);

}