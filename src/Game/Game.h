#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

struct ReturnView
{
    sf::Vector2f viewCenter;
    sf::Vector2f viewSize;
};

struct MovementTexture
{
    sf::IntRect notMoving;
    sf::IntRect up01;
    sf::IntRect up02;
    sf::IntRect down01;
    sf::IntRect down02;
    sf::IntRect left01;
    sf::IntRect left02;
    sf::IntRect right01;
    sf::IntRect right02;
};

enum class MenuState
{
    Playing = 0,
    Main,
    Pause,
    Options,
    Inventory
};

class Game
{

public:
    Game(const std::uint16_t windowWidth, const std::uint16_t windowHeight);
    ~Game() = default;

    bool GetPlaying() const;
    MenuState GetMenuState() const;
    std::uint8_t GetMaxZoom() const;
    std::uint8_t GetZoom() const;
    std::uint16_t GetWindowWidth() const;
    std::uint16_t GetWindowHeight() const;
    std::uint32_t GetMaxTiles() const;
    std::uint8_t GetTileSize() const;
    std::uint16_t GetGameWidth() const;
    std::uint16_t GetGameHeight() const;

    void SetPlaying(const bool playing);
    void SetMenuState(const MenuState menuState);
    void SetZoom(const std::uint8_t zoom);
    void SetWindowHeight(const std::uint16_t height);
    void SetWindowWidth(const std::uint16_t width);

private:
    bool m_playing;
    MenuState m_menuState;
    std::uint8_t m_maxZoom;
    std::uint8_t m_zoom;
    std::uint16_t m_windowWidth;
    std::uint16_t m_windowHeight;
    std::uint16_t m_gameWidth;
    std::uint16_t m_gameHeight;
    std::uint8_t m_tileSize;
    std::uint32_t m_maxTiles;
};

ReturnView InitView(Game &game, sf::View &view);

void UpdateView(const Game &game, sf::View &view);
