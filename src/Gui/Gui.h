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
    Resume
};

enum class MenuState;
class Title
{

public:
    Title(const MenuState menuState, const sf::Text &text);
    ~Title() = default;

    sf::Text GetText() const;

    MenuState GetMenuState() const;

private:
    MenuState m_menuState;
    sf::Text m_text;
};

class Button
{
public:
    Button(const vector<MenuState> menuState, const BtnFunc btnfnc, const sf::Text text, const sf::Sprite sprite);
    ~Button() = default;

    sf::Text GetText() const;
    sf::Sprite GetSprite() const;

    vector<MenuState> GetMenuState() const;
    BtnFunc GetBtnFnc() const;

private:
    vector<MenuState> m_menuState;
    sf::Text m_text;
    sf::Sprite m_sprite;
    BtnFunc m_btnfnc;
};
