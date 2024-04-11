#pragma once
#include "Sprite.h"
#include "Text.h"
#include <SFML/Graphics.hpp>
#include <vector>

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
    Load
};

enum class MenuState;

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

class Title : public Gui, public Text
{
public:
    Title(const MenuState menuState, sf::Text *text);
    ~Title() = default;
};

class Button : public Gui, public Text, public Sprite
{
public:
    Button(const MenuState menuState, const BtnFunc btnfnc, sf::Text *text, sf::Sprite *sprite);
    ~Button() = default;

    BtnFunc GetBtnFnc() const;

private:
    BtnFunc m_btnfnc;
};

class Input : public Gui, public Text
{
public:
    Input(const MenuState menuState, sf::Text *text, const uint8_t maxChars);
    ~Input() = default;

    string_view GetName() const;

    void Write(const uint16_t width, const sf::Uint32 character);
    void Popback(const uint16_t width);

private:
    string m_string;
    uint8_t m_maxChars;
};
