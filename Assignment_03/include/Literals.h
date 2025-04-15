#pragma once
#include <cmath>
#include <cstdint>


using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

using f32 = std::float_t;
using f64 = std::double_t;
using f128 = long double;

static_assert(sizeof(f32) == 0x4);
static_assert(sizeof(f64) == 0x8);
static_assert(sizeof(f128) == 0x10);