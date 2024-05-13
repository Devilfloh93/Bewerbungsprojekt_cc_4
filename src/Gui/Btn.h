#pragma once
#include "Gui.h"
#include "Sprite.h"
#include "Text.h"

using namespace std;

class Btn : public Gui, public Text, public Sprite
{
public:
    Btn(const MenuState menuState,
        const BtnFunc btnfnc,
        unique_ptr<sf::Text> text,
        unique_ptr<sf::Sprite> sprite,
        const Alignment alignment);
    ~Btn() = default;

    BtnFunc GetBtnFnc() const;
    Alignment GetAlignment() const;

private:
    BtnFunc m_btnfnc;
    Alignment m_alignment;
};
