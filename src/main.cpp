#include <common.hpp>
#include <tokenizer.hpp>

#include <iostream>

int main()
{
    Fleur::Tokenizer tokenizer{ Fleur::Util::ReadFile("CMakeLists.txt") };
    tokenizer.Tokenize();

    for (Fleur::u64 i = 0; i < tokenizer.tokens.size(); i++)
    {
        if (tokenizer.tokens[i].type == Fleur::TokenType::Identifier)
        {
            std::cout << "Identifier: ";
        }
        else if (tokenizer.tokens[i].type == Fleur::TokenType::Integer)
        {
            std::cout << "Integer: ";
        }

        std::cout << tokenizer.tokens[i].string << std::endl;
    }

    return 0;
}