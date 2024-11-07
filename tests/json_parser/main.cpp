#include <iostream>
#include <format>

#include "parser_combinator/parser.hpp"
#include "json.hpp"

using namespace json;

int main() {

    auto test_json = R"(
{"widget": {
    "debug": "on",
    "window": {
        "title": "Sample Konfabulator Widget",
        "name": "main_window",
        "width": 500,
        "height": 500
    },
    "image": { 
        "src": ["Images/Sun.png", "Images/Sun2.png"],
        "name": "sun1",
        "hOffset": 250,
        "vOffset": 250,
        "alignment": "center"
    },
    "text": {
        "data": "Click Here",
        "size": 36,
        "style": "bold",
        "name": "text1",
        "hOffset": 250,
        "vOffset": 100,
        "alignment": "center",
        "onMouseUp": "sun1.opacity = (sun1.opacity / 100) * 90;"
    }
}}
)";

    auto result = json::parser::parse(test_json);
    
    if (result) {
        std::cout << std::format("Used: {}, Rest: {}", result->used, result->rest) << std::endl;
    } else {
        std::cout << std::format("Error: {}", result.error()) << std::endl;
    }
}