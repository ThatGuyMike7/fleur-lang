#include <tokenizer.hpp>
#include <common.hpp>
#include <util/string.hpp>

#include <string_view>
#include <vector>

namespace Fleur
{
    SymbolToken const* SymbolTokenWithCharAt(char c, u64 index)
    {
        for (u64 i = 0; i < SYMBOL_TOKENS.size(); i++)
        {
            SymbolToken const *current = &SYMBOL_TOKENS[i];
            if (current->string.length() >= index)
            {
                if (current->string[index] == c)
                {
                    return current;
                }
            }
        }
        return nullptr;
    }

    Tokenizer::Tokenizer(Util::String &&source)
        : source(std::move(source))
    {
    }

    Tokenizer::~Tokenizer()
    {
        source.Free();
    }

    bool Tokenizer::Peek(char *peek, u64 amount)
    {
        if (index + amount >= source.length)
        {
            return false;
        }

        *peek = source.data[index + amount];
        return true;
    }

    void Tokenizer::Eat(u64 amount)
    {
        index += amount;
        lineColumn += amount;
    }

    bool Tokenizer::EatWhitespace()
    {
        while (true)
        {
            char c;
            if (!Peek(&c))
            {
                return false;
            }

            if (c == '\n')
            {
                lineNumber++;
                lineColumn = 0;
                index++;
            }
            else if(Util::IsWhitespaceNoNewline(c))
            {
                lineColumn++;
                index++;
            }
            else
            {
                return true;
            }
        }
    }

    void Tokenizer::AddIdentifierToken(std::string_view string)
    {
        tokens.push_back(Token{ TokenType::IDENTIFIER, string, lineNumber, lineColumn });
    }

    void Tokenizer::AddIntegerToken(std::string_view string)
    {
        tokens.push_back(Token{ TokenType::INTEGER, string, lineNumber, lineColumn });
    }

    void Tokenizer::AddSymbolToken(std::string_view string, TokenType tokenType)
    {
        tokens.push_back(Token{ tokenType, string, lineNumber, lineColumn });
    }

    bool Tokenizer::Identifier()
    {
        char const *begin = source.data + index;
        u64 size = 1;

        while (true)
        {
            char peek;
            if (!Peek(&peek, size))
            {
                AddIdentifierToken(std::string_view{ begin, size });
                Eat(size - 1);
                return false;
            }

            if (Util::IsLetter(peek) || Util::IsDigit(peek))
            {
                size++;
            }
            else
            {
                AddIdentifierToken(std::string_view{ begin, size });
                Eat(size - 1);
                return true;
            }
        }
    }

    bool Tokenizer::Number()
    {
        char const *begin = source.data + index;
        u64 size = 1;

        while (true)
        {
            char peek;
            if (!Peek(&peek, size))
            {
                AddIntegerToken(std::string_view{ begin, size });
                Eat(size - 1);
                return false;
            }

            if (Util::IsDigit(peek))
            {
                size++;
            }
            else
            {
                AddIntegerToken(std::string_view{ begin, size });
                Eat(size - 1);
                return true;
            }
        }
    }

    bool Tokenizer::SymbolToken()
    {
        char c = source.data[index];
        Fleur::SymbolToken const *token = nullptr;

        u64 i = 0;
        while (true)
        {
            Fleur::SymbolToken const *nextToken = SymbolTokenWithCharAt(c, i);
            if (nextToken == nullptr) // Either a full match or no match.
            {
                if (token == nullptr)
                {
                    // No match. This means the symbol is unknown.
                    // TODO: Print Error: Unknown token.
                }
                else
                {
                    // Note: String view is into `SYMBOL_TOKENS`, not into the source file.
                    AddSymbolToken(token->string, token->tokenType);
                    Eat(i - 1);
                }

                return true;
            }

            if (!Peek(&c, i + 1))
            {
                // EOF. Add what we have.
                AddSymbolToken(nextToken->string, nextToken->tokenType);
                Eat(i);
                return false;
            }

            token = nextToken;
            i++;
        }
    }

    void Tokenizer::Tokenize()
    {
        if (source.length == 0)
        {
            // Empty file.
            return;
        }

        index = static_cast<u64>(-1); // Wraparound is defined for unsigned types.
        lineColumn = 0;
        lineNumber = 1;

        while (true)
        {
            if (!EatWhitespace())
            {
                return;
            }

            char c;
            if (!Peek(&c))
            {
                return;
            }
            Eat();

            if (Util::IsLetter(c))
            {
                if (!Identifier())
                {
                    return;
                }
            }
            else if (Util::IsDigit(c))
            {
                if (!Number())
                {
                    return;
                }
            }
            else
            {
                if (!SymbolToken())
                {
                    return;
                }
            }
        }
    }
}