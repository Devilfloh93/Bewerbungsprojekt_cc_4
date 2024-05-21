#include "MessageFormat.h"

MessageFormat::MessageFormat(const uint16_t id, string_view format) : m_id(id), m_format(format)
{
}

uint16_t MessageFormat::GetID() const
{
    return m_id;
}

string MessageFormat::GetFormat() const
{
    return m_format;
}
