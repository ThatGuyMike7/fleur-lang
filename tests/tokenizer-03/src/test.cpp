#include <tokenizer.hpp>

int main()
{
    Fleur::Tokenizer tokenizer{ Fleur::Util::ReadFile("test.txt") };
    tokenizer.Tokenize();

    if (tokenizer.Tokens().size() != 12)
    {
        return 1;
    }

    if (tokenizer.Tokens()[0].type != Fleur::TokenType::IDENTIFIER)
    {
        return 2;
    }
    if (tokenizer.Tokens()[0].string != "this_is_quite_a_long_identifier")
    {
        return 3;
    }

    if (tokenizer.Tokens()[1].type != Fleur::TokenType::INTEGER)
    {
        return 4;
    }
    if (tokenizer.Tokens()[1].string != "702")
    {
        return 5;
    }

    if (tokenizer.Tokens()[2].type != Fleur::TokenType::IDENTIFIER)
    {
        return 6;
    }
    if (tokenizer.Tokens()[2].string != "a")
    {
        return 7;
    }

    if (tokenizer.Tokens()[3].type != Fleur::TokenType::IDENTIFIER)
    {
        return 8;
    }
    if (tokenizer.Tokens()[3].string != "b")
    {
        return 9;
    }

    if (tokenizer.Tokens()[4].type != Fleur::TokenType::INTEGER)
    {
        return 10;
    }
    if (tokenizer.Tokens()[4].string != "12345678901234567890")
    {
        return 11;
    }

    if (tokenizer.Tokens()[5].type != Fleur::TokenType::EQUAL)
    {
        return 12;
    }
    if (tokenizer.Tokens()[5].string != "==")
    {
        return 13;
    }

    if (tokenizer.Tokens()[6].type != Fleur::TokenType::INTEGER)
    {
        return 14;
    }
    if (tokenizer.Tokens()[6].string != "2")
    {
        return 15;
    }

    if (tokenizer.Tokens()[7].type != Fleur::TokenType::ASSIGNMENT)
    {
        return 16;
    }
    if (tokenizer.Tokens()[7].string != "=")
    {
        return 17;
    }

    if (tokenizer.Tokens()[8].type != Fleur::TokenType::INTEGER)
    {
        return 18;
    }
    if (tokenizer.Tokens()[8].string != "31")
    {
        return 19;
    }

    if (tokenizer.Tokens()[9].type != Fleur::TokenType::INTEGER)
    {
        return 20;
    }
    if (tokenizer.Tokens()[9].string != "0")
    {
        return 21;
    }

    if (tokenizer.Tokens()[10].type != Fleur::TokenType::LINE_COMMENT)
    {
        return 22;
    }
    if (tokenizer.Tokens()[10].string != "//")
    {
        return 23;
    }

    if (tokenizer.Tokens()[11].type != Fleur::TokenType::IDENTIFIER)
    {
        return 24;
    }
    if (tokenizer.Tokens()[11].string != "foo")
    {
        return 25;
    }

    return 0;
}