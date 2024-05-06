#include "Title.h"


Title::Title(const MenuState menuState, unique_ptr<sf::Text> text) : Gui(menuState), Text(move(text))
{
}
