// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#ifndef MESSAGE_READER_H
#define MESSAGE_READER_H

#include <memory>
#include <span>
#include <set>

#include "BufferReader.h"
#include "CommonDefinitions.h"

namespace Iso8583 {

class Format;
class Message;
struct FieldSpecification;

class MessageReader {
private:
    std::shared_ptr<const Format> format;
    static std::set<int> readSingleBMP(BufferReader& inputReader, int offset = 0) ;

public:
    explicit MessageReader(const std::shared_ptr<const Format>& aFormat) : format(aFormat) { }

    static int readVarLen(const LengthType llType, BufferReader& bufferReader);

    bool readBinaryMessage(Message& message, std::span<std::byte> inputBuffer) const;


};

} // Iso8583

#endif //MESSAGE_READER_H
