#include "Btn.h"

Btn::Btn(const MenuState menuState, const BtnFunc btnfnc, sf::Text *text, sf::Sprite *sprite)
    : Gui(menuState), Text(text), Sprite(sprite), m_btnfnc(btnfnc)
{
}

BtnFunc Btn::GetBtnFnc() const
{
    return m_btnfnc;
}
