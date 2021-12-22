#include <util/io.hpp>
#include <common.hpp>

#include <string_view>
#include <cstdlib>
#include <cstdio>

namespace Fleur::Util
{
    void String::Free()
    {
        std::free(this->data);
    }

    String ReadFile(std::string_view path)
    {
        constexpr u64 INITIAL_SIZE = 1024;
        constexpr u64 GROWTH_FACTOR = 3;

        String contents;

        //#pragma warning (disable: 4996)
        FILE *stream = std::fopen(path.data(), "r");
        if (stream == nullptr)
        {
            return {};
        }

        // Disable internal stream buffering.
        //std::setvbuf(stream, nullptr, _IONBF, 0);

        contents.data = static_cast<char*>(std::malloc(INITIAL_SIZE));
        u64 bufferSize = INITIAL_SIZE;
        u64 totalNumRead = 0;

        while (true)
        {
            u64 space = bufferSize - totalNumRead;
            u64 numRead = std::fread(contents.data + totalNumRead, 1, space, stream);
            totalNumRead += numRead;
            if (numRead < space) // Either EOF or error.
            {
                break;
            }

            bufferSize *= GROWTH_FACTOR;
            contents.data = static_cast<char*>(std::realloc(contents.data, bufferSize));
        }

        if (std::feof(stream))
        {
            // Trim the buffer.
            contents.data = static_cast<char*>(std::realloc(contents.data, totalNumRead + 1));
            *(contents.data + totalNumRead) = '\0';
            contents.length = totalNumRead;

            std::fclose(stream);
            return contents;
        }

        if (std::ferror(stream))
        {
            std::fclose(stream);
            std::free(contents.data);
            return {};
        }

        // EOF was not reached.
        std::fclose(stream);
        std::free(contents.data);
        return {};
    }
}