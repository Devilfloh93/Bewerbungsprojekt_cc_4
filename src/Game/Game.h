#pragma once
#include "Anim.h"
#include "Btn.h"
#include "Creature.h"
#include "Font.h"
#include "Gui.h"
#include "Input.h"
#include "ItemCfg.h"
#include "ItemGround.h"
#include "Player.h"
#include "Stats.h"
#include "Surface.h"
#include "Thread.h"
#include "Title.h"
#include "Utilities.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <cstdint>

struct StatDecay
{
    float food;
    float water;
};

class Thread;

class Game
{
public:
    Game();
    ~Game() = default;

    // RUNNING
    bool GetPlaying() const;
    void SetPlaying(const bool playing);

    // UNLOAD
    void Unload();

    // AREA
    uint8_t GetTileSize() const;
    uint16_t GetGameWidth() const;
    uint16_t GetGameHeight() const;

    // WINDOW
    vector<World *> GetWorld() const;
    uint16_t GetWindowWidth() const;
    void SetWindowHeight(uint16_t width);
    void SetWindowWidth(uint16_t height);
    sf::RenderWindow *GetWindow();
    void InitWindow();

    // ZOOM
    void SetZoom(const uint8_t zoom);
    void SetZoom(const uint8_t zoom, const float zoomLevel);
    void UpdateZoom(const float delta);
    uint8_t GetZoom() const;
    uint8_t GetMaxZoom() const;
    void InitZoom();

    // ANIM
    vector<Anim *> GetAnim() const;
    void InitAnim();

    // TEXUTRE
    vector<Texture *> GetTexture() const;
    void InitTexture();

    // FONT
    vector<Font *> GetFont() const;
    void InitFont();

    // CREATURE
    vector<Creature *> GetCreature() const;
    void InitCreature();

    // VIEW
    sf::View *GetView();
    void UpdateView();
    void HandleViewPosition();
    void InitViews();

    // PLAYER
    void SetPlayer(Player *player);
    Player *GetPlayer() const;
    bool CreatePlayer();
    bool LoadPlayer(const uint8_t id);
    void InitPlayer();

    // LOAD SAVE FILE
    vector<sf::Text *> GetSaveFiles() const;
    void CreateLoadMenu();
    void SetSaveGameID(const uint8_t id);
    uint8_t GetSaveGameID() const;
    void Saving(const bool destroy);
    void SaveCreatures(const bool destroy);
    void SaveWorld(const bool destroy);
    void SaveGroundItems(const bool destroy);
    void LoadGroundItems();

    // STATS
    void InitRenderStats();
    vector<Stats *> GetStats() const;
    StatDecay GetStatDecay() const;
    void ResizeStats();
    void RenderStats();

    // THREAD
    Thread *GetThread();

    // ITEMS
    vector<ItemGround *> GetItem() const;
    vector<ItemCfg *> GetItemCfg() const;
    void InitItemCfg();
    void RenderItems();
    void SetItems(ItemGround *item);
    void RemoveItems(const size_t i);
    void CreateGroundItem(const uint8_t textureID,
                          const sf::IntRect &textureData,
                          const float posX,
                          const float posY,
                          const uint8_t id,
                          const uint16_t count);

    // INITS
    void Init();
    void InitFolder();

    void InitSurface();
    void InitWorld();

    // Render
    void Render(sf::Clock &clock);
    void RenderSurface();
    void RenderWorld();
    void RenderCreature();
    float GetRenderPuffer() const;
    void RenderHotkey();

    // RESIZE
    void ResizeWindow(const uint16_t width, const uint16_t height);
    void ResizeMenu();
    void FullscreenWindow();
    void SetWindowProperties();

    // FOLDER
    void CreateSaveFolder(const uint8_t id);
    uint8_t CountSaveFolders();

    // MENU
    vector<Title *> GetTitles() const;
    vector<Btn *> GetBtn() const;
    vector<Input *> GetInput() const;
    void InitMenu();
    void RenderMenu();
    void ResetInputToDefault();

    // SETTINGS
    map<uint8_t, uint16_t> GetHotkeys() const;
    void ChangeLanguage(const string language);
    void InitSettings();
    void SaveGeneral();
    void LoadGeneral();
    void LoadHotkeys();
    void InitHotkeys();

    // DIALOG
    void SetDialogSprite(unique_ptr<sf::Sprite> sprite);
    void SetDialogText(unique_ptr<sf::Text> text);
    void ClearDialog();
    sf::Text *RenderDialog();

    MenuState GetMenuState() const;
    void SetMenuState(const MenuState menuState);

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
    sf::RenderWindow *m_window;
    uint8_t m_windowStyle;
    // ZOOM
    uint8_t m_maxZoom;
    uint8_t m_zoom;
    // VECTOR
    vector<ItemCfg *> m_itemCfg;
    vector<World *> m_world;
    vector<Creature *> m_creature;
    vector<Surface *> m_surfaces;
    vector<Title *> m_titles;
    vector<Btn *> m_btns;
    vector<ItemGround *> m_items;
    vector<Texture *> m_textures;
    vector<Font *> m_fonts;
    vector<Anim *> m_anim;
    vector<Input *> m_inputs;
    vector<sf::Text *> m_saveFiles;
    vector<sf::Text *> m_hotkeyMenu; // TODO: TEMPORARY FIX

    vector<unique_ptr<sf::Sprite>> m_dialogSprites;
    vector<unique_ptr<sf::Text>> m_dialogTexts;
    // SETTINGS
    map<uint8_t, uint16_t> m_hotkeys;
    string m_language;
    // VIEW
    sf::View *m_view;
    sf::View *m_menuView;
    // PLAYER
    uint8_t m_defaultAnimID;
    Player *m_player;
    uint8_t m_saveGameID;
    // STATS
    StatDecay m_statDecay;
    vector<Stats *> m_stats;
    // THREAD
    Thread *m_thread;
    // RENDER
    float m_renderPuffer;
    sf::Text *m_hotkeyRender;
    MenuState m_menuState;
};
