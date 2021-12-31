#include <common.hpp>
#include <tokenizer.hpp>

#include <iostream>

int main()
{
    Fleur::Tokenizer tokenizer;
    Fleur::TokenizerData data = tokenizer.Tokenize(Fleur::Util::ReadFile("CMakeLists.txt"));

    if (data.Source().data == nullptr)
    {
        return 1;
    }

    for (Fleur::u64 i = 0; i < data.tokens.size(); i++)
    {
        std::cout << data.tokens[i].string << std::endl;
    }

    return 0;
}