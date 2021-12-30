#include <tokenizer.hpp>

int main()
{
    Fleur::Tokenizer tokenizer{ Fleur::Util::ReadFile("test.txt") };
    tokenizer.Tokenize();

    if (tokenizer.Tokens().size() != 1)
    {
        return 1;
    }

    if (tokenizer.Tokens()[0].type != Fleur::TokenType::DIVIDE)
    {
        return 2;
    }

    return 0;
}