#include "Gui.h"
#include "Sprite.h"
#include "Text.h"
#include "Utilities.h"

Gui::Gui(const MenuState menuState) : m_menuState(menuState)
{
}

MenuState Gui::GetMenuState() const
{
    return m_menuState;
}

void Gui::SetMenuState(const MenuState menuState)
{
    m_menuState = menuState;
}

Title::Title(const MenuState menuState, sf::Text *text) : Gui(menuState), Text(text)
{
}

Button::Button(const MenuState menuState, const BtnFunc btnfnc, sf::Text *text, sf::Sprite *sprite)
    : Gui(menuState), Text(text), Sprite(sprite), m_btnfnc(btnfnc)
{
}

BtnFunc Button::GetBtnFnc() const
{
    return m_btnfnc;
}

Input::Input(const MenuState menuState, sf::Text *text, const uint8_t maxChars)
    : Gui(menuState), Text(text), m_maxChars(maxChars)
{
}

string_view Input::GetName() const
{
    return m_string;
}

void Input::Write(const uint16_t width, const sf::Uint32 character)
{
    Utilities utilities;
    if (m_string.size() < m_maxChars && utilities.isAlpha(character))
    {
        m_string.insert(m_string.end(), character);
        m_text->setString(m_string);
        utilities.SetInputPos(width, m_text);
    }
}

void Input::Popback(const uint16_t width)
{
    if (m_string.size() > 0)
    {
        Utilities utilities;
        m_string.pop_back();
        m_text->setString(m_string);
        utilities.SetInputPos(width, m_text);
    }
}
