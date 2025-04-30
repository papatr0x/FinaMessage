// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#include "DebugTools.h"

#include <iomanip>
#include <iostream>

void printBuffer(std::span<std::byte> buffer)
{
    for (size_t i = 0; i < buffer.size(); ++i)
    {
        if (i > 0)
        {
            if (i % 16 == 0)
            {
                std::cout << '\n';
            }
            else if (i % 4 == 0)
            {
                std::cout << "  ";
            }
        }
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]) << ' ';
    }
    std::cout << std::endl;
}
