#ifndef FLEUR_COMMON_HPP
#define FLEUR_COMMON_HPP

#include <cstdint>
#include <type_traits>

namespace Fleur
{
    using i64 = int64_t;
    using i32 = int32_t;
    using i16 = int16_t;
    using i8 = int8_t;

    using u64 = uint64_t;
    using u32 = uint32_t;
    using u16 = uint16_t;
    using u8 = uint8_t;

    using f64 = double;
    using f32 = float;

    static_assert(std::is_same<u64, std::size_t>::value);
}

#endif