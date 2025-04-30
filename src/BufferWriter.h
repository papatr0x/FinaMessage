// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#ifndef BUFFER_WRITER_H
#define BUFFER_WRITER_H

#include <string_view>

namespace Iso8583 {

class BufferWriter {
private:
    std::span<std::byte> buffer;
    size_t position = 0;

public:
    explicit BufferWriter(std::span<std::byte> buffer) : buffer(buffer) {}

    void write(std::byte value)
    {
        if (position >= buffer.size()) {
            throw std::overflow_error("Buffer overflow");
        }
        buffer[position] = value;
        ++position;
    }

    void write(std::span<const std::byte> data)
    {
        if (position + data.size() > buffer.size()) {
            throw std::overflow_error("Buffer overflow");
        }
        std::ranges::copy(data, buffer.begin() + position);
        position += data.size();
    }

    // Numeric types
    template<typename T>
    void writeNumber(T value)
    {
        const auto bytes = reinterpret_cast<const std::byte*>(&value);
        write(std::span<const std::byte>(bytes, sizeof(T)));
    }

    void write(std::string_view data)
    {
        write(std::span<const std::byte>(reinterpret_cast<const std::byte*>(data.data()), data.size()));
    }

    void write(const std::string_view data, size_t maxLength)
    {
        const size_t len = std::min(data.size(), maxLength);
        for (size_t i = 0; i < len; ++i)
        {
            write(static_cast<std::byte>(data[i]));
        }
    }

    size_t getPosition() const { return position; }

    size_t remaining() const { return buffer.size() - position; }

    std::span<std::byte> written() const
    {
        return buffer.subspan(0, position);
    }
};

} // Iso8583

#endif //BUFFER_WRITER_H
