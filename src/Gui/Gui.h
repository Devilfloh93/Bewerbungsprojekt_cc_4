#pragma once
#include "Game.h"
#include "Sprite.h"
#include "Text.h"
#include <SFML/Graphics.hpp>
#include <vector>

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
    Load,
    Create
};

enum class MenuState;

class Gui
{
public:
    Gui(const MenuState menuState);
    ~Gui() = default;

    MenuState GetMenuState() const;

private:
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
