// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#include "MessageWriter.h"
#include "Format.h"
#include "BufferWriter.h"
#include "Message.h"

namespace Iso8583 {

void MessageWriter::writeField(const FieldData& data, BufferWriter& outputWriter) const
{
    const FieldSpecification& spec = format->getFieldSpecification(data.id);
    if (spec.encoding == Encoding::ASCII)
    {
        writeField(spec, data.value, outputWriter);
    }
    // else if (spec.encoding == Encoding::EBCDIC)
    // {
    //     writeEBCDIC(spec, data.value, outputWriter);
    // }
    // else if (spec.encoding == Encoding::BCD)
    // {
    //     writeBCD(spec, data.value, outputWriter);
    // }
}

void MessageWriter::writeField(const FieldSpecification& spec, const std::string_view data, BufferWriter& outputWriter)
{
    const size_t length = std::min(spec.length, data.size());
    if (spec.lengthType == LengthType::Fixed)
    {
        outputWriter.write(data, spec.length);
        for (size_t i = length; i < spec.length; ++i)
        {
            outputWriter.write(static_cast<std::byte>(' '));
        }
    }
    else
    {
        writeVarLen(length, spec.lengthType, outputWriter);
        outputWriter.write(data, length);
    }
}

void MessageWriter::writeSingleBMP(const std::bitset<128>& bitmap, const int start, BufferWriter& outputWriter)
{
    std::bitset<64> tmpBMP;
    for (int i = start, j = 63; i >= (start-63) ; --i, --j)
    {
        tmpBMP.set(j, bitmap.test(i));
    }
    const unsigned long long bmpValue = htonll(tmpBMP.to_ullong());
    outputWriter.writeNumber(bmpValue);
}

void MessageWriter::writeVarLen(size_t fieldLength, const LengthType lengthType, BufferWriter& outputWriter)
{
    const int length = static_cast<int>(lengthType);
    std::string data(length, '0');
    for (int i=length-1; i>=0; --i)
    {
        data[i] = 0x30 | fieldLength % 10;
        fieldLength /= 10;
    }
    outputWriter.write(data);
}

void MessageWriter::writeBMP(const std::bitset<128>& bitmap, bool longBMP, BufferWriter& outputWriter)
{
    writeSingleBMP(bitmap, 127, outputWriter);
    if (longBMP)
    {
        writeSingleBMP(bitmap, 63, outputWriter);
    }
}

std::span<std::byte> MessageWriter::writeBinaryMessage(const Message& message, std::span<std::byte> outputBuffer) const
{
    BufferWriter outputWriter(outputBuffer);

    writeField(message.fieldsData.at(MTI_FIELD), outputWriter);
    writeBMP(message.bitmap, message.fieldsData.crbegin()->first > 64, outputWriter);
    for (const auto& [fieldId, fieldData] : message.fieldsData)
    {
        if (fieldId == MTI_FIELD) continue;
        writeField(fieldData, outputWriter);
    }

    return outputWriter.written();
}

} // Iso8583

