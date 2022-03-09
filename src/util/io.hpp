#ifndef FLEUR_IO_HPP
#define FLEUR_IO_HPP

#include <util/string.hpp>

#include <string_view>

namespace Fleur::Util
{
    // Sets `String::data` to `nullptr` on error.
    String ReadFile(std::string_view path);
}

#endif