// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#ifndef MESSAGE_H
#define MESSAGE_H

#include "CommonDefinitions.h"
#include "Field.h"
#include <bitset>
#include <sstream>
#include <map>

namespace Iso8583 {

class Message {
    std::bitset<128> bitmap;
    std::map<unsigned, FieldData> fieldsData;

    friend class MessageWriter;

public:

    // Set bits from left to right.
    void setBit(const int fieldNo, const bool value = true)
    {
        bitmap.set(128-fieldNo, value);
    }

    void setFieldInBitmap(const int fieldNo)
    {
        setBit(fieldNo);

        // When using fields above 64, mark the presence of the secondary bmp
        if (fieldNo > 64)
        {
            setBit(1);
        }
    }

    void addField(const FieldId& aFieldId, const std::string_view value)
    {
        fieldsData.emplace(aFieldId, FieldData{aFieldId, value});
        if (aFieldId > 0)
        {
            setFieldInBitmap(aFieldId);
        }
    }

    FieldData& addEmptyField(const FieldId& aFieldId)
    {
        auto [it, ok] = fieldsData.emplace(aFieldId, FieldData{aFieldId});
        if (aFieldId > 0)
        {
            setFieldInBitmap(aFieldId);
        }
        return it->second;
    }

    std::string toString() const
    {
        std::ostringstream oss;

        const std::string bmp{bitmap.to_string()};
        oss << "BMP: " << bmp.substr(0, 64) << "\n"
               "     " << bmp.substr(64) << "\n";

        for (const auto& [k,v] : fieldsData)
        {
            if (k == MTI_FIELD)
            {
                oss << "MTI: " << v.value << '\n';

            }
            else
            {
                oss << "Field " << k << ": " << v.value << '\n';
            }

        }
        return oss.str();
    }
};

} // Iso8583

#endif //MESSAGE_H
