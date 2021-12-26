#include <tokenizer.hpp>
#include <common.hpp>
#include <util/string.hpp>

#include <string_view>
#include <string>
#include <vector>
#include <iostream>

namespace Fleur
{
    std::vector<SymbolToken const*> SymbolTokensWithFirstChar(char c)
    {
        std::vector<SymbolToken const*> result;
        for (u64 i = 0; i < SYMBOL_TOKENS.size(); i++)
        {
            SymbolToken const *current = &SYMBOL_TOKENS[i];
            if (current->string[0] == c)
            {
                result.push_back(current);
            }
        }
        return result;
    }

    void SymbolTokensWithCharAt(std::vector<SymbolToken const*> *symbolTokens, char c, u64 index)
    {
        auto it = symbolTokens->begin();
        while (it != symbolTokens->end())
        {
            if ((*it)->string.length() <= index ||
                (*it)->string[index] != c)
            {
                it = symbolTokens->erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    SymbolToken const* MatchSymbolToken(std::vector<SymbolToken const*> const &symbolTokens,
        std::string_view match)
    {
        for (u64 i = 0; i < symbolTokens.size(); i++)
        {
            if (symbolTokens[i]->string == match)
            {
                return symbolTokens[i];
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
            else if (Util::IsWhitespaceNoNewline(c))
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
        std::string str;
        str += source.data[index];

        auto candidates = SymbolTokensWithFirstChar(str[0]);
        while (true)
        {
            auto temp = candidates;

            char peek;
            if (!Peek(&peek, str.length()))
            {
                // This is a situation where the string we have is shorter than some candidates.
                // Find the element that matches exactly.
                auto *match = MatchSymbolToken(candidates, str);
                if (match == nullptr)
                {
                    // Unknown token.
                    // TODO: Error message.
                }
                else
                {
                    AddSymbolToken(match->string, match->tokenType);
                }

                Eat(str.length() - 1);
                return false;
            }

            SymbolTokensWithCharAt(&temp, peek, str.length());

            if (temp.size() == 0)
            {
                break;
            }
            else
            {
                str += peek;
                candidates = temp;
            }
        }

        // Find the element that matches exactly.
        auto *match = MatchSymbolToken(candidates, str);

        /*
        std::cout << "----Candidates:" << std::endl;
        for (u64 i = 0; i < candidates.size(); i++)
        {
            std::cout << candidates[i]->string << std::endl;
        }
        std::cout << "----str:" << std::endl;
        std::cout << str << std::endl;
        */

        if (match == nullptr)
        {
            // Unknown token.
            // TODO: Error message.
        }
        else
        {
            AddSymbolToken(match->string, match->tokenType);
        }

        Eat(str.length() - 1);
        return true;
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