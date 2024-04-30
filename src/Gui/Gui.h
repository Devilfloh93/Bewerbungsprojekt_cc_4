#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

enum class BtnFunc
{
    Nothing = 0,
    Play,
    Quit,
    Options,
    Back,
    Resume,
    Resolution,
    Fullscreen,
    Save,
    OpenLoad,
    Create,
    Load,
    MainMenu,
    Language,
    ChangeLanguageEN,
    ChangeLanguageDE
};

enum class MenuState
{
    Playing = 0,
    Main,
    Pause,
    Options,
    Inventory,
    OpenLoad,
    Save,
    Create,
    Load,
    Language,
    Trader
};

class Gui
{
public:
    Gui(const MenuState menuState);
    ~Gui() = default;

    MenuState GetMenuState() const;

    void SetMenuState(const MenuState menuState);

protected:
    MenuState m_menuState;
};
