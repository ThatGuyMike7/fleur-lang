#include <tokenizer.hpp>

int main()
{
    Fleur::Tokenizer tokenizer;
    Fleur::TokenizerData data = tokenizer.Tokenize(Fleur::Util::ReadFile("test.txt"));

    if (data.tokens.size() != 1)
    {
        return 1;
    }

    if (data.tokens[0].type != Fleur::TokenType::DIVIDE)
    {
        return 2;
    }

    return 0;
}