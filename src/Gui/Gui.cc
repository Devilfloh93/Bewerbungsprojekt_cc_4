#include "Gui.h"

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
