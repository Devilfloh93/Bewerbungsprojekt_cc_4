#pragma once
#include "Game.h"
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
    Fullscreen
};

enum class MenuState;
class Title
{

public:
    Title(const MenuState menuState, sf::Text *text);
    ~Title() = default;

    sf::Text *GetText() const;

    MenuState GetMenuState() const;

private:
    MenuState m_menuState;
    sf::Text *m_text;
};

class Button
{
public:
    Button(const MenuState menuState, const BtnFunc btnfnc, sf::Text *text, sf::Sprite *sprite);
    ~Button() = default;

    sf::Text *GetText() const;
    sf::Sprite *GetSprite() const;

    MenuState GetMenuState() const;
    BtnFunc GetBtnFnc() const;

private:
    MenuState m_menuState;
    sf::Text *m_text;
    sf::Sprite *m_sprite;
    BtnFunc m_btnfnc;
};
