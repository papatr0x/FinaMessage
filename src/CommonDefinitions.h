// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#ifndef COMMON_DEFINITIONS_H
#define COMMON_DEFINITIONS_H

namespace Iso8583 {

constexpr int MAX_NUM_FIELDS = 128;
constexpr int MTI_FIELD = 0;

enum class Encoding
{
    ASCII = 0,
    BCD,
    EBCDIC
};

enum class LengthType
{
    Fixed   = 0,
    L       = 1,
    LL      = 2,
    LLL     = 3,
    LLLL    = 4
};

} // Iso8583

#endif //COMMON_DEFINITIONS_H
