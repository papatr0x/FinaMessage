# Fina ISO-8583 Lib
(c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

This is an implementation of the ISO-8583 as it is published in wikipedia.
A long time ago, I used to work on a different system using this standard; it called my attention 
that all fields where defined in hard code, even when they were subject of change from time
to time requiring modifications to a system that was what we called a critical system today.

So the main motivation for this implementation is to allow defining a format based in ISO-8583
(like Visa or Mastercard) just on a text file and not require further compilation of the source code.
Also move to a modern version of C++ hoping to avoid the typical C patterns read/write binary 
data to a buffer.

At this moment this implementation uses C++23 features. I am hoping this could be compiled to
be embedded in a POS or another device that requires ISO-8583.

This is a work in progress. At the moment only ISO-8583 is working. No EBCDIC or BCD support. 

## Class summary

Message: is just data of a message, this is not tied to a specific format.
Format: is a definition of a format, this is the 128 fields main definitions, length, length type and encoding if required
MessageWriter: this renders the message to write to the network.
MessageReader: generates a Message instance on a received message.

## Main Scenarios

### On POS
The card data collected is used to build an instance of a message. Message contains FieldData
this is added with method addField();
After the message contains all data, the binary message is rendered with MessageWriter, which 
has MessageFormat associated.

### As Issuer
MessageReader instance
Generates a new Message.
This message is processed by the system and response code other required data is added.
MessageWriter instance is used to render the final response.

### As a Router
This case is the same as the POS case but it should consider different formats and channels.
A MessageReader instance generates a new Message.
The incoming message is processed.
MessageWriter instance is used to render the final response.






