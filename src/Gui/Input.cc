#include "Input.h"
#include "Utilities.h"

Input::Input(const MenuState menuState,
             unique_ptr<sf::Text> text,
             unique_ptr<sf::Sprite> sprite,
             const uint8_t maxInput,
             const string_view defaultString,
             const Alignment alignment,
             const AllowedInput allowedInput)
    : Gui(menuState, alignment), Text(move(text)), Sprite(move(sprite)), m_maxInput(maxInput),
      m_defaultString(defaultString), m_allowedInput(allowedInput)
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
    Utilities utilities;
    auto newWidth = utilities.CalculateAlignmentWindowWidth(width, m_alignment);

    m_string = "";
    m_text->setString(m_defaultString);
    utilities.SetInputPos(newWidth, m_text.get());
}

void Input::Write(const uint16_t width, const sf::Uint32 character)
{
    Utilities utilities;
    auto newWidth = utilities.CalculateAlignmentWindowWidth(width, m_alignment);

    bool inputAllowed = false;

    switch (m_allowedInput)
    {
    case AllowedInput::Alphabet:
        inputAllowed = utilities.isAlpha(character);
        break;
    case AllowedInput::Number:
        inputAllowed = utilities.isNum(character);
        break;
    case AllowedInput::Everything:
        inputAllowed = true;
        break;
    default:
        break;
    }

    if (m_string.size() < m_maxInput && inputAllowed)
    {
        m_string.insert(m_string.end(), character);
        m_text->setString(m_string);
        utilities.SetInputPos(newWidth, m_text.get());
    }
}

void Input::Popback(const uint16_t width)
{
    if (m_string.size() > 0)
    {
        Utilities utilities;
        auto newWidth = utilities.CalculateAlignmentWindowWidth(width, m_alignment);

        m_string.pop_back();
        m_text->setString(m_string);
        utilities.SetInputPos(newWidth, m_text.get());
    }
}
