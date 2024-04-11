#include "Gui.h"
#include "Sprite.h"
#include "Text.h"

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
