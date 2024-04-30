#pragma once
#include "Gui.h"
#include "Text.h"

using namespace std;

class Title : public Gui, public Text
{
public:
    Title(const MenuState menuState, sf::Text *text);
    ~Title() = default;
};
