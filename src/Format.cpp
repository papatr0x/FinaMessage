// (c) 2025 Patricio Palma <ppalma.dev@protonmail.com>

#include "Format.h"

#include <charconv>
#include <fstream>
#include <iostream>

namespace Iso8583 {

const std::unordered_map<std::string_view, Encoding> TableEncoding = {
    { "ASCII", Encoding::ASCII },
    { "BCD", Encoding::BCD }
};

const std::unordered_map<std::string_view, LengthType> TableLengthType = {
    { "..", LengthType::LL},
    { "...", LengthType::LLL}
};

std::vector<std::string_view> getTokenFromDefinitionLine(std::string_view input)
{
    std::vector<std::string_view> tokens;
    size_t i = 0;
    while (true)
    {
        if (const size_t space = input.find(' ', i); space != std::string::npos)
        {
            tokens.emplace_back(input.substr(i, space - i));
            i = space+1;
            while (input[i] == ' ') ++i;
        }
        else
        {
            tokens.emplace_back(input.substr(i));
            break;
        }
    }
    return tokens;
}

void removeInitialSpaces(std::string& line)
{
    int start = 0;
    while (std::isblank(line[start])) ++start;
    if (start > 0)
    {
        line = line.substr(start);
    }
}

void removeComments(std::string& line)
{
    if (const size_t pos = line.find('#'); pos != std::string::npos)
    {
        line = line.substr(0, pos);
    }
}

void Format::applyTokens(const std::vector<std::string_view>& tokens)
{
    if (tokens[0] == "ENCODING")
    {
        defaultEncoding = TableEncoding.at(tokens[1]);
        fieldsSpecification.emplace(MTI_FIELD, FieldSpecification{MTI_FIELD, LengthType::Fixed, 4, defaultEncoding});

    }
    else if (tokens[0] == "MTI")
    {
        fieldsSpecification.emplace(MTI_FIELD, FieldSpecification{MTI_FIELD, LengthType::Fixed, 4,  TableEncoding.at(tokens[1])});
    }
    else if (tokens[0] == "FIELD")
    {
        int fieldNo = 0;
        std::from_chars(tokens[1].data(), tokens[1].data() + tokens[1].size(), fieldNo);

        size_t fieldSize = 0;
        const std::string_view& fieldSizeStr = tokens[3][0] == '.' ? tokens[4] : tokens[3];
        std::from_chars(fieldSizeStr.data(), fieldSizeStr.data() + fieldSizeStr.size(), fieldSize);

        fieldsSpecification.emplace(fieldNo, FieldSpecification{fieldNo, tokens[3][0] == '.' ? TableLengthType.at(tokens[3]) : LengthType::Fixed,fieldSize, Encoding::ASCII});
    }
}

std::shared_ptr<const Format> Format::fromDefinition(const std::string& filename)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        return nullptr;
    }
    auto theFormat = std::make_shared<Format>();
    std::string line;
    while (std::getline(input, line))
    {
        removeInitialSpaces(line);
        removeComments(line);
        if (line.empty())
        {
            continue;
        }
        theFormat->applyTokens(getTokenFromDefinitionLine(line));
    }
    return theFormat;
}

} // Iso8583
