#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

enum class Element
{
    Nothing = 0,
    Title,
    Input
};

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
    ChangeLanguageDE,
    OpenResolution,
    Resolution1920x1080,
    Resolution1280x720,
    OpenHotkeys
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
    Trader,
    Resolution,
    Hotkeys
};

enum class Hotkey
{
    Interact = 0,
    Escape,
    Inventory,
    LeftMove,
    RightMove,
    UpMove,
    DownMove
};

enum class Alignment
{
    Middle = 0,
    Left,
    Right
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
