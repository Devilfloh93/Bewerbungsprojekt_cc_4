#include "Gui.h"


Title::Title(const MenuState menuState, sf::Text *text) : m_menuState(menuState), m_text(text)
{
}

Button::Button(const MenuState menuState, const BtnFunc btnfnc, sf::Text *text, sf::Sprite *sprite)
    : m_menuState(menuState), m_text(text), m_sprite(sprite), m_btnfnc(btnfnc)
{
}

sf::Text *Title::GetText() const
{
    return m_text;
}

MenuState Title::GetMenuState() const
{
    return m_menuState;
}

sf::Text *Button::GetText() const
{
    return m_text;
}

MenuState Button::GetMenuState() const
{
    return m_menuState;
}

sf::Sprite *Button::GetSprite() const
{
    return m_sprite;
}

BtnFunc Button::GetBtnFnc() const
{
    return m_btnfnc;
}
