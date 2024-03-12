#pragma once
#include "Game.h"
#include "nlohmann/json.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <memory>
#include <vector>

using json = nlohmann::json;


enum class BtnFunc
{
    Nothing = 0,
    Play,
    Quit,
    Options,
    Back,
    Resume
};

class Title
{

public:
    Title(const MenuState menuState, const sf::Text text);
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
    Button(const std::vector<MenuState> menuState, const BtnFunc btnfnc, const sf::Text text, const sf::Sprite sprite);
    ~Button() = default;

    sf::Text GetText() const;
    sf::Sprite GetSprite() const;

    std::vector<MenuState> GetMenuState() const;
    BtnFunc GetBtnFnc() const;

private:
    std::vector<MenuState> m_menuState;
    sf::Text m_text;
    sf::Sprite m_sprite;
    BtnFunc m_btnfnc;
};


void SetTitlePos(const std::uint32_t width, sf::Text &text);

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Text &title, sf::Text &btntext);

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Sprite &btn, sf::Text &btntext);

void InitMenus(const Game &game,
               const sf::Font &font,
               const sf::Texture &texture,
               std::vector<std::unique_ptr<Title>> &titles,
               std::vector<std::unique_ptr<Button>> &buttons);

void DrawMenu(sf::RenderWindow &window,
              sf::View &view,
              std::vector<std::unique_ptr<Title>> &titles,
              std::vector<std::unique_ptr<Button>> &buttons,
              MenuState menuState);

void ProcessJSON(const json &j, std::vector<nlohmann::json_abi_v3_11_2::ordered_json> &vec);

bool HandleMenuBtnClicked(sf::RenderWindow &window,
                       std::vector<std::unique_ptr<Button>> &buttons,
                       MenuState state,
                       Game &game);
