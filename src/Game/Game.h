#pragma once
#include "Font.h"
#include "Gui.h"
#include "Item.h"
#include "Player.h"
#include "Surface.h"
#include "Texture.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>

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

class Player;
class Title;
class Button;
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
    sf::View GetView() const;
    sf::View GetMenuView() const;

    std::vector<World> GetWorld() const;
    std::vector<ItemCfg> GetItemCfg() const;
    std::vector<Item> GetItem() const;

    void SetPlaying(const bool playing);
    void SetMenuState(const MenuState menuState);
    void SetZoom(const std::uint8_t zoom);
    void SetZoom(const std::uint8_t zoom, const float zoomLevel);
    void SetWindowHeight(const std::uint16_t height);
    void SetWindowWidth(const std::uint16_t width);

    void InitViews();
    void UpdateView();
    void HandleViewPosition(const sf::RenderWindow &window);

    void InitItemCfg();
    void DrawItems(sf::RenderWindow &window);

    void InitTexture();

    void InitFont();

    Player InitPlayer();

    void InitMenu();

    void InitSurface();

    void InitWorld();

    bool HandleBtnClicked(sf::RenderWindow &window);

    void DrawSurface(sf::RenderWindow &window, Player &player);

    void DrawWorld(sf::RenderWindow &window);

    void DrawMenu(sf::RenderWindow &window);
    void DrawMenu(sf::RenderWindow &window, Player &player);

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
    std::vector<ItemCfg> m_itemCfg;
    std::vector<World> m_world;
    std::vector<Surface> m_surfaces;
    std::vector<Title> m_titles;
    std::vector<Button> m_buttons;
    std::vector<Item> m_items;
    std::vector<Texture> m_textures;
    std::vector<Font> m_fonts;
    sf::View m_view;
    sf::View m_menuView;
    sf::Vector2f m_defaultCenter;
};
