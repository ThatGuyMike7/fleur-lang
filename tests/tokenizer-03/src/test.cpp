#include <tokenizer.hpp>

int main()
{
    Fleur::Tokenizer tokenizer;
    Fleur::TokenizerData data = tokenizer.Tokenize(Fleur::Util::ReadFile("test.txt"));

    if (data.tokens.size() != 12)
    {
        return 1;
    }

    if (data.tokens[0].type != Fleur::TokenType::IDENTIFIER)
    {
        return 2;
    }
    if (data.tokens[0].string != "this_is_quite_a_long_identifier")
    {
        return 3;
    }

    if (data.tokens[1].type != Fleur::TokenType::INTEGER)
    {
        return 4;
    }
    if (data.tokens[1].string != "702")
    {
        return 5;
    }

    if (data.tokens[2].type != Fleur::TokenType::IDENTIFIER)
    {
        return 6;
    }
    if (data.tokens[2].string != "a")
    {
        return 7;
    }

    if (data.tokens[3].type != Fleur::TokenType::IDENTIFIER)
    {
        return 8;
    }
    if (data.tokens[3].string != "b")
    {
        return 9;
    }

    if (data.tokens[4].type != Fleur::TokenType::INTEGER)
    {
        return 10;
    }
    if (data.tokens[4].string != "12345678901234567890")
    {
        return 11;
    }

    if (data.tokens[5].type != Fleur::TokenType::EQUAL)
    {
        return 12;
    }
    if (data.tokens[5].string != "==")
    {
        return 13;
    }

    if (data.tokens[6].type != Fleur::TokenType::INTEGER)
    {
        return 14;
    }
    if (data.tokens[6].string != "2")
    {
        return 15;
    }

    if (data.tokens[7].type != Fleur::TokenType::ASSIGNMENT)
    {
        return 16;
    }
    if (data.tokens[7].string != "=")
    {
        return 17;
    }

    if (data.tokens[8].type != Fleur::TokenType::INTEGER)
    {
        return 18;
    }
    if (data.tokens[8].string != "31")
    {
        return 19;
    }

    if (data.tokens[9].type != Fleur::TokenType::INTEGER)
    {
        return 20;
    }
    if (data.tokens[9].string != "0")
    {
        return 21;
    }

    if (data.tokens[10].type != Fleur::TokenType::LINE_COMMENT)
    {
        return 22;
    }
    if (data.tokens[10].string != "//")
    {
        return 23;
    }

    if (data.tokens[11].type != Fleur::TokenType::IDENTIFIER)
    {
        return 24;
    }
    if (data.tokens[11].string != "foo")
    {
        return 25;
    }

    return 0;
}