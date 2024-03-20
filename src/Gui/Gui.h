#pragma once
#include "Font.h"
#include "Game.h"
#include "Player.h"
#include "Texture.h"
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

void ProcessJSON(const json &j, std::vector<nlohmann::json_abi_v3_11_2::ordered_json> &vec);

namespace Menu
{
void SetTitlePos(const std::uint32_t width, sf::Text &text);

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Text &title, sf::Text &btntext);

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Sprite &btn, sf::Text &btntext);

void SetTextBeforeIcon(const std::uint16_t x, const std::uint16_t y, sf::Sprite &icon, sf::Text &text);

void SetTextBeforeIcon(sf::Sprite &icon, sf::Text &text, sf::Sprite &prevIcon);

void Init(const Game &game,
          std::vector<std::unique_ptr<Title>> &titles,
          std::vector<std::unique_ptr<Button>> &buttons,
          const std::vector<std::unique_ptr<Texture>> &textures,
          const std::vector<std::unique_ptr<Font>> &fonts);

void Draw(sf::RenderWindow &window,
          sf::View &view,
          const std::vector<std::unique_ptr<Title>> &titles,
          const std::vector<std::unique_ptr<Button>> &buttons,
          const MenuState menuState,
          Player &player,
          const std::vector<std::unique_ptr<ItemCfg>> &itemCfg);

void Draw(sf::RenderWindow &window,
          sf::View &view,
          const std::vector<std::unique_ptr<Title>> &titles,
          const std::vector<std::unique_ptr<Button>> &buttons,
          const MenuState state);

bool HandleBtnClicked(sf::RenderWindow &window,
                      const std::vector<std::unique_ptr<Button>> &buttons,
                      const MenuState state,
                      Game &game);


} // namespace Menu
