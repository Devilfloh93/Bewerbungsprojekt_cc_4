#pragma once
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <memory>
#include <vector>

enum class BtnFunc
{
    Nothing = 0,
    Play,
    Quit,
    Options
};

struct GuiCfg
{
    GameState gameState;
};

struct TextCfg
{
    GuiCfg guiCfg;
    std::string text;
    std::uint8_t fontSize;
};

struct ButtonCfg
{
    TextCfg textCfg;
    BtnFunc btnFnc;
    sf::IntRect textureRect;
    sf::Vector2f scale;
};

class Text
{

public:
    Text(const GameState gameState, const BtnFunc btnfnc, const sf::Text text, const sf::Sprite sprite);
    ~Text() = default;

    sf::Text GetText() const;
    sf::Sprite GetSprite() const;

    GameState GetGameState() const;
    BtnFunc GetBtnFnc() const;

private:
    GameState m_gameState;
    sf::Text m_text;
    sf::Sprite m_sprite;
    BtnFunc m_btnfnc;
};

class Button : public Text
{

public:
    Button(const GameState gameState, const BtnFunc btnfnc, const sf::Text text, const sf::Sprite sprite);
    ~Button() = default;

    // sf::Sprite GetSprite() const;
    // BtnFunc GetBtnFnc() const;

private:
    // sf::Sprite m_sprite;
    // BtnFunc m_btnfnc;
};


void SetTitlePos(const std::uint32_t width, sf::Text &text);

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Text &title, sf::Text &btntext);

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Sprite &btn, sf::Text &btntext);

void InitMenus(const Game &game,
               const sf::Font &font,
               const sf::Texture &texture,
               std::vector<std::unique_ptr<Text>> &menus,
               const std::vector<TextCfg> &menuTitles,
               const std::vector<ButtonCfg> &menuButtons);

void DrawMenu(sf::RenderWindow &window, sf::View &view, std::vector<std::unique_ptr<Text>> &menus, GameState state);
