#pragma once
#include "Gui.h"
#include "Sprite.h"
#include "Text.h"

using namespace std;

class Btn : public Gui, public Text, public Sprite
{
public:
    Btn(const MenuState menuState, const BtnFunc btnfnc, sf::Text *text, sf::Sprite *sprite);
    ~Btn() = default;

    BtnFunc GetBtnFnc() const;

private:
    BtnFunc m_btnfnc;
};
