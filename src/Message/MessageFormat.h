#pragma once
#include <cstdint>
#include <string>

using namespace std;

class MessageFormat
{
public:
    MessageFormat(const uint16_t m_id, string_view format);
    ~MessageFormat() = default;

    uint16_t GetID() const;
    string GetFormat() const;

private:
    uint16_t m_id;
    string m_format;
};
