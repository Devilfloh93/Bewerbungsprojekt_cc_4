#include "Title.h"


Title::Title(const MenuState menuState, unique_ptr<sf::Text> text, const Alignment alignment)
    : Gui(menuState, alignment), Text(move(text))
{
}
