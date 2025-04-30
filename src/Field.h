// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#ifndef FIELD_DEFINITION_H
#define FIELD_DEFINITION_H

#include "CommonDefinitions.h"
#include <string>

namespace Iso8583 {

struct FieldSpecification
{
    int id;
    LengthType lengthType;
    size_t length;
    Encoding encoding;
};

struct FieldId {
    int id;
    constexpr FieldId(const int aId)
    {
        if (aId > 128 || aId < 0)
        {
            throw std::out_of_range("Field id must be between 0 and 128.");
        }
        id = aId;
    }
    constexpr operator int() const { return id; }
    const int& get() const { return id; }
};

struct FieldData
{
    int id;
    std::string value;
    FieldData() = default;
    FieldData(const FieldId& aFieldId) : id(aFieldId) {}
    FieldData(const FieldId& aFieldId, const std::string_view aValue) : id(aFieldId), value(aValue) {}
    // void set(const FieldId& aFieldId, const std::string_view aValue)
    // {
    //     id = aFieldId;
    //     value = aValue;
    // }
};

} // Iso8583

#endif //FIELD_DEFINITION_H
