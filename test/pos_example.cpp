// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#include <iostream>

#include "DebugTools.h"
#include "Format.h"
#include "Message.h"
#include "MessageReader.h"
#include "MessageWriter.h"

int main()
{
    std::cout << "== Building a basic message\n";
    Iso8583::Message msg;
    msg.addField(Iso8583::MTI_FIELD, "0100");
    msg.addField(2, "4444333322221111");
    msg.addField(3, "001000");
    msg.addField(4, "000000000190");
    msg.addField(5, "000000000190");
    msg.addField(6, "000000000190");
    msg.addField(70, "999");
    std::cout << msg.toString() << std::endl;

    const auto format = Iso8583::Format::fromDefinition("../iso8583_wikipedia.def");

    std::cout << "== Serialize the message to some buffer\n";
    std::array<std::byte, 1024> buffer;
    const Iso8583::MessageWriter writer(format);
    const auto writtenBuffer = writer.writeBinaryMessage(msg, buffer);
    printBuffer(writtenBuffer);

    std::cout << "== Read a message from a binary buffer\n";
    Iso8583::Message fromBinary;
    Iso8583::MessageReader reader(format);
    reader.readBinaryMessage(fromBinary, writtenBuffer);
    std::cout << fromBinary.toString() << std::endl;
}
