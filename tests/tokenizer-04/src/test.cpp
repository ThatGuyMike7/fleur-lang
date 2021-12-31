#include <tokenizer.hpp>

int main()
{
    Fleur::Tokenizer tokenizer;
    Fleur::TokenizerData data = tokenizer.Tokenize(Fleur::Util::ReadFile("test.txt"));

    if (data.tokens[0].line != 1 || data.tokens[0].column != 1 || data.tokens[0].ColumnEnd() != 7)
    {
        return 1;
    }

    if (data.tokens[1].line != 1 || data.tokens[1].column != 8 || data.tokens[1].ColumnEnd() != 13)
    {
        return 2;
    }

    if (data.tokens[2].line != 1 || data.tokens[2].column != 13 || data.tokens[2].ColumnEnd() != 14)
    {
        return 3;
    }

    if (data.tokens[3].line != 3 || data.tokens[3].column != 1 || data.tokens[3].ColumnEnd() != 4)
    {
        return 4;
    }

    if (data.tokens[4].line != 3 || data.tokens[4].column != 9 || data.tokens[4].ColumnEnd() != 10)
    {
        return 5;
    }

    if (data.tokens[5].line != 4 || data.tokens[5].column != 5 || data.tokens[5].ColumnEnd() != 8)
    {
        return 6;
    }

    if (data.tokens[6].line != 4 || data.tokens[6].column != 8 || data.tokens[6].ColumnEnd() != 9)
    {
        return 7;
    }

    if (data.tokens[7].line != 4 || data.tokens[7].column != 12 || data.tokens[7].ColumnEnd() != 14)
    {
        return 8;
    }

    if (data.tokens[8].line != 11 || data.tokens[8].column != 41 || data.tokens[8].ColumnEnd() != 44)
    {
        return 9;
    }

    return 0;
}