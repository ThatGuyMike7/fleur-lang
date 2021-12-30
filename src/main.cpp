#include <common.hpp>
#include <tokenizer.hpp>

#include <iostream>

int main()
{
    Fleur::Tokenizer tokenizer{ Fleur::Util::ReadFile("CMakeLists.txt") };
    tokenizer.Tokenize();

    for (Fleur::u64 i = 0; i < tokenizer.Tokens().size(); i++)
    {
        if (tokenizer.Tokens()[i].type == Fleur::TokenType::IDENTIFIER)
        {
            std::cout << "Identifier: ";
        }
        else if (tokenizer.Tokens()[i].type == Fleur::TokenType::INTEGER)
        {
            std::cout << "Integer: ";
        }
        else
        {
            std::cout << "Symbol Token with ID " << static_cast<Fleur::u64>(tokenizer.Tokens()[i].type) << ": ";
        }

        std::cout << tokenizer.Tokens()[i].string << std::endl;
    }

    return 0;
}