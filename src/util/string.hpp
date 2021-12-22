#ifndef FLEUR_STRING_H
#define FLEUR_STRING_H

#include <common.hpp>

namespace Fleur::Util
{
    /* Returns true if `c` is whitespace, ignoring newline:
     *
     * ' '  Space,
     * '\r' Carriage Return,
     * '\t' Horizontal Tab,
     * '\v' Vertical Tab,
     * '\f' Form Feed.
     */
    bool IsWhitespaceNoNewline(char c);

    /* Returns true if `c` is whitespace, which is defined as being equal to one of:
     *
     * ' '  Space,
     * '\r' Carriage Return,
     * '\t' Horizontal Tab,
     * '\v' Vertical Tab,
     * '\f' Form Feed,
     * '\n' Newline.
     */
    bool IsWhitespace(char c);

    // [A;Z]
    bool IsUppercaseLetter(char c);
    // [a;z]
    bool IsLowercaseLetter(char c);
    // [A;Z], [a;z] and '_'
    bool IsLetter(char c);
    // [0;9]
    bool IsDigit(char c);

    // Null-terminated string with length.
    struct String
    {
        char *data;
        u64 length;

        // Release `data`.
        void Free();
    };
}

#endif