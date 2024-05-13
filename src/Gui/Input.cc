#include "Input.h"
#include "Utilities.h"

Input::Input(const MenuState menuState,
             unique_ptr<sf::Text> text,
             const uint8_t maxChars,
             const string_view defaultString,
             const Alignment alignment)
    : Gui(menuState, alignment), Text(move(text)), m_maxChars(maxChars), m_defaultString(defaultString)
{
}

string Input::GetString() const
{
    return m_string;
}

string Input::GetDefaultString() const
{
    return m_defaultString;
}

void Input::ResetToDefaultString(const uint16_t width)
{
    m_string = "";
    m_text->setString(m_defaultString);
    UpdateInputPos(width);
}

void Input::Write(const uint16_t width, const sf::Uint32 character)
{
    Utilities utilities;
    if (m_string.size() < m_maxChars && utilities.isAlpha(character))
    {
        m_string.insert(m_string.end(), character);
        m_text->setString(m_string);
        utilities.SetInputPos(width, m_text.get());
    }
}

void Input::Popback(const uint16_t width)
{
    if (m_string.size() > 0)
    {
        Utilities utilities;
        m_string.pop_back();
        m_text->setString(m_string);
        UpdateInputPos(width);
    }
}

void Input::UpdateInputPos(const uint16_t width)
{
    Utilities utilities;
    utilities.SetInputPos(width, m_text.get());
}
