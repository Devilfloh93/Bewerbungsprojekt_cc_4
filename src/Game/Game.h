#pragma once
#include "AllTextures.h"
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

class Game : public Gui
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
    uint16_t GetWindowWidth() const;
    void SetWindowHeight(uint16_t width);
    void SetWindowWidth(uint16_t height);
    sf::RenderWindow *GetWindow();

    // ZOOM
    void SetZoom(const uint8_t zoom);
    void SetZoom(const uint8_t zoom, const float zoomLevel);
    void UpdateZoom(const float delta);
    uint8_t GetZoom() const;
    uint8_t GetMaxZoom() const;

    // VECTOR
    vector<ItemCfg *> GetItemCfg() const;
    vector<Anim *> GetAnim() const;
    vector<AllTextures *> GetTexture() const;
    vector<Stats *> GetStats() const;
    vector<Input *> GetInput() const;
    vector<sf::Text *> GetSaveFiles() const;
    vector<Btn *> GetBtn() const;
    vector<Title *> GetTitles() const;
    vector<Font *> GetFont() const;

    vector<ItemGround *> GetItem() const;
    vector<World *> GetWorld() const;
    vector<Creature *> GetCreature() const;

    // VIEW
    sf::View *GetView();
    void UpdateView(const sf::Vector2f &size);
    void HandleViewPosition();

    // PLAYER
    void SetPlayer(Player *player);
    Player *GetPlayer() const;
    bool CreatePlayer();
    bool LoadPlayer(const uint8_t id);

    // LOAD SAVE FILE
    void CreateLoadMenu();
    void SetSaveGameID(const uint8_t id);
    uint8_t GetSaveGameID() const;
    void Saving(const bool destroy);
    void SaveCreatures(const bool destroy);
    void SaveWorld(const bool destroy);
    void SaveGroundItems(const bool destroy);
    void LoadGroundItems();

    // STATS
    StatDecay GetStatDecay() const;
    void ResizeStats();
    void RenderStats();

    // THREAD
    Thread *GetThread();

    // ITEMS
    void SetItems(ItemGround *item);
    void RemoveItems(const size_t i);

    // INITS
    void Init();
    void InitFolder();
    void InitSettings();
    void InitGeneral();
    void InitHotkeys();

    void InitViews();
    void InitTexture();
    void InitFont();
    void InitAnim();

    void InitItemCfg();
    void InitRenderStats();
    void InitMenu();
    void InitSurface();
    void InitWorld();
    void InitPlayer();
    void InitCreature();
    void InitWindow();

    // Render
    void Render(sf::Clock &clock);
    void RenderSurface();
    void RenderWorld();
    void RenderMenu();
    void RenderItems();
    void RenderCreature();
    float GetRenderPuffer() const;

    // RESIZE
    void ResizeWindow();
    void ResizeMenu();

    // FOLDER
    void CreateSaveFolder(const uint8_t id);
    uint8_t CountSaveFolders();

    // MENU
    void ResetInputToDefault();
    void SetMenuState(MenuState state);

    // SETTINGS
    map<string, uint8_t> GetHotkeys() const;
    void ChangeLanguage(const string language);

    // UNIQUE PTR
    void SetDialogSprite(unique_ptr<sf::Sprite> sprite);
    void SetDialogText(unique_ptr<sf::Text> text);
    void ClearDialog();
    void RenderDialog();

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
    vector<Creature *> m_creature;
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

    vector<unique_ptr<sf::Sprite>> m_dialogSprites;
    vector<unique_ptr<sf::Text>> m_dialogTexts;
    // SETTINGS
    map<string, uint8_t> m_hotkeys;
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
    // THREAD
    Thread *m_thread;
    // RENDER
    float m_renderPuffer;
    // WINDOW
    sf::RenderWindow *m_window;
};
