#pragma once
#include "Anim.h"
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

struct StatDecay
{
    float food;
    float water;
};

struct ReturnView
{
    sf::Vector2f viewCenter;
    sf::Vector2f viewSize;
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
    Game(const uint16_t windowWidth, const uint16_t windowHeight);
    ~Game() = default;

    bool GetPlaying() const;
    MenuState GetMenuState() const;
    uint8_t GetTileSize() const;
    uint16_t GetGameWidth() const;
    uint16_t GetGameHeight() const;
    sf::View GetView() const;

    vector<World *> GetWorld() const;
    vector<ItemCfg *> GetItemCfg() const;
    vector<Item *> GetItem() const;
    vector<Anim *> GetAnim() const;

    StatDecay GetStatDecay() const;

    void SetItems(Item *item);
    void RemoveItems(const size_t i);

    void SetMenuState(const MenuState menuState);
    void SetZoom(const uint8_t zoom);
    void SetZoom(const uint8_t zoom, const float zoomLevel);

    void InitAnim();

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

    void UpdateZoom(const float delta);

    bool HandleBtnClicked(sf::RenderWindow &window);

    void DrawSurface(sf::RenderWindow &window, Player &player);

    void DrawWorld(sf::RenderWindow &window);

    void DrawMenu(sf::RenderWindow &window);
    void DrawMenu(sf::RenderWindow &window, Player &player);

    void ResizeWindow(sf::RenderWindow &window);
    void ResizeMenu();

    void Quit(sf::RenderWindow &window);

private:
    bool m_playing;
    MenuState m_menuState;
    uint8_t m_maxZoom;
    uint8_t m_zoom;
    uint16_t m_windowWidth;
    uint16_t m_windowHeight;
    uint16_t m_gameWidth;
    uint16_t m_gameHeight;
    uint8_t m_tileSize;
    uint32_t m_maxTiles;
    vector<ItemCfg *> m_itemCfg;
    vector<World *> m_world;
    vector<Surface *> m_surfaces;
    vector<Title *> m_titles;
    vector<Button *> m_buttons;
    vector<Item *> m_items;
    vector<Texture *> m_textures;
    vector<Font *> m_fonts;
    vector<Anim *> m_anim;
    sf::View m_view;
    sf::View m_menuView;
    sf::Vector2f m_defaultCenter;
    uint8_t m_defaultPlayerTextureID;
    StatDecay m_statDecay;
};
