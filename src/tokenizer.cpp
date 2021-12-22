#include <tokenizer.hpp>
#include <common.hpp>
#include <util/string.hpp>

#include <string_view>

namespace Fleur
{
    Tokenizer::Tokenizer(Util::String &&source)
        : source(std::move(source))
    {
    }

    Tokenizer::~Tokenizer()
    {
        source.Free();
    }

    bool Tokenizer::Advance()
    {
        index++;
        if (index == source.length)
        {
            return false;
        }

        c = source.data[index];
        return true;
    }

    bool Tokenizer::Peek(char *c)
    {
        if (index + 1 == source.length)
        {
            return false;
        }

        *c = source.data[index + 1];
        return true;
    }

    bool Tokenizer::SkipWhitespace()
    {
        while (true)
        {
            if (c == '\n')
            {
                lineNumber++;
                lineColumn = 1;
            }
            else if (!Util::IsWhitespace(c))
            {
                return false;
            }

            if (!Advance())
            {
                return false;
            }
        }
        return true;
    }

    void Tokenizer::AddIdentifierToken(std::string_view string)
    {
        tokens.push_back(Token{ TokenType::Identifier, string, lineNumber, lineColumn });
    }

    void Tokenizer::AddIntegerToken(std::string_view string)
    {
        tokens.push_back(Token{ TokenType::Integer, string, lineNumber, lineColumn });
    }

    bool Tokenizer::Identifier()
    {
        char *begin = source.data + index;
        u64 size = 1;

        while (true)
        {
            char peek;
            if (!Peek(&peek))
            {
                AddIdentifierToken(std::string_view{ begin, size });
                return false;
            }

            if (Util::IsLetter(peek) || Util::IsDigit(peek))
            {
                size++;
                Advance();
            }
            else
            {
                AddIdentifierToken(std::string_view{ begin, size });
                return true;
            }
        }
    }

    bool Tokenizer::Number()
    {
        char *begin = source.data + index;
        u64 size = 1;

        while (true)
        {
            char peek;
            if (!Peek(&peek))
            {
                AddIntegerToken(std::string_view{ begin, size });
                return false;
            }

            if (Util::IsDigit(peek))
            {
                size++;
                Advance();
            }
            else
            {
                AddIntegerToken(std::string_view{ begin, size });
                return true;
            }
        }
    }

    void Tokenizer::Tokenize()
    {
        if (source.length == 0)
        {
            // Empty file.
            return;
        }

        index = 0;
        c = source.data[index];
        lineColumn = 1;
        lineNumber = 1;

        while (true)
        {
            if (!Advance())
            {
                return;
            }

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
        }
    }
}