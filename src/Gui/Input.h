#pragma once
#include "Gui.h"
#include "Sprite.h"
#include "Text.h"

using namespace std;

enum class AllowedInput
{
    Nothing = 0,
    Everything,
    Alphabet,
    Number
};

class Input : public Gui, public Text, public Sprite
{
public:
    Input(const MenuState menuState,
          unique_ptr<sf::Text> text,
          unique_ptr<sf::Sprite> sprite,
          const uint8_t maxInput,
          const string_view defaultString,
          const Alignment alignment,
          const AllowedInput allowedInput);
    ~Input() = default;

    string GetString() const;
    string GetDefaultString() const;

    void ResetToDefaultString(const uint16_t width);
    void Write(const uint16_t width, const sf::Uint32 character);
    void Popback(const uint16_t width);

private:
    string m_defaultString;
    string m_string;
    uint8_t m_maxInput;
    AllowedInput m_allowedInput;
};
