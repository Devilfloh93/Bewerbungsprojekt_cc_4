#include "Btn.h"

/**
 * @brief Construct a new Btn:: Btn object
 *
 * @param menuState in which menuState should this BTN be displayed menuState is found in Game.cc too
 * @param btnfnc used in a switch statement to find the correct funtion to use loaded from data/menu/btn.json
 * @param text
 * @param sprite
 */

Btn::Btn(const MenuState menuState, const BtnFunc btnfnc, unique_ptr<sf::Text> text, unique_ptr<sf::Sprite> sprite)
    : Gui(menuState), Text(move(text)), Sprite(move(sprite)), m_btnfnc(btnfnc)
{
}

BtnFunc Btn::GetBtnFnc() const
{
    return m_btnfnc;
}
