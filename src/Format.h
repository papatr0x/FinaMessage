// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#ifndef FORMAT_H
#define FORMAT_H

#include "CommonDefinitions.h"
#include "Field.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace Iso8583 {

class Format
{
private:
    Encoding defaultEncoding = Encoding::ASCII;
    std::unordered_map<int, FieldSpecification> fieldsSpecification;
    void applyTokens(const std::vector<std::string_view>& tokens);

public:
    Encoding getDefaultEncoding() const { return defaultEncoding; }
    const FieldSpecification& getFieldSpecification(const FieldId aFieldId) const
    {
        return fieldsSpecification.at(aFieldId);
    }

    static std::shared_ptr<const Format> fromDefinition(const std::string& filename);
};

} // Iso8583

#endif //FORMAT_H
