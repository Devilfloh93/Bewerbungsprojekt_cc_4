#pragma once
#include "Anim.h"
#include "EventHandler.h"
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
class Btn;
class Input;
class Thread;
class EventHandler;

class Game : public Gui
{
public:
    Game(const uint16_t windowWidth, const uint16_t windowHeight);
    ~Game() = default;

    void SetMenuState(MenuState state);

    // RUNNING
    bool GetPlaying() const;

    // AREA
    uint8_t GetTileSize() const;
    uint16_t GetGameWidth() const;
    uint16_t GetGameHeight() const;

    // WINDOW
    uint16_t GetWindowWidth() const;
    void SetWindowHeight(uint16_t width);
    void SetWindowWidth(uint16_t height);

    // ZOOM
    void SetZoom(const uint8_t zoom);
    void SetZoom(const uint8_t zoom, const float zoomLevel);
    void UpdateZoom(const float delta);

    // VECTOR
    vector<World *> GetWorld() const;
    vector<ItemCfg *> GetItemCfg() const;
    vector<ItemGround *> GetItem() const;
    vector<Anim *> GetAnim() const;
    vector<AllTextures *> GetTexture() const;
    vector<Stats *> GetStats() const;
    vector<Input *> GetInput() const;
    vector<sf::Text *> GetSaveFiles() const;
    vector<Btn *> GetBtn() const;

    // VIEW
    sf::View GetView() const;
    void UpdateView();
    void HandleViewPosition(const sf::RenderWindow &window);

    // PLAYER
    void SetPlayer(Player *player);
    Player *GetPlayer() const;
    bool CreatePlayer();
    bool LoadPlayer(const uint8_t id);

    // LOAD SAVE FILE
    void CreateLoadMenu();
    void SetSaveGameID(const uint8_t id);
    uint8_t GetSaveGameID() const;

    // STATS
    StatDecay GetStatDecay() const;

    // THREAD
    Thread *GetThread();

    // ITEMS
    void SetItems(ItemGround *item);
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
    void InitPlayer(sf::RenderWindow &window);

    // DRAW
    void DrawSurface(sf::RenderWindow &window, Player *player);
    void DrawWorld(sf::RenderWindow &window);
    void DrawMenu(sf::RenderWindow &window);
    void DrawItems(sf::RenderWindow &window);

    // RESIZE
    void ResizeWindow(sf::RenderWindow &window);
    void ResizeMenu();

    // FOLDER
    void CreateFolder();
    void CreateSaveFolder(const uint8_t id);
    uint8_t CountSaveFolders();

    // MENU
    void ResetInputToDefault();

    // DRAW
    float GetDrawPuffer() const;

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
    // VECTOR
    vector<ItemCfg *> m_itemCfg;
    vector<World *> m_world;
    vector<Surface *> m_surfaces;
    vector<Title *> m_titles;
    vector<Btn *> m_btns;
    vector<ItemGround *> m_items;
    vector<AllTextures *> m_textures;
    vector<Font *> m_fonts;
    vector<Anim *> m_anim;
    vector<Stats *> m_stats;
    vector<Input *> m_inputs;
    vector<sf::Text *> m_saveFiles;
    // VIEW
    sf::View m_view;
    sf::View m_menuView;
    sf::Vector2f m_defaultCenter;
    // PLAYER
    uint8_t m_defaultPlayerTextureID;
    Player *m_player;
    uint8_t m_saveGameID;
    // STATS
    StatDecay m_statDecay;
    // THREAD
    Thread *m_thread;
    // DRAW
    float m_drawPuffer;
};
