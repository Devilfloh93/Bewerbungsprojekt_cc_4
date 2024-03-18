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
    sf::IntRect up00; // Not Moving
    sf::IntRect up01; // Left Moving Feet
    sf::IntRect up02; // Right Moving Feet
    sf::IntRect down00;
    sf::IntRect down01;
    sf::IntRect down02;
    sf::IntRect left00;
    sf::IntRect left01;
    sf::IntRect right00;
    sf::IntRect right01;
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

    void InitView(sf::View &view);
    void UpdateView(sf::View &view);
    void HandleViewPosition(const sf::RenderWindow &window, sf::View &view);

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
