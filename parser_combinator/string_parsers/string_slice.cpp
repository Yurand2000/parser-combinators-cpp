#include "string_slice.hpp"

namespace parser_combinators::string {

string_slice::string_slice(std::string&& str) {
    this->string = std::make_shared<std::string>(std::move(str));
    this->begin = this->string->cbegin();
    this->end = this->string->cend();
}

std::pair<string_slice, string_slice> string_slice::split_at(std::string::const_iterator iter) const {
    auto pre = string_slice(*this);
    auto post = string_slice(*this);

    pre.end = iter;
    post.begin = iter;
    return { pre, post };
}

Option<string_slice> string_slice::join_adjacent(string_slice left, string_slice right) {
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

string_slice string_slice::def() {
    static string_slice def = string_slice("");
    return def;
}

}
