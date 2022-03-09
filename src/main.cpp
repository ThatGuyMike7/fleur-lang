#include <common.hpp>
#include <string.hpp>
#include <io.hpp>
#include <tokenizer.hpp>
#include <parser.hpp>

#include <iostream>

int main()
{
    Fleur::Util::String file = Fleur::Util::ReadFile("CMakeLists.txt");
    if (file.data == nullptr)
        return 0;

    Fleur::Tokenizer tokenizer;
    Fleur::TokenizerData tokenizerData = tokenizer.Tokenize(std::move(file));
    if (tokenizerData.tokens.size() == 0)
        return 0;

    Fleur::Parser parser;
    Fleur::ParserData parserData = parser.Parse(&tokenizerData);

    return 0;
}