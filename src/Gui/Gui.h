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
    ChangeLanguageDE,
    OpenResolution,
    Resolution1920x1080,
    Resolution1280x720,
    OpenHotkeys,
    Buy,
    Sell
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

enum class SelectedTextCategorie
{
    Nothing = 0,
    Buy,
    Sell
};

class Gui
{
public:
    Gui(const MenuState menuState, const Alignment alignment);
    ~Gui() = default;

    MenuState GetMenuState() const;

    void SetMenuState(const MenuState menuState);
    Alignment GetAlignment() const;

protected:
    MenuState m_menuState;
    Alignment m_alignment;
};
