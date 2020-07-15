#include "tests.h"

#include <eggcpt/bits.h>

using namespace eggcpt;

void Tests::addTestsBits()
{
    add("ror", []() {
        bool result = true;
        result &= bits::ror(0x0000'00FF,  8) == 0xFF00'0000;
        result &= bits::ror(0x0000'00FF, 32) == 0x0000'00FF;
        result &= bits::ror(0x0000'00FF, 40) == 0xFF00'0000;

        return result;
    });

    add("rol", []() {
        bool result = true;
        result &= bits::rol(0xFF00'0000,  8) == 0x0000'00FF;
        result &= bits::rol(0xFF00'0000, 32) == 0xFF00'0000;
        result &= bits::rol(0xFF00'0000, 40) == 0x0000'00FF;

        return result;
    });

    add("clz", []() {
        bool result = true;
        result &= bits::clz(0x00FF'0000) == 8;
        result &= bits::clz(0x0000'FF00) == 16;
        result &= bits::clz(0x0000'0001) == 31;

        return result;
    });

    add("ctz", []() {
        bool result = true;
        result &= bits::ctz(0x0000'FF00) == 8;
        result &= bits::ctz(0x00FF'0000) == 16;
        result &= bits::ctz(0x8000'0000) == 31;

        return result;
    });

    add("popcnt", []() {
        bool result = true;
        result &= bits::popcnt(0x0000'0001) == 1;
        result &= bits::popcnt(0x0000'00FF) == 8;
        result &= bits::popcnt(0xFFFF'FFFF) == 32;

        return result;
    });

    add("bswap", []() {
        bool result = true;
        result &= bits::bswap(0x0000'0001) == 0x0100'0000;
        result &= bits::bswap(0x0000'00FF) == 0xFF00'0000;
        result &= bits::bswap(0xABCD'0000) == 0x0000'CDAB;

        return result;
    });
}