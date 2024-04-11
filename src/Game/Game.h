#pragma once
#include "Anim.h"
#include "Font.h"
#include "Gui.h"
#include "Item.h"
#include "Player.h"
#include "Stats.h"
#include "Surface.h"
#include "Texture.h"
#include "Thread.h"
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
    Inventory,
    OpenLoad,
    Save,
    Create,
    Load
};


class Player;
class Title;
class Button;
class Input;
class Thread;

class Game : public Gui
{
public:
    Game(const uint16_t windowWidth, const uint16_t windowHeight);
    ~Game() = default;

    // RUNNING
    bool GetPlaying() const;

    // AREA
    uint8_t GetTileSize() const;
    uint16_t GetGameWidth() const;
    uint16_t GetGameHeight() const;

    // WINDOW
    uint16_t GetWindowZoomHeight() const;
    uint16_t GetWindowZoomWidth() const;
    uint16_t GetWindowWidth() const;

    // ZOOM
    void SetZoom(const uint8_t zoom);
    void SetZoom(const uint8_t zoom, const float zoomLevel);
    void UpdateZoom(const float delta);

    // VECTOR
    vector<World *> GetWorld() const;
    vector<ItemCfg *> GetItemCfg() const;
    vector<Item *> GetItem() const;
    vector<Anim *> GetAnim() const;
    vector<AllTextures *> GetTexture() const;
    vector<Stats *> GetStats() const;
    vector<Input *> GetInput() const;

    // VIEW
    sf::View GetView() const;
    void UpdateView();
    void HandleViewPosition(const sf::RenderWindow &window);

    // PLAYER
    Player *GetPlayer() const;
    bool CreatePlayer();
    bool LoadPlayer(const uint8_t id);

    // STATS
    StatDecay GetStatDecay() const;

    // THREAD
    Thread *GetThread() const;

    // ITEMS
    void SetItems(Item *item);
    void RemoveItems(const size_t i);

    // INITS
    void InitItemCfg();
    void InitTexture();
    void InitFont();
    void InitDrawStats();
    void InitMenu();
    void InitSurface();
    void InitWorld();
    void InitAnim();
    void InitViews();

    // DRAW
    void DrawSurface(sf::RenderWindow &window, Player *player);
    void DrawWorld(sf::RenderWindow &window);
    void DrawMenu(sf::RenderWindow &window);
    void DrawItems(sf::RenderWindow &window);

    // GUI
    bool HandleBtnClicked(sf::RenderWindow &window, Game &game);

    // RESIZE
    void ResizeWindow(sf::RenderWindow &window);
    void ResizeMenu();

    // FOLDER
    void CreateFolder();
    void CreateFolder(const uint8_t id);
    uint8_t CountFolders();

    // QUIT
    void Quit(sf::RenderWindow &window);

private:
    // RUNNING
    bool m_playing;
    // AREA
    uint8_t m_tileSize;
    uint32_t m_maxTiles;
    uint16_t m_gameWidth;
    uint16_t m_gameHeight;
    // WINDOW
    uint16_t m_windowWidth;
    uint16_t m_windowHeight;
    // ZOOM
    uint8_t m_maxZoom;
    uint8_t m_zoom;
    uint16_t m_windowZoomWidth;
    uint16_t m_windowZoomHeight;
    // VECTOR
    vector<ItemCfg *> m_itemCfg;
    vector<World *> m_world;
    vector<Surface *> m_surfaces;
    vector<Title *> m_titles;
    vector<Button *> m_buttons;
    vector<Item *> m_items;
    vector<AllTextures *> m_textures;
    vector<Font *> m_fonts;
    vector<Anim *> m_anim;
    vector<Stats *> m_stats;
    vector<Input *> m_inputs;
    // VIEW
    sf::View m_view;
    sf::View m_menuView;
    sf::Vector2f m_defaultCenter;
    // PLAYER
    uint8_t m_defaultPlayerTextureID;
    Player *m_player;
    // STATS
    StatDecay m_statDecay;
    // THREAD
    Thread *m_thread;
};
