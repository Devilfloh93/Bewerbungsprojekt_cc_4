#pragma once
#include "Gui.h"
#include "Text.h"

using namespace std;

class Input : public Gui, public Text
{
public:
    Input(const MenuState menuState,
          unique_ptr<sf::Text> text,
          const uint8_t maxChars,
          const string_view defaultString);
    ~Input() = default;

    string GetString() const;
    string GetDefaultString() const;

    void ResetToDefaultString(const uint16_t width);
    void Write(const uint16_t width, const sf::Uint32 character);
    void Popback(const uint16_t width);
    void UpdateInputPos(const uint16_t width);

private:
    string m_defaultString;
    string m_string;
    uint8_t m_maxChars;
};