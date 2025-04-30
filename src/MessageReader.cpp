// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#include "MessageReader.h"

#include "BufferReader.h"
#include "CommonDefinitions.h"
#include "Field.h"
#include "Format.h"
#include "Message.h"

namespace Iso8583 {

int MessageReader::readVarLen(const LengthType llType, BufferReader& bufferReader)
{
    std::string varLen;
    bufferReader.read(varLen, static_cast<int>(llType));
    return std::stoi(varLen);
}

bool MessageReader::readBinaryMessage(Message& message, std::span<std::byte> inputBuffer) const
{
    BufferReader inputReader(inputBuffer);

    // Read MTI
    FieldData& mtiField = message.addEmptyField(MTI_FIELD);
    inputReader.read(mtiField.value, 4);


    // Read and process bitmap
    std::set<int> availableFields = readSingleBMP(inputReader);
    if (availableFields.contains(1))
    {
        availableFields.erase(1);
        // Read the secondary bitmap
        std::set<int> secondaryBitmap = readSingleBMP(inputReader, 64);
        availableFields.insert(secondaryBitmap.begin(), secondaryBitmap.end());
    }

    // Read present fields
    for (const int fieldId : availableFields)
    {
        const FieldSpecification& spec = format->getFieldSpecification(fieldId);
        FieldData& field = message.addEmptyField(fieldId);
        if (spec.lengthType == LengthType::Fixed)
        {
            inputReader.read(field.value, spec.length);
        }
        else
        {
            const int varLen = readVarLen(spec.lengthType, inputReader);
            inputReader.read(field.value, varLen);
        }
    }

    return false;
}

std::set<int> MessageReader::readSingleBMP(BufferReader& inputReader, int offset)
{
    std::set<int> result;
    int currentField = 1 + offset;
    for (int i=0; i<8; ++i)
    {
        std::byte bits = inputReader.read();
        for (int j=0; j<8; ++j)
        {
            if ((0b10000000 >> j) & static_cast<int>(bits))
            {
                result.insert(currentField);
            }
            ++currentField;
        }
    }
    return result;
}

} // Iso8583