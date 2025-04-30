// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#ifndef BUFFERREADER_H
#define BUFFERREADER_H

#include <cstddef>
#include <span>
#include <string>

namespace Iso8583 {

class BufferReader {
private:
    std::span<std::byte> buffer;
    size_t position = 0;
public:
    BufferReader(std::span<std::byte> buffer) : buffer(buffer) {}
    size_t getPosition() const { return position; }
    size_t remaining() const { return buffer.size() - position; }

    std::byte read()
    {
        if (position >= buffer.size()) {
            throw std::overflow_error("Buffer overflow");
        }
        const std::byte value = buffer[position];
        ++position;
        return value;
    }

    void read(std::string& dest, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
        {
            dest.push_back(static_cast<char>(read()));
        }
    }

};

} // Iso8583

#endif //BUFFERREADER_H
