#ifndef FLEUR_TOKENIZER_HPP
#define FLEUR_TOKENIZER_HPP

#include <util/io.hpp>

#include <string>
#include <string_view>
#include <vector>

namespace Fleur
{
    enum class TokenType : u64
    {
        IDENTIFIER,
        INTEGER,
        LINE_COMMENT,
        MULTILINE_COMMENT_BEGIN,
        MULTILINE_COMMENT_END,
        SEMICOLON,
        COLON,
        DOT,
        COMMA,
        OPEN_ROUND_BRACKET,
        CLOSE_ROUND_BRACKET,
        OPEN_SQUARE_BRACKET,
        CLOSE_SQUARE_BRACKET,
        OPEN_CURLY_BRACKET,
        CLOSE_CURLY_BRACKET,
        POINTER,
        QUESTION_MARK,
        ASSIGNMENT,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        AMPERSAND,
        VERTICAL_BAR,
        EXCLAMATION_MARK,
        LESS,
        GREATER,
        DOUBLE_AMPERSAND,
        DOUBLE_VERTICAL_BAR,
        NOT_EQUAL,
        EQUAL,
        LESS_OR_EQUAL,
        GREATER_OR_EQUAL,
        DOUBLE_COLON,
        RIGHT_ARROW
    };

    struct Token
    {
        TokenType type;
        std::string_view string;
        u64 line;
        u64 column;

        // Returns the column of the last character of the token.
        u64 ColumnEnd() const;
    };

    struct SymbolToken
    {
        std::string string;
        TokenType tokenType;
    };

    inline std::vector<SymbolToken> const SYMBOL_TOKENS = {
        { "//", TokenType::LINE_COMMENT },
        { "/*", TokenType::MULTILINE_COMMENT_BEGIN },
        { "*/", TokenType::MULTILINE_COMMENT_END },
        { ";", TokenType::SEMICOLON },
        { ":", TokenType::COLON },
        { ".", TokenType::DOT },
        { ",", TokenType::COMMA },
        { "(", TokenType::OPEN_ROUND_BRACKET },
        { ")", TokenType::CLOSE_ROUND_BRACKET },
        { "[", TokenType::OPEN_SQUARE_BRACKET },
        { "]", TokenType::CLOSE_SQUARE_BRACKET },
        { "{", TokenType::OPEN_CURLY_BRACKET },
        { "}", TokenType::CLOSE_CURLY_BRACKET },
        { "^", TokenType::POINTER },
        { "?", TokenType::QUESTION_MARK },
        { "=", TokenType::ASSIGNMENT },
        { "+", TokenType::PLUS },
        { "-", TokenType::MINUS },
        { "*", TokenType::MULTIPLY },
        { "/", TokenType::DIVIDE },
        { "&", TokenType::AMPERSAND },
        { "|", TokenType::VERTICAL_BAR },
        { "!", TokenType::EXCLAMATION_MARK },
        { "<", TokenType::LESS },
        { ">", TokenType::GREATER },
        { "&&", TokenType::DOUBLE_AMPERSAND },
        { "||", TokenType::DOUBLE_VERTICAL_BAR },
        { "!=", TokenType::NOT_EQUAL },
        { "==", TokenType::EQUAL },
        { "<=", TokenType::LESS_OR_EQUAL },
        { ">=", TokenType::GREATER_OR_EQUAL },
        { "::", TokenType::DOUBLE_COLON },
        { "->", TokenType::RIGHT_ARROW }
    };

    // Returns every element of `SYMBOL_TOKENS` whose first character equals `c`.
    std::vector<SymbolToken const*> SymbolTokensWithFirstChar(char c);

    // Remove all elements from `symbolTokens` that do not have character `c` at `index`.
    void SymbolTokensWithCharAt(std::vector<SymbolToken const*> *symbolTokens, char c, u64 index);

    SymbolToken const* MatchSymbolToken(std::vector<SymbolToken const*> const &symbolTokens,
        std::string_view match);

    class TokenizerData
    {
    public:
        TokenizerData(Util::String &&source);
        ~TokenizerData();
        TokenizerData(TokenizerData &&other);

        std::vector<Token> tokens;

        Util::String const& Source() const;

    private:
        TokenizerData(TokenizerData const&) = delete;
        TokenizerData& operator=(TokenizerData const&) = delete;
        TokenizerData& operator=(TokenizerData&&) = delete;

        Util::String source;
    };

    class Tokenizer
    {
    public:
        TokenizerData Tokenize(Util::String &&source);

    private:
        u64 index;
        u64 column;
        u64 line;

        bool Peek(Util::String const &source, char *peek, u64 amount = 1);

        // Note: Only eat what you peeked before!
        void Eat(u64 amount = 1);

        // Skip whitespace and manage line number.
        bool EatWhitespace(Util::String const &source);

        void AddIdentifierToken(std::vector<Token> *tokens, std::string_view string);
        void AddIntegerToken(std::vector<Token> *tokens, std::string_view string);
        void AddSymbolToken(std::vector<Token> *tokens, std::string_view string,
            TokenType tokenType);

        bool Identifier(std::vector<Token> *tokens, Util::String const &source);
        bool Number(std::vector<Token> *tokens, Util::String const &source);
        bool SymbolToken(std::vector<Token> *tokens, Util::String const &source);
    };
}

#endif