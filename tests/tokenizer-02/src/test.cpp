#include <tokenizer.hpp>

int main()
{
    Fleur::Tokenizer tokenizer;
    Fleur::TokenizerData data = tokenizer.Tokenize(Fleur::Util::ReadFile("test.txt"));

    if (data.tokens.size() != 1)
    {
        return 1;
    }

    if (data.tokens[0].type != Fleur::TokenType::INTEGER)
    {
        return 2;
    }

    if (data.tokens[0].string != "42")
    {
        return 3;
    }

    return 0;
}