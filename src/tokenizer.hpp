#ifndef FLEUR_TOKENIZER_HPP
#define FLEUR_TOKENIZER_HPP

#include <util/io.hpp>

#include <string_view>
#include <vector>

namespace Fleur
{
    enum class TokenType : u64
    {
        Identifier,
        Integer
    };

    struct Token
    {
        TokenType type;
        std::string_view string;
        u64 lineNumber;
        u64 lineColumn;
    };

    class Tokenizer
    {
    public:
        Tokenizer(Util::String &&source);
        ~Tokenizer();

        void Tokenize();
        std::vector<Token> tokens;

    private:
        Util::String source;
        u64 index;
        char c;

        u64 lineColumn;
        u64 lineNumber;

        //std::vector<Token> tokens;

        bool Advance();
        bool Peek(char *c);
        bool SkipWhitespace();

        void AddIdentifierToken(std::string_view string);
        void AddIntegerToken(std::string_view string);

        bool Identifier();
        bool Number();
    };
}

#endif