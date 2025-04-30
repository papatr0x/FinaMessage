// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#ifndef MESSAGE_WRITER_H
#define MESSAGE_WRITER_H

#include <bitset>
#include <memory>
#include <span>

#include "CommonDefinitions.h"

namespace Iso8583 {

class Format;
class BufferWriter;
class Message;
struct FieldSpecification;
struct FieldData;

class MessageWriter {
private:
    std::shared_ptr<const Format> format;

    static void writeField(const FieldSpecification& spec, std::string_view data, BufferWriter& outputWriter);

    // void writeEBCDIC(const FieldSpecification& spec, const std::string& value, const BufferWriter& buffer_writer) const;
    //
    // void writeBCD(const FieldSpecification& spec, const std::string& value, const BufferWriter& buffer_writer) const;

    void writeField(const FieldData& data, BufferWriter& outputWriter) const;

    static void writeBMP(const std::bitset<128>& bitmap, bool longBMP, BufferWriter& outputWriter);

    static void writeSingleBMP(const std::bitset<128>& bitmap, int start, BufferWriter& outputWriter);

    static void writeVarLen(size_t fieldLength, LengthType lengthType, BufferWriter& outputWriter);

public:
    explicit MessageWriter(const std::shared_ptr<const Format>& aFormat) : format(aFormat) { }

    std::span<std::byte> writeBinaryMessage(const Message& message, std::span<std::byte> outputBuffer) const;

};

} // Iso8583

#endif //MESSAGE_WRITER_H
