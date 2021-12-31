#include <tokenizer.hpp>
#include <common.hpp>
#include <util/string.hpp>

#include <string_view>
#include <string>
#include <vector>
#include <iostream>

namespace Fleur
{
    u64 Token::ColumnEnd() const
    {
        return column + string.length();
    }

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

    TokenizerData::TokenizerData(Util::String &&source)
        : source(std::move(source))
    {
    }

    TokenizerData::~TokenizerData()
    {
        source.Free();
    }

    TokenizerData::TokenizerData(TokenizerData &&other)
        : source(std::move(other.source)), tokens(std::move(other.tokens))
    {
        other.source.data = nullptr; // Avoid double-free.
    }

    Util::String const& TokenizerData::Source() const
    {
        return source;
    }

    bool Tokenizer::Peek(Util::String const &source, char *peek, u64 amount)
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
        column += amount;
    }

    bool Tokenizer::EatWhitespace(Util::String const &source)
    {
        while (true)
        {
            char c;
            if (!Peek(source, &c))
            {
                return false;
            }

            if (c == '\n')
            {
                line++;
                column = 0;
                index++;
            }
            else if (Util::IsWhitespaceNoNewline(c))
            {
                column++;
                index++;
            }
            else
            {
                return true;
            }
        }
    }

    void Tokenizer::AddIdentifierToken(std::vector<Token> *tokens, std::string_view string)
    {
        tokens->push_back(Token{ TokenType::IDENTIFIER, string, line, column });
    }

    void Tokenizer::AddIntegerToken(std::vector<Token> *tokens, std::string_view string)
    {
        tokens->push_back(Token{ TokenType::INTEGER, string, line, column });
    }

    void Tokenizer::AddSymbolToken(std::vector<Token> *tokens, std::string_view string,
        TokenType tokenType)
    {
        tokens->push_back(Token{ tokenType, string, line, column });
    }

    bool Tokenizer::Identifier(std::vector<Token> *tokens, Util::String const &source)
    {
        char const *begin = source.data + index;
        u64 size = 1;

        while (true)
        {
            char peek;
            if (!Peek(source, &peek, size))
            {
                AddIdentifierToken(tokens, std::string_view{ begin, size });
                Eat(size - 1);
                return false;
            }

            if (Util::IsLetter(peek) || Util::IsDigit(peek))
            {
                size++;
            }
            else
            {
                AddIdentifierToken(tokens, std::string_view{ begin, size });
                Eat(size - 1);
                return true;
            }
        }
    }

    bool Tokenizer::Number(std::vector<Token> *tokens, Util::String const &source)
    {
        char const *begin = source.data + index;
        u64 size = 1;

        while (true)
        {
            char peek;
            if (!Peek(source, &peek, size))
            {
                AddIntegerToken(tokens, std::string_view{ begin, size });
                Eat(size - 1);
                return false;
            }

            if (Util::IsDigit(peek))
            {
                size++;
            }
            else
            {
                AddIntegerToken(tokens, std::string_view{ begin, size });
                Eat(size - 1);
                return true;
            }
        }
    }

    bool Tokenizer::SymbolToken(std::vector<Token> *tokens, Util::String const &source)
    {
        std::string str;
        str += source.data[index];

        auto candidates = SymbolTokensWithFirstChar(str[0]);
        while (true)
        {
            auto temp = candidates;

            char peek;
            if (!Peek(source, &peek, str.length()))
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
                    AddSymbolToken(tokens, match->string, match->tokenType);
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
            AddSymbolToken(tokens, match->string, match->tokenType);
        }

        Eat(str.length() - 1);
        return true;
    }

    TokenizerData Tokenizer::Tokenize(Util::String &&source)
    {
        TokenizerData data{ std::move(source) };

        if (data.Source().length == 0)
        {
            // Empty file.
            return data;
        }

        index = static_cast<u64>(-1); // Wraparound is defined for unsigned integer types.
        column = 0;
        line = 1;

        while (true)
        {
            if (!EatWhitespace(data.Source()))
            {
                return data;
            }

            char c;
            if (!Peek(data.Source(), &c))
            {
                return data;
            }
            Eat();

            if (Util::IsLetter(c))
            {
                if (!Identifier(&data.tokens, data.Source()))
                {
                    return data;
                }
            }
            else if (Util::IsDigit(c))
            {
                if (!Number(&data.tokens, data.Source()))
                {
                    return data;
                }
            }
            else
            {
                if (!SymbolToken(&data.tokens, data.Source()))
                {
                    return data;
                }
            }
        }
    }
}