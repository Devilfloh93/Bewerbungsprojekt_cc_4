#include "Btn.h"

Btn::Btn(const MenuState menuState, const BtnFunc btnfnc, unique_ptr<sf::Text> text, unique_ptr<sf::Sprite> sprite)
    : Gui(menuState), Text(move(text)), Sprite(move(sprite)), m_btnfnc(btnfnc)
{
}

BtnFunc Btn::GetBtnFnc() const
{
    return m_btnfnc;
}
