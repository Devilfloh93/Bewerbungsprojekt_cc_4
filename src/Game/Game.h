#pragma once
#include "Anim.h"
#include "Btn.h"
#include "Creature.h"
#include "Font.h"
#include "Gui.h"
#include "Input.h"
#include "ItemCfg.h"
#include "ItemGround.h"
#include "Message.h"
#include "MessageFormat.h"
#include "Player.h"
#include "Stats.h"
#include "Surface.h"
#include "Thread.h"
#include "Title.h"
#include "Trader.h"
#include "Utilities.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <cstdint>

struct StatDecay
{
    float food;
    float water;
};

struct GameSize
{
    uint16_t width;
    uint16_t height;
};

struct WindowSize
{
    uint16_t width;
    uint16_t height;
};

struct SurfaceSize
{
    uint8_t tileSize;
    uint32_t maxTiles;
};

struct Zoom
{
    uint8_t curZoom;
    uint8_t maxZoom;
};

class Thread;

class Game final
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
    SurfaceSize GetSurfaceSize() const;
    GameSize GetGameSize() const;

    // WINDOW
    vector<World *> GetWorld() const;
    WindowSize GetWindowSize() const;
    sf::RenderWindow *GetWindow();
    void InitWindow();

    // ZOOM
    void SetZoom(const uint8_t zoom);
    void SetZoom(const uint8_t zoom, const float zoomLevel);
    void UpdateZoom(const float delta);
    Zoom GetZoom() const;
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
    void CreateLoadMenu();
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
    void CollectItem();

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

    // MESSAGE
    void RenderMessage();
    void AddMessage(const string &message, const MessageType type);
    void InitMessageFormat();
    const vector<unique_ptr<MessageFormat>> *GetMessageFormat() const;

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
    void InitMenu();
    void RenderMenu();

    // SETTINGS
    map<uint8_t, uint16_t> GetHotkeys() const;
    const vector<unique_ptr<SelectableText>> *GetHotkeyMenu() const;
    void ChangeLanguage(const string language);
    void InitSettings();
    void SaveGeneral();
    void LoadGeneral();
    void LoadHotkeys();
    void InitHotkeys();

    // DIALOG
    void SetDialogSprite(unique_ptr<SelectableSprite> sprite);
    void SetDialogText(unique_ptr<SelectableText> text);
    void ClearDialog();
    sf::Text *RenderSelectableTextDialog(const vector<unique_ptr<SelectableText>> *selectableTexts);
    void RenderSelectableSpriteDialog();
    const vector<unique_ptr<SelectableText>> *GetDialogText() const;
    uint8_t GetDialogSelectedID(const SelectedTextCategorie selectedCategorie) const;
    void UpdateDialog(const SelectedTextCategorie selectedCategorie, const string &text);
    void UpdateDialog(const SelectedTextCategorie selectedCategorie);

    void SetSelectedTextID(const uint8_t ID);
    uint8_t GetSelectedTextID() const;

    pair<MenuState, bool> GetMenuState() const;
    void SetMenuState(const MenuState menuState, const bool rePositioning);
    void SetMenuState();
    void ResetMenuState();

    // INPUT
    vector<Input *> GetInput() const;
    Input *GetSelectedInput();
    void SetSelectedInput(Input *selectedInput);
    void ResetInputToDefault();
    string GetInputString() const;

    void MoveCreature();
    void HandleCreatureMove(sf::Clock &clock);
    void ExecuteMove(Unit *unit, sf::Clock &clock);

private:
    // RUNNING
    bool m_playing;
    // AREA
    SurfaceSize m_surfaceSize;
    GameSize m_gameSize;
    // WINDOW
    WindowSize m_windowSize;
    sf::RenderWindow *m_window;
    uint8_t m_windowStyle;
    // ZOOM
    Zoom m_zoom;
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

    vector<unique_ptr<Message>> m_messages;
    vector<unique_ptr<MessageFormat>> m_messageFormat;

    vector<unique_ptr<SelectableText>> m_hotkeyMenu;

    vector<unique_ptr<SelectableSprite>> m_dialogSprites;
    vector<unique_ptr<SelectableText>> m_dialogTexts;
    uint8_t m_selectedTextID;
    // SETTINGS
    map<uint8_t, uint16_t> m_hotkeys;
    string m_language;
    // VIEW
    sf::View *m_view;
    sf::View *m_menuView;
    // PLAYER
    uint8_t m_defaultAnimID;
    Player *m_player;
    // STATS
    StatDecay m_statDecay;
    vector<Stats *> m_stats;
    // THREAD
    Thread *m_thread;
    // RENDER
    float m_renderPuffer;
    sf::Text *m_hotkeyRender;

    pair<MenuState, bool> m_menuState;
    vector<pair<MenuState, bool>> m_lastMenuState;
    // INPUT
    Input *m_selectedInput;
};
