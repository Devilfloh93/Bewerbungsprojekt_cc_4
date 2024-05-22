#pragma once
#include "Gui.h"
#include "Text.h"

using namespace std;

class Title final : public Gui, public Text
{
public:
    Title(const MenuState menuState, unique_ptr<sf::Text> text, const Alignment alignment);
    ~Title() = default;
};
