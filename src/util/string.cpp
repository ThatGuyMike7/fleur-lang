#include <util/string.hpp>
#include <cstdlib>
#include <iostream>

namespace Fleur::Util
{
    bool IsWhitespaceNoNewline(char c)
    {
        return c == 9 || (c >= 11 && c <= 13) || c == ' ';
    }

    bool IsWhitespace(char c)
    {
        return (c >= 9 && c <= 13) || c == ' ';
    }

    bool IsUppercaseLetter(char c)
    {
        return (c >= 65 && c <= 90);
    }

    bool IsLowercaseLetter(char c)
    {
        return (c >= 97 && c <= 122);
    }

    bool IsLetter(char c)
    {
        return IsUppercaseLetter(c) || IsLowercaseLetter(c) || c == '_';
    }

    bool IsDigit(char c)
    {
        return (c >= 48 && c <= 57);
    }

    void String::Free()
    {
        std::free(this->data);
    }
}