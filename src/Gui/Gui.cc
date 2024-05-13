#include "Gui.h"

Gui::Gui(const MenuState menuState, const Alignment alignment) : m_menuState(menuState), m_alignment(alignment)
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

Alignment Gui::GetAlignment() const
{
    return m_alignment;
}
