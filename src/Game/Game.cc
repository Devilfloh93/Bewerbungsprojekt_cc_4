#include "Game.h"
#include "Collision.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <random>


using json = nlohmann::json;
using namespace std;

Game::Game() : m_menuState(MenuState::Main)
{
    m_renderPuffer = 200.0F;
    m_maxZoom = 3U;
    m_playing = false;
    m_defaultAnimID = 0;
    m_statDecay = {.food = 0.2F, .water = 0.5F};
    m_gameWidth = 8800U;
    m_gameHeight = 4800U;
    m_tileSize = 32U;
    m_maxTiles = ((m_gameWidth * m_gameHeight) / m_tileSize) / m_tileSize;
}

// BE CAREFUL WITH CHANGING INITIALIZING ORDER
void Game::Init()
{
    InitFolder();
    cout << "InitFolder Done!" << endl;

    InitSettings();
    cout << "InitSettings Done!" << endl;

    InitFont();
    cout << "InitFont Done!" << endl;

    InitTexture();
    cout << "InitTexture Done!" << endl;

    InitAnim();
    cout << "InitAnim Done!" << endl;

    InitItemCfg();
    cout << "InitItemCfg Done!" << endl;

    InitSurface();
    cout << "InitSurface Done!" << endl;

    InitRenderStats();
    cout << "InitRenderStats Done!" << endl;

    InitMenu();
    cout << "InitMenu Done!" << endl;

    InitWindow();
    cout << "InitWindow Done!" << endl;

    InitViews();
    cout << "InitViews Done!" << endl;

    InitHotkeys();
    cout << "InitHotkeys Done!" << endl;

    cout << "Game Init Done!" << endl;
}

// RUNNING
bool Game::GetPlaying() const
{
    return m_playing;
}

void Game::SetPlaying(const bool playing)
{
    m_playing = playing;
}

// UNLOAD
void Game::Unload()
{
    Saving(true);

    m_menuState = MenuState::Main;
    m_playing = false;

    m_thread->Join();
    delete m_thread;
    m_thread = nullptr;

    delete m_player;
    m_player = nullptr;
}

// AREA
uint8_t Game::GetTileSize() const
{
    return m_tileSize;
}

uint16_t Game::GetGameWidth() const
{
    return m_gameWidth;
}

uint16_t Game::GetGameHeight() const
{
    return m_gameHeight;
}

// WINDOW
uint16_t Game::GetWindowWidth() const
{
    return m_windowWidth;
}

void Game::SetWindowHeight(uint16_t height)
{
    m_windowHeight = height;
}

void Game::SetWindowWidth(uint16_t width)
{
    m_windowWidth = width;
}

sf::RenderWindow *Game::GetWindow()
{
    return m_window;
}

// ZOOM
void Game::SetZoom(const uint8_t zoom)
{
    m_zoom += zoom;
}

uint8_t Game::GetMaxZoom() const
{
    return m_maxZoom;
}

void Game::SetZoom(const uint8_t zoom, const float zoomLevel)
{
    m_view->zoom(zoomLevel);
    m_zoom += zoom;
}

void Game::UpdateZoom(const float delta)
{
    if (delta > 0U)
    {
        if (m_zoom < m_maxZoom)
            SetZoom(1U, 0.5F);
    }
    else
    {
        if (m_zoom > 0U)
            SetZoom(-1, 2.0F);
    }

    UpdateView();
}

uint8_t Game::GetZoom() const
{
    return m_zoom;
}

// VECTOR
vector<Texture *> Game::GetTexture() const
{
    return m_textures;
}

vector<World *> Game::GetWorld() const
{
    return m_world;
}

vector<Creature *> Game::GetCreature() const
{
    return m_creature;
}

vector<ItemCfg *> Game::GetItemCfg() const
{
    return m_itemCfg;
}

vector<ItemGround *> Game::GetItem() const
{
    return m_items;
}

vector<Stats *> Game::GetStats() const
{
    return m_stats;
}

vector<Anim *> Game::GetAnim() const
{
    return m_anim;
}

vector<Input *> Game::GetInput() const
{
    return m_inputs;
}

const vector<unique_ptr<SelectableText>> *Game::GetSaveFiles() const
{
    return &m_saveFiles;
}

vector<Btn *> Game::GetBtn() const
{
    return m_btns;
}

vector<Font *> Game::GetFont() const
{
    return m_fonts;
}

vector<Title *> Game::GetTitles() const
{
    return m_titles;
}

// VIEW
sf::View *Game::GetView()
{
    return m_view;
}

void Game::UpdateView()
{
    auto size = m_view->getSize();

    if (m_view->getCenter().x >= m_gameWidth - (size.x / 2U))
        m_view->setCenter({m_gameWidth - (size.x / 2U), m_view->getCenter().y});

    if (m_view->getCenter().x <= 0U + (size.x / 2U))
        m_view->setCenter({size.x / 2U, m_view->getCenter().y});

    if (m_view->getCenter().y <= 0U + (size.y / 2U))
        m_view->setCenter({m_view->getCenter().x, size.y / 2U});

    if (m_view->getCenter().y >= m_gameHeight - (size.y / 2U))
        m_view->setCenter({m_view->getCenter().x, m_gameHeight - (size.y / 2U)});

    ResizeStats();
}

void Game::HandleViewPosition()
{
    // get the current mouse position in the window
    sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
    // convert it to world coordinates
    sf::Vector2f worldPos = m_window->mapPixelToCoords(pixelPos);

    auto center = m_view->getCenter();
    auto size = m_view->getSize();

    auto moveX = worldPos.x - center.x;
    auto moveY = worldPos.y - center.y;

    // RIGHT
    if (moveX > 0U)
    {
        if (moveX + center.x >= m_gameWidth - (size.x / 2U))
            m_view->setCenter({m_gameWidth - (size.x / 2U), center.y});
        else
            m_view->move({moveX, 0U});
    }
    else // LEFT
    {
        if (center.x + (moveX) <= 0U + (size.x / 2U))
            m_view->setCenter({size.x / 2U, center.y});
        else
            m_view->move({moveX, 0U});
    }
    center = m_view->getCenter();

    // UP
    if (moveY < 0U)
    {
        if (moveY + center.y <= 0U + (size.y / 2U))
            m_view->setCenter({center.x, size.y / 2U});
        else
            m_view->move({0U, moveY});
    }
    else // DOWN
    {
        if (moveY + center.y >= m_gameHeight - (size.y / 2U))
            m_view->setCenter({center.x, m_gameHeight - (size.y / 2U)});
        else
            m_view->move({0U, moveY});
    }

    ResizeStats();
}

// PLAYER
Player *Game::GetPlayer() const
{
    return m_player;
}

void Game::SetPlayer(Player *player)
{
    m_player = player;
}

bool Game::LoadPlayer(const uint8_t id)
{
    Utilities utilities;
    auto sprite = make_unique<sf::Sprite>();

    auto animData = utilities.GetAnim(m_anim, m_defaultAnimID);
    auto texture = utilities.GetAnimTexture(m_anim, m_textures, m_defaultAnimID);

    utilities.SetSFSprite(sprite.get(), texture, animData.down.notMoving);

    m_player = new Player(move(sprite), m_defaultAnimID, id);

    return true;
}

bool Game::CreatePlayer()
{
    Utilities utilities;
    auto countFolders = CountSaveFolders();
    if (countFolders <= 0 || countFolders > 6)
        return false;

    string playerName = "";
    for (const auto &data : m_inputs)
    {
        if (m_menuState == data->GetMenuState())
            playerName = data->GetString();
    }

    if (playerName.size() == 0)
        return false;


    auto sprite = make_unique<sf::Sprite>();

    auto animData = utilities.GetAnim(m_anim, m_defaultAnimID);
    auto texture = utilities.GetAnimTexture(m_anim, m_textures, m_defaultAnimID);

    utilities.SetSFSprite(sprite.get(), texture, animData.down.notMoving);

    auto font = utilities.GetFont(m_fonts, 0);

    m_player = new Player(move(sprite), m_defaultAnimID, playerName, countFolders);
    return true;
}

// STATS
StatDecay Game::GetStatDecay() const
{
    return m_statDecay;
}

void Game::ResizeStats()
{
    auto width = (m_view->getCenter().x - (m_view->getSize().x / 2)) + 5.0F;

    size_t i = 1;
    size_t j = 1;

    for (const auto &data : m_stats)
    {
        auto type = data->GetType();
        auto sprite = data->GetSprite();
        auto height = (m_view->getCenter().y + (m_view->getSize().y / 2)) - 10.0F;

        if (type == StatType::Empty)
        {
            height -= i * 10;
            ++i;
        }
        else
        {
            height -= j * 10;
            ++j;
        }

        sprite->setPosition(sf::Vector2f{width, height});
    }
}

void Game::RenderStats()
{
    for (const auto &data : m_stats)
    {
        auto textureSize = data->GetTextureSize();
        auto type = data->GetType();
        auto sprite = data->GetSprite();
        auto textureRectSize = sprite->getTextureRect().getSize();
        auto textureRectPos = sprite->getTextureRect().getPosition();

        if (type == StatType::Empty)
        {
        }
        else
        {
            auto statValue = m_player->GetStatValue(type);
            uint8_t newTextureRectSizeX = (statValue * textureSize.x) / 100;
            if (textureRectSize.x > 0)
            {
                auto textureRect =
                    sf::IntRect{textureRectPos.x, textureRectPos.y, newTextureRectSizeX, textureRectSize.y};
                sprite->setTextureRect(textureRect);
            }
        }

        m_window->draw(*sprite);
    }
}

// ITEMS
void Game::SetItems(ItemGround *item)
{
    m_items.push_back(item);
}

void Game::RemoveItems(const size_t i)
{
    auto item = m_items.begin() + i;

    delete *item;
    *item = nullptr;
    m_items.erase(item);
}

// INITS
void Game::InitFolder()
{
    auto new_directory_path = filesystem::current_path();
    new_directory_path /= "save";

    if (!filesystem::exists(new_directory_path))
        filesystem::create_directory(new_directory_path);
}

void Game::InitSettings()
{
    LoadGeneral();
    LoadHotkeys();
}

void Game::LoadGeneral()
{
    auto path = filesystem::current_path();
    path /= "settings/general.json";

    ifstream file(path);

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        m_language = jsonData["language"];
        m_windowWidth = jsonData["resolutionWidth"];
        m_windowHeight = jsonData["resolutionHeight"];
        m_windowStyle = jsonData["resolutionStyle"];

        file.close();
    }
    else
    {
        /**
         * @brief  Default settings when no saved settings are found
         *
         */
        m_language = "enEN";

        // m_windowWidth = sf::VideoMode::getDesktopMode().width;
        // m_windowHeight = sf::VideoMode::getDesktopMode().height;
        // m_windowStyle = 8;

        /**
         * @brief Comment this back in to enable a standalone window for better testing
         * @test
         */
        m_windowWidth = 1280U;
        m_windowHeight = 720U;
        m_windowStyle = 4;
    }
}

void Game::InitHotkeys()
{
    ifstream fileLanguage("./data/language/hotkeys.json");

    if (fileLanguage.is_open())
    {
        Utilities utilities;
        auto font = utilities.GetFont(m_fonts, 0);
        auto jsonDataLanguage = json::parse(fileLanguage);

        for (const auto &[key, value] : m_hotkeys)
        {
            auto hotkeyName = utilities.GetLanguageText(jsonDataLanguage, key, m_language);
            auto sfmlHotkey = sf::Keyboard::delocalize(static_cast<sf::Keyboard::Key>(value));
            auto hotkeyString = sf::Keyboard::getDescription(sfmlHotkey).toAnsiString();

            auto string = format("{} : {}", hotkeyName, hotkeyString);

            auto text = new sf::Text();
            utilities.SetSFText(text, font, 20U, string);
            m_hotkeyMenu.push_back(text);
        }

        auto interact = m_hotkeys[static_cast<uint8_t>(Hotkey::Interact)];

        m_hotkeyRender = new sf::Text();

        utilities.SetSFText(m_hotkeyRender, font, 10);

        auto sfmlHotkey = sf::Keyboard::delocalize(static_cast<sf::Keyboard::Key>(interact));
        auto hotkeyString = sf::Keyboard::getDescription(sfmlHotkey).toAnsiString();

        m_hotkeyRender->setString(hotkeyString);

        fileLanguage.close();
    }
}

void Game::LoadHotkeys()
{
    ifstream file("./settings/hotkeys.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            uint8_t hotkeyID = data["hotkeyID"];
            uint16_t sfmlHotkey = data["SFMLHotkey"];

            m_hotkeys[hotkeyID] = sfmlHotkey;
        }

        file.close();
    }
}

void Game::InitPlayer()
{
    bool startGame = false;
    if (m_menuState == MenuState::Create)
    {
        if (CreatePlayer())
        {
            CreateSaveFolder(m_player->GetID());
            cout << "Player Create Done!" << endl;
            startGame = true;
        }
    }

    if (m_menuState == MenuState::Load)
    {
        auto saveGameID = GetDialogSelectedID();

        if (LoadPlayer(saveGameID))
        {
            m_player->Load(saveGameID, this);
            LoadGroundItems();
            cout << "Player Load Done!" << endl;
            startGame = true;
        }
    }

    if (startGame)
    {
        InitWorld();
        InitCreature();
        ResizeStats();

        m_playing = true;
        m_menuState = MenuState::Playing;

        // Thread Init
        m_thread = new Thread(this);
        cout << "Thread Init Done!" << endl;

        Saving(false);
    }
}

void Game::InitViews()
{
    sf::FloatRect visibleArea(0, 0, m_windowWidth, m_windowHeight);

    m_view = new sf::View(visibleArea);
    m_menuView = new sf::View(visibleArea);

    InitZoom();
}

void Game::InitZoom()
{
    auto center = m_view->getCenter();

    m_view->zoom(0.5F);
    m_zoom = 1U;

    m_view->move({-(center.x / 2), -(center.y / 2)});
}

void Game::InitItemCfg()
{
    Utilities utilities;
    ifstream fileLanguage("./data/language/menu.json");

    ifstream fileItem("./data/entities/item/item.json");
    ifstream fileItemTemplate("./data/entities/item/itemTemplate.json");

    if (fileItem.is_open() && fileLanguage.is_open() && fileItemTemplate.is_open())
    {
        auto jsonDataItem = nlohmann::ordered_json::parse(fileItem);
        auto jsonDataItemTemplate = json::parse(fileItemTemplate);

        auto jsonDataLanguage = json::parse(fileLanguage);

        for (const auto &data : jsonDataItem)
        {
            uint8_t ID = data["id"];
            uint8_t languageID = data["languageID"];
            uint8_t maxDrop = data["maxDrop"];
            uint8_t textureID = data["textureID"];
            uint8_t fontID = jsonDataItemTemplate["fontID"];

            auto textureData = sf::IntRect(data["textureData"][0],
                                           data["textureData"][1],
                                           data["textureData"][2],
                                           data["textureData"][3]);

            auto text = utilities.GetLanguageText(jsonDataLanguage, languageID, m_language);

            auto itemCfg = new ItemCfg(textureID, textureData, ID, text, maxDrop, fontID);
            m_itemCfg.push_back(itemCfg);
        }
        fileItemTemplate.close();
        fileLanguage.close();
        fileItem.close();
    }
}

void Game::InitTexture()
{
    ifstream file("./data/texture/texture.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            auto texture = make_unique<sf::Texture>();
            uint8_t ID = data["id"];
            string path = data["path"];

            texture->loadFromFile(path);

            auto textures = new Texture(move(texture), ID);
            m_textures.push_back(textures);
        }
        file.close();
    }
}

void Game::InitFont()
{
    ifstream file("./data/font/font.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            auto font = make_unique<sf::Font>();
            uint8_t ID = data["id"];
            string path = data["path"];

            font->loadFromFile(path);

            auto fonts = new Font(ID, move(font));
            m_fonts.push_back(fonts);
        }
        file.close();
    }
}

void Game::InitMenu()
{
    ifstream fileLanguage("./data/language/menu.json");

    ifstream fileTitle("./data/menu/title.json");
    ifstream fileTitleTemplate("./data/menu/titleTemplate.json");

    ifstream fileBtn("./data/menu/btn.json");
    ifstream fileBtnTemplate("./data/menu/btnTemplate.json");

    ifstream fileInput("./data/menu/input.json");
    ifstream fileInputTemplate("./data/menu/inputTemplate.json");

    if (fileTitle.is_open() && fileBtn.is_open() && fileInput.is_open() && fileTitleTemplate.is_open() &&
        fileBtnTemplate.is_open() && fileInputTemplate.is_open() && fileLanguage.is_open())
    {
        auto jsonDataLanguage = json::parse(fileLanguage);

        auto jsonDataTitle = nlohmann::ordered_json::parse(fileTitle);
        auto jsonDataTitleTemplate = json::parse(fileTitleTemplate);

        auto jsonDataBtn = nlohmann::ordered_json::parse(fileBtn);
        auto jsonDataBtnTemplate = json::parse(fileBtnTemplate);

        auto jsonDataInput = nlohmann::ordered_json::parse(fileInput);
        auto jsonDataInputTemplate = json::parse(fileInputTemplate);

        for (const auto &dataTitle : jsonDataTitle)
        {
            Alignment alignment = jsonDataTitleTemplate["alignment"];
            auto titleText = make_unique<sf::Text>();
            Element element = Element::Title;

            uint8_t fontSize = jsonDataTitleTemplate["fontSize"];
            uint8_t fontID = jsonDataTitleTemplate["fontID"];
            uint8_t languageID = dataTitle["languageID"];
            MenuState state = dataTitle["state"];
            Utilities utilities;
            sf::Sprite *prevBtn;
            sf::Text *prevInputText;
            sf::Text *prevTitleText;

            auto font = utilities.GetFont(m_fonts, fontID);

            auto text = utilities.GetLanguageText(jsonDataLanguage, languageID, m_language);

            utilities.SetSFText(titleText.get(), font, fontSize, text);

            utilities.SetTitlePos(m_windowWidth, titleText.get());

            auto titles = new Title(state, move(titleText), alignment);
            m_titles.push_back(titles);
            prevTitleText = titles->GetText();

            for (const auto &dataInput : jsonDataInput)
            {
                alignment = jsonDataInputTemplate["alignment"];

                vector<MenuState> inputState = dataInput["state"];

                auto canAdd = utilities.CheckMenuState(inputState, state);

                if (canAdd)
                {
                    if (dataInput.contains("alignment"))
                        alignment = dataInput["alignment"];

                    auto inputText = make_unique<sf::Text>();

                    fontID = jsonDataInputTemplate["fontID"];
                    fontSize = jsonDataInputTemplate["fontSize"];
                    uint8_t maxChars = jsonDataInputTemplate["maxChars"];

                    languageID = dataInput["languageID"];

                    auto font = utilities.GetFont(m_fonts, fontID);

                    auto text = utilities.GetLanguageText(jsonDataLanguage, languageID, m_language);

                    auto windowWidth = utilities.CalculateAlignmentWindowWidth(m_windowWidth, alignment);
                    auto spaceBetween = utilities.CalculateSpaceBetweenMenu(alignment);

                    utilities.SetSFText(inputText.get(), font, fontSize, text);

                    utilities.SetTitlePos(windowWidth, prevTitleText, inputText.get(), spaceBetween);

                    element = Element::Input;

                    auto inputs = new Input(state, move(inputText), maxChars, text, alignment);
                    m_inputs.push_back(inputs);
                    prevInputText = inputs->GetText();
                }
            }

            for (const auto &dataBtn : jsonDataBtn)
            {
                alignment = jsonDataBtnTemplate["alignment"];
                vector<MenuState> btnState = dataBtn["state"];

                auto canAdd = utilities.CheckMenuState(btnState, state);

                if (canAdd)
                {
                    if (dataBtn.contains("alignment"))
                        alignment = dataBtn["alignment"];

                    auto btnText = make_unique<sf::Text>();
                    auto btn = make_unique<sf::Sprite>();

                    auto scale = sf::Vector2f{jsonDataBtnTemplate["scale"][0], jsonDataBtnTemplate["scale"][1]};
                    fontSize = jsonDataBtnTemplate["fontSize"];
                    uint8_t textureID = jsonDataBtnTemplate["textureID"];
                    fontID = jsonDataBtnTemplate["fontID"];

                    auto textureRect = sf::IntRect{jsonDataBtnTemplate["textureData"][0],
                                                   jsonDataBtnTemplate["textureData"][1],
                                                   jsonDataBtnTemplate["textureData"][2],
                                                   jsonDataBtnTemplate["textureData"][3]};
                    languageID = dataBtn["languageID"];
                    BtnFunc btnFnc = dataBtn["fnc"];

                    auto texture = utilities.GetTexture(m_textures, textureID);

                    auto font = utilities.GetFont(m_fonts, fontID);

                    auto text = utilities.GetLanguageText(jsonDataLanguage, languageID, m_language);

                    utilities.SetSFSprite(btn.get(), texture, textureRect, scale);

                    utilities.SetSFText(btnText.get(), font, fontSize, text);

                    auto windowWidth = utilities.CalculateAlignmentWindowWidth(m_windowWidth, alignment);
                    auto spaceBetween = utilities.CalculateSpaceBetweenMenu(alignment);

                    if (element == Element::Title)
                        utilities.SetBtnAndTextPos(windowWidth, btn.get(), prevTitleText, btnText.get(), spaceBetween);
                    else if (element == Element::Input)
                        utilities.SetBtnAndTextPos(windowWidth, btn.get(), prevInputText, btnText.get(), spaceBetween);
                    else
                        utilities.SetBtnAndTextPos(windowWidth, btn.get(), prevBtn, btnText.get(), spaceBetween);

                    element = Element::Nothing;

                    auto btns = new Btn(state, btnFnc, move(btnText), move(btn), alignment);
                    m_btns.push_back(btns);
                    prevBtn = btns->GetSprite();
                }
            }
        }

        fileLanguage.close();
        fileTitle.close();
        fileBtn.close();
        fileInput.close();
        fileTitleTemplate.close();
        fileBtnTemplate.close();
        fileInputTemplate.close();
    }
}

void Game::InitSurface()
{
    Utilities utilities;
    random_device rd;  // a seed source for the random number engine
    mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

    // Random Grass Texture Selector
    uniform_int_distribution<> dist(0U, 3U);

    size_t j = 0;
    size_t k = 0;

    ifstream file("./data/world/surface.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (size_t i = 0; i < m_maxTiles; ++i)
        {
            sf::Texture *texture;
            uint8_t id;
            uint8_t textureID;
            sf::IntRect textureData;
            float speed;
            auto rnd = dist(gen);
            bool canCreate = false;

            if (j * m_tileSize >= m_gameWidth)
            {
                j = 0;
                ++k;
            }

            for (const auto &data : jsonData)
            {
                id = data["id"];
                textureID = data["textureID"];
                textureData = sf::IntRect(data["textureData"][0], data["textureData"][1], m_tileSize, m_tileSize);
                speed = data["speed"];

                auto texture = utilities.GetTexture(m_textures, textureID);

                if (id == 0 && j == 0 && k == 0 || (j * m_tileSize == (m_gameWidth - m_tileSize) && k == 0) ||
                    (k * m_tileSize == (m_gameHeight - m_tileSize) && j == 0) ||
                    (j * m_tileSize == (m_gameWidth - m_tileSize) && k * m_tileSize == (m_gameHeight - m_tileSize)))
                    canCreate = true;
                else if (id == 1 && j == 0)
                    canCreate = true;
                else if (id == 2 && j * m_tileSize == (m_gameWidth - m_tileSize))
                    canCreate = true;
                else if (id == 3 && k == 0)
                    canCreate = true;
                else if (id == 4 && k * m_tileSize == (m_gameHeight - m_tileSize))
                    canCreate = true;
                else if (id == 5 && rnd == 0)
                    canCreate = true;
                else if (id == 6 && rnd == 1)
                    canCreate = true;
                else if (id == 7 && rnd == 2)
                    canCreate = true;
                else if (id == 8 && rnd == 3)
                    canCreate = true;

                if (canCreate)
                {
                    auto tileSprite = make_unique<sf::Sprite>();

                    utilities.SetSFSprite(tileSprite.get(), texture, textureData, j * m_tileSize, k * m_tileSize);

                    auto surfaces = new Surface(move(tileSprite), speed);
                    m_surfaces.push_back(surfaces);
                    ++j;
                    break;
                }
            }
        }
        file.close();
    }
}

void Game::InitAnim()
{
    ifstream file("./data/anim/anim.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            uint8_t id = data["id"];
            uint8_t textureId = data["textureID"];

            auto anim = new Anim(id, textureId);
            m_anim.push_back(anim);

            for (const auto &data1 : data["textureRect"])
            {
                string state = data1["state"];

                uint16_t notMovingX = data1["notMoving"]["x"];
                uint16_t notMovingY = data1["notMoving"]["y"];
                uint16_t notMovingSizeX = data1["notMoving"]["sizeX"];
                uint16_t notMovingSizeY = data1["notMoving"]["sizeY"];

                uint16_t anim01X = data1["anim01"]["x"];
                uint16_t anim01Y = data1["anim01"]["y"];
                uint16_t anim01SizeX = data1["anim01"]["sizeX"];
                uint16_t anim01SizeY = data1["anim01"]["sizeY"];

                uint16_t anim02X = data1["anim02"]["x"];
                uint16_t anim02Y = data1["anim02"]["y"];
                uint16_t anim02SizeX = data1["anim02"]["sizeX"];
                uint16_t anim02SizeY = data1["anim02"]["sizeY"];

                AnimTexture animTexture = {.notMoving = {notMovingX, notMovingY, notMovingSizeX, notMovingSizeY},
                                           .anim01 = {anim01X, anim01Y, anim01SizeX, anim01SizeY},
                                           .anim02 = {anim02X, anim02Y, anim02SizeX, anim02SizeY}};

                anim->SetAnimTexture(state, animTexture);
            }
        }
        file.close();
    }
}

void Game::InitWindow()
{
    m_window = new sf::RenderWindow(sf::VideoMode(m_windowWidth, m_windowHeight), "Good Game", m_windowStyle);
    m_window->setVerticalSyncEnabled(true);
    m_window->setFramerateLimit(60U);
    m_window->setKeyRepeatEnabled(false);
}

void Game::InitCreature()
{
    Utilities utilities;
    ifstream fileCreatureTemplate("./data/entities/creature/creatureTemplate.json");
    ifstream fileCreature("./data/world/creature.json");
    ifstream fileDialog("./data/language/creature.json");
    ifstream fileTrader("./data/entities/trader/trader.json");

    if (fileCreatureTemplate.is_open() && fileCreature.is_open() && fileDialog.is_open() && fileTrader.is_open())
    {
        auto jsonDataCreatureTemplate = json::parse(fileCreatureTemplate);
        auto jsonDataCreature = json::parse(fileCreature);
        auto jsonDataDialog = json::parse(fileDialog);
        auto jsonDataTrader = json::parse(fileTrader);

        for (const auto &data : jsonDataCreature)
        {
            bool interactable = false;
            string name;
            uint8_t animID;
            const sf::Texture *texture;
            AnimTextureCombined animData;
            uint8_t templateId = data["creatureTemplateID"];
            uint8_t traderID = data["traderID"];
            vector<string> dialogIntro;
            vector<string> dialogOutro;
            vector<string> dialogOffensive;
            map<uint8_t, uint16_t> sellingItem;
            map<uint8_t, uint16_t> buyingItem;

            for (const auto &data1 : jsonDataCreatureTemplate)
            {
                uint8_t id = data1["id"];

                if (templateId == id)
                {
                    name = data1["name"];
                    animID = data1["animID"];
                    uint8_t dialogID = data1["dialogID"];

                    animData = utilities.GetAnim(m_anim, animID);

                    texture = utilities.GetAnimTexture(m_anim, m_textures, animID);

                    for (const auto &data3 : jsonDataDialog)
                    {
                        if (data3["id"] == dialogID)
                        {
                            for (const auto &data4 : data3["intro"][m_language])
                                dialogIntro.push_back(data4);

                            for (const auto &data4 : data3["outro"][m_language])
                                dialogOutro.push_back(data4);

                            for (const auto &data4 : data3["offensive"][m_language])
                                dialogOffensive.push_back(data4);
                        }
                    }
                }
            }


            for (const auto &data1 : jsonDataTrader)
            {
                if (data1["id"] == traderID)
                {
                    for (const auto &data2 : data1["selling"])
                    {
                        sellingItem[data2["itemID"]] = data2["count"];
                    }

                    for (const auto &data2 : data1["buying"])
                    {
                        buyingItem[data2["itemID"]] = data2["count"];
                    }

                    interactable = true;
                    break;
                }
            }

            vector<vector<float>> pos = data["pos"];

            for (const auto &data1 : pos)
            {
                float posX = data1[0];
                float posY = data1[1];
                bool moving = data1[2];

                auto tileSprite = make_unique<sf::Sprite>();

                utilities.SetSFSprite(tileSprite.get(), texture, animData.down.notMoving, posX, posY);

                if (sellingItem.size() > 0 || buyingItem.size() > 0)
                {
                    auto trader = new Trader(move(tileSprite),
                                             100.0F,
                                             1.0F,
                                             animID,
                                             moving,
                                             dialogIntro,
                                             dialogOutro,
                                             dialogOffensive,
                                             interactable,
                                             sellingItem,
                                             buyingItem);

                    m_creature.push_back(trader);
                }
                else
                {
                    auto creature = new Creature(move(tileSprite),
                                                 100.0F,
                                                 1.0F,
                                                 animID,
                                                 moving,
                                                 dialogIntro,
                                                 dialogOutro,
                                                 dialogOffensive,
                                                 interactable);

                    m_creature.push_back(creature);
                }
            }
        }

        fileDialog.close();
        fileTrader.close();
        fileCreatureTemplate.close();
        fileCreature.close();
    }
}


void Game::InitWorld()
{
    Utilities utilities;
    ifstream fileWorld("./data/world/world.json");
    ifstream fileWorldTemplate("./data/world/worldTemplate.json");

    auto saveId = m_player->GetID();
    auto path = format("./save/{}/world.json", saveId);

    ifstream fileWorldSave(path);

    if (fileWorld.is_open() && fileWorldTemplate.is_open())
    {
        bool saveFileFound = false;
        auto jsonDataWorld = json::parse(fileWorld);
        auto jsonDataWorldTemplate = json::parse(fileWorldTemplate);
        nlohmann::json_abi_v3_11_2::json jsonDataWorldSave;

        if (fileWorldSave.is_open())
        {
            jsonDataWorldSave = json::parse(fileWorldSave);
            saveFileFound = true;
        }

        const sf::Texture *texture;

        for (const auto &data : jsonDataWorld)
        {
            CollisionData templateCollision;
            CollisionData collision;
            vector<uint8_t> itemOutputID;
            sf::IntRect textureData;
            TextureProgData textureProgData;
            uint8_t worldId = data["id"];
            uint8_t templateId = data["worldTemplateID"];

            for (const auto &data1 : jsonDataWorldTemplate)
            {
                uint8_t id = data1["id"];

                if (templateId == id)
                {
                    templateCollision = {.x = data1["textureData"][4], .y = data1["textureData"][5]};

                    vector<uint8_t> itemOutputIds = data1["itemOutputID"];
                    for (const auto &data : itemOutputIds)
                        itemOutputID.push_back(data);

                    textureData = {data1["textureData"][0],
                                   data1["textureData"][1],
                                   data1["textureData"][2],
                                   data1["textureData"][3]};
                    textureProgData = {
                        .rect = sf::IntRect{data1["textureProgData"][0],
                                            data1["textureProgData"][1],
                                            data1["textureProgData"][2],
                                            data1["textureProgData"][3]},
                        .collision = CollisionData{.x = data1["textureProgData"][4], .y = data1["textureProgData"][5]}};

                    uint8_t textureID = data1["textureID"];

                    texture = utilities.GetTexture(m_textures, textureID);

                    break;
                }
            }

            vector<vector<float>> pos = data["pos"];

            for (size_t i = 0; const auto &data1 : pos)
            {
                bool load = false;
                float posX = data1[0];
                float posY = data1[1];

                if (saveFileFound)
                {
                    for (const auto &data2 : jsonDataWorldSave)
                    {
                        uint8_t saveID = data2["id"];

                        if (worldId == saveID)
                        {
                            uint8_t overrideId = data2["override"];
                            if (overrideId == i)
                            {
                                posX = data2["posX"];
                                posY = data2["posY"];
                                load = true;
                                break;
                            }
                        }
                    }
                }

                auto tileSprite = make_unique<sf::Sprite>();
                sf::IntRect rectangle;

                if (load)
                {
                    rectangle = textureProgData.rect;
                    collision = {.x = textureProgData.collision.x, .y = textureProgData.collision.y};
                }
                else
                {
                    rectangle = textureData;
                    collision = templateCollision;
                }

                utilities.SetSFSprite(tileSprite.get(), texture, rectangle, posX, posY);

                auto world = new World(move(tileSprite), worldId, collision, itemOutputID, textureProgData, load);
                m_world.push_back(world);
                ++i;
            }
        }
        fileWorld.close();
        fileWorldTemplate.close();
        fileWorldSave.close();
    }
}

void Game::InitRenderStats()
{
    Utilities utilities;
    ifstream file("./data/entities/player/stat.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            uint8_t add = data["add"];
            uint8_t textureID = data["textureID"];
            auto textureRect = sf::IntRect{data["textureData"][0],
                                           data["textureData"][1],
                                           data["textureData"][2],
                                           data["textureData"][3]};
            StatType type = data["type"];

            auto texture = utilities.GetTexture(m_textures, textureID);

            for (size_t i = 0; i < add; ++i)
            {
                auto sprite = make_unique<sf::Sprite>();

                utilities.SetSFSprite(sprite.get(), texture, textureRect);

                auto textureSize = textureRect.getSize();

                auto stats = new Stats(move(sprite), textureSize, type);
                m_stats.push_back(stats);
            }
        }
        file.close();
    }
}

// Render
void Game::Render(sf::Clock &clock)
{
    m_window->setView(*m_view);

    RenderSurface();
    m_player->HandleMove(clock, this);

    RenderItems();
    m_window->draw(*(m_player->GetSprite()));
    RenderCreature();

    RenderWorld();

    m_player->CheckCollision(this);

    m_player->CheckRenderHotkey(this);

    RenderStats();
}

void Game::RenderItems()
{
    Collision collision;
    for (const auto &data : m_items)
    {
        auto sprite = data->GetSprite();
        auto spritePos = sprite->getPosition();

        if (collision.InViewRange(this, spritePos))
            m_window->draw(*sprite);
    }
}

void Game::RenderSurface()
{
    Collision collision;
    auto playerSprite = m_player->GetSprite();
    auto playerPos = playerSprite->getPosition();

    for (auto &data : m_surfaces)
    {
        auto sprite = data->GetSprite();
        auto spritePos = sprite->getPosition();

        if (collision.InViewRange(this, spritePos))
        {
            auto tileSize = m_tileSize / 2;

            if (playerPos.x >= spritePos.x - tileSize && playerPos.x <= spritePos.x + tileSize &&
                playerPos.y >= spritePos.y - tileSize && playerPos.y <= spritePos.y + tileSize)
            {
                auto speed = data->GetSpeed();
                m_player->SetSpeed(speed);
            }

            m_window->draw(*sprite);
        }
    }
}

void Game::RenderWorld()
{
    Collision collision;
    for (auto &data : m_world)
    {
        auto sprite = data->GetSprite();
        auto spritePos = sprite->getPosition();

        if (collision.InViewRange(this, spritePos))
            m_window->draw(*sprite);
    }
}

void Game::RenderCreature()
{
    Collision collision;
    for (auto &data : m_creature)
    {
        auto sprite = data->GetSprite();
        auto spritePos = sprite->getPosition();

        if (collision.InViewRange(this, spritePos))
            m_window->draw(*sprite);
    }
}

float Game::GetRenderPuffer() const
{
    return m_renderPuffer;
}

void Game::RenderHotkey()
{
    auto playerPos = m_player->GetSprite()->getPosition();
    auto playerSize = m_player->GetSprite()->getLocalBounds().getSize();

    m_hotkeyRender->setPosition((playerPos.x - (m_hotkeyRender->getLocalBounds().getSize().x / 2)) + (playerSize.x / 2),
                                playerPos.y - 15);

    m_window->draw(*m_hotkeyRender);
}

void Game::RenderMenu()
{
    if (m_playing)
        m_player->SetMove(PlayerMove::NotMoving);

    m_window->setView(*m_menuView);
    Utilities utilities;
    sf::Sprite *lastbtn;

    for (const auto &data : m_titles)
    {
        Element element = Element::Title;

        if (m_menuState == data->GetMenuState())
        {
            auto previousTxt = data->GetText();
            m_window->draw(*previousTxt);

            for (const auto &data1 : m_hotkeyMenu)
            {
                if (m_menuState == MenuState::Hotkeys)
                {
                    auto spaceBetween = utilities.CalculateSpaceBetweenMenu(Alignment::Middle);

                    utilities.SetTitlePos(m_windowWidth, previousTxt, data1, spaceBetween);
                    m_window->draw(*data1);
                    previousTxt = data1;
                }
            }

            for (const auto &data1 : m_inputs)
            {
                if (m_menuState == data1->GetMenuState())
                {
                    element = Element::Input;
                    m_window->draw(*(data1->GetText()));
                    previousTxt = data1->GetText();
                }
            }

            for (const auto &data1 : m_saveFiles)
            {
                if (m_menuState == MenuState::OpenLoad)
                {
                    auto text = data1.get()->GetText();

                    text->setColor(sf::Color(255, 255, 255));

                    auto textID = data1.get()->GetID();

                    if (m_selectedTextID == textID)
                        text->setColor(sf::Color(0, 255, 0));

                    auto spaceBetween = utilities.CalculateSpaceBetweenMenu(Alignment::Middle);

                    utilities.SetTitlePos(m_windowWidth, previousTxt, text, spaceBetween);
                    m_window->draw(*text);
                    previousTxt = text;
                }
            }

            if (m_menuState == MenuState::Inventory || m_menuState == MenuState::Trader)
                previousTxt = RenderDialog();

            for (const auto &data1 : m_btns)
            {
                auto menuStates = data1->GetMenuState();

                if (m_menuState == menuStates)
                {
                    auto btnObj = data1->GetSprite();
                    auto btnText = data1->GetText();
                    if (m_menuState == MenuState::OpenLoad || m_menuState == MenuState::Hotkeys ||
                        m_menuState == MenuState::Trader || m_menuState == MenuState::Inventory)
                    {
                        auto alignment = data1->GetAlignment();
                        auto windowWidth = utilities.CalculateAlignmentWindowWidth(m_windowWidth, alignment);
                        auto spaceBetween = utilities.CalculateSpaceBetweenMenu(alignment);

                        if (element == Element::Title)
                            utilities.SetBtnAndTextPos(windowWidth, btnObj, previousTxt, btnText, spaceBetween);
                        else if (element == Element::Input)
                            utilities.SetBtnAndTextPos(windowWidth, btnObj, previousTxt, btnText, spaceBetween);
                        else
                            utilities.SetBtnAndTextPos(windowWidth, btnObj, lastbtn, btnText, spaceBetween);

                        element = Element::Nothing;
                        lastbtn = btnObj;
                    }

                    m_window->draw(*btnObj);
                    m_window->draw(*btnText);
                }
            }
            break;
        }
    }
}

// LOAD SAVE FILE
void Game::CreateLoadMenu()
{
    Utilities utilities;
    m_saveFiles.clear();
    m_selectedTextID = 0;

    auto count = CountSaveFolders();

    auto font = utilities.GetFont(m_fonts, 0);

    for (uint8_t i = 1; i < count; ++i)
    {
        auto text = make_unique<sf::Text>(format("{}. Savegame", i), *font, 20U);

        m_saveFiles.push_back(make_unique<SelectableText>(move(text), i, i));
    }
}

void Game::Saving(const bool destroy)
{
    m_player->Save(this);
    SaveCreatures(destroy);
    SaveWorld(destroy);
    SaveGroundItems(destroy);
}

void Game::SaveCreatures(const bool destroy)
{
    auto saveId = m_player->GetID();
    auto path = format("./save/{}/creature.json", saveId);

    ofstream file(path);

    if (file.is_open())
    {
        bool firstElement = true;
        file << '[';
        for (auto &data : m_creature)
        {
            if (data->GetMoving())
            {
                if (!firstElement)
                    file << ",";

                json jsonData = {{"posX", data->GetSprite()->getPosition().x},
                                 {"posY", data->GetSprite()->getPosition().y}};

                file << jsonData;

                if (destroy)
                {
                    delete data;
                    data = nullptr;
                }
                firstElement = false;
            }
        }

        file << ']';
        if (destroy)
            m_creature.clear();

        file.close();
    }
}

void Game::SaveWorld(const bool destroy)
{
    auto saveId = m_player->GetID();
    auto path = format("./save/{}/world.json", saveId);

    ofstream file(path);

    if (file.is_open())
    {
        uint8_t prevID = 0;
        bool firstElement = true;
        file << '[';
        for (size_t i = 0; auto &data : m_world)
        {
            auto id = data->GetID();
            if (prevID != id)
            {
                i = 0;
                prevID = id;
            }

            if (data->GetSaveIt())
            {
                if (!firstElement)
                    file << ",";

                json jsonData = {{"id", id},
                                 {"override", i},
                                 {"posX", data->GetSprite()->getPosition().x},
                                 {"posY", data->GetSprite()->getPosition().y}};

                file << jsonData;

                if (destroy)
                {
                    delete data;
                    data = nullptr;
                }
                firstElement = false;
            }
            ++i;
        }

        file << ']';
        if (destroy)
            m_world.clear();

        file.close();
    }
}

void Game::LoadGroundItems()
{
    auto saveId = m_player->GetID();
    auto path = format("./save/{}/groundItems.json", saveId);

    ifstream file(path);

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            uint8_t id = data["id"];
            uint16_t count = data["count"];
            float posX = data["posX"];
            float posY = data["posY"];

            for (const auto &data : m_itemCfg)
            {
                auto itemID = data->GetID();

                if (id == itemID)
                {
                    auto textureID = data->GetTextureID();
                    auto textureData = data->GetTextureData();
                    CreateGroundItem(textureID, textureData, posX, posY, id, count);
                }
            }
        }

        file.close();
    }
}

void Game::CreateGroundItem(const uint8_t textureID,
                            const sf::IntRect &textureData,
                            const float posX,
                            const float posY,
                            const uint8_t id,
                            const uint16_t count)
{
    Utilities utilities;

    auto texture = utilities.GetTexture(m_textures, textureID);

    auto itemSprite = make_unique<sf::Sprite>();

    utilities.SetSFSprite(itemSprite.get(), texture, textureData, posX, posY);

    auto item = new ItemGround(move(itemSprite), id, count);
    m_items.push_back(item);
}

void Game::SaveGroundItems(const bool destroy)
{
    auto saveId = m_player->GetID();
    auto path = format("./save/{}/groundItems.json", saveId);

    ofstream file(path);

    if (file.is_open())
    {
        bool firstElement = true;

        file << '[';
        for (auto &data : m_items)
        {
            if (!firstElement)
                file << ",";

            json jsonData = {{"id", data->GetID()},
                             {"count", data->GetCount()},
                             {"posX", data->GetSprite()->getPosition().x},
                             {"posY", data->GetSprite()->getPosition().y}};

            file << jsonData;

            if (destroy)
            {
                delete data;
                data = nullptr;
            }
            firstElement = false;
        }
        file << ']';

        if (destroy)
            m_items.clear();

        file.close();
    }
}

// RESIZE
void Game::ResizeWindow(const uint16_t width, const uint16_t height)
{
    if (m_windowWidth != width || m_windowHeight != height)
    {
        m_windowWidth = width;
        m_windowHeight = height;

        delete m_window;
        m_window = nullptr;

        m_window = new sf::RenderWindow(sf::VideoMode(m_windowWidth, m_windowHeight), "Good Game", m_windowStyle);
        SetWindowProperties();

        sf::FloatRect visibleArea(0, 0, m_windowWidth, m_windowHeight);

        delete m_menuView;
        m_menuView = nullptr;
        delete m_view;
        m_view = nullptr;

        m_menuView = new sf::View(visibleArea);
        m_view = new sf::View(visibleArea);
        m_window->setSize({m_windowWidth, m_windowHeight});

        SaveGeneral();
        InitZoom();
        UpdateView();
        ResizeMenu();
        ResizeStats();
    }
}

void Game::FullscreenWindow()
{
    if (m_windowStyle != sf::Style::Fullscreen)
    {
        m_windowStyle = sf::Style::Fullscreen;

        delete m_window;
        m_window = nullptr;

        m_window = new sf::RenderWindow(sf::VideoMode(m_windowWidth, m_windowHeight), "Good Game", m_windowStyle);
        SetWindowProperties();
        SaveGeneral();
    }
}

void Game::SetWindowProperties()
{
    m_window->setVerticalSyncEnabled(true);
    m_window->setFramerateLimit(60U);
    m_window->setKeyRepeatEnabled(false);
}

void Game::ResizeMenu()
{
    Utilities utilities;

    for (const auto &data : m_titles)
    {
        Element element = Element::Title;
        sf::Sprite *prevBtn;
        sf::Text *inputText;
        auto titleState = data->GetMenuState();

        auto text = data->GetText();
        utilities.SetTitlePos(m_windowWidth, text);

        for (const auto &dataInput : m_inputs)
        {
            auto inputState = dataInput->GetMenuState();

            if (titleState == inputState)
            {
                inputText = dataInput->GetText();
                auto alignment = dataInput->GetAlignment();

                auto windowWidth = utilities.CalculateAlignmentWindowWidth(m_windowWidth, alignment);
                auto spaceBetween = utilities.CalculateSpaceBetweenMenu(alignment);

                utilities.SetTitlePos(windowWidth, text, inputText, spaceBetween);

                element = Element::Input;
            }
        }

        for (const auto &data2 : m_btns)
        {
            auto btnState = data2->GetMenuState();

            if (titleState == btnState)
            {
                auto sprite = data2->GetSprite();
                auto btnText = data2->GetText();
                auto alignment = data2->GetAlignment();

                auto windowWidth = utilities.CalculateAlignmentWindowWidth(m_windowWidth, alignment);
                auto spaceBetween = utilities.CalculateSpaceBetweenMenu(alignment);

                if (element == Element::Title)
                    utilities.SetBtnAndTextPos(windowWidth, sprite, text, btnText, spaceBetween);
                else if (element == Element::Input)
                    utilities.SetBtnAndTextPos(windowWidth, sprite, inputText, btnText, spaceBetween);
                else
                    utilities.SetBtnAndTextPos(windowWidth, sprite, prevBtn, btnText, spaceBetween);

                prevBtn = sprite;
                element = Element::Nothing;
            }
        }
    }
}

// FOLDER
void Game::CreateSaveFolder(const uint8_t id)
{
    auto new_directory_path = filesystem::current_path();
    new_directory_path /= format("save/{}", id);

    if (!filesystem::exists(new_directory_path))
        filesystem::create_directory(new_directory_path);
}

uint8_t Game::CountSaveFolders()
{
    auto directory_path = filesystem::current_path();
    directory_path /= format("save");
    int8_t id = 0;

    if (filesystem::exists(directory_path))
    {
        int8_t count = distance(filesystem::directory_iterator(directory_path), filesystem::directory_iterator{});

        for (size_t i = 1; i < count + 1; ++i)
        {
            filesystem::path folderKnown = directory_path;
            folderKnown /= format("{}", i);
            if (!filesystem::exists(folderKnown))
            {
                id = i;
                break;
            }
        }

        if (id == 0)
            id = count + 1;
    }
    return id;
}

// MENU
void Game::ResetInputToDefault()
{
    for (const auto &data : m_inputs)
    {
        if (data->GetMenuState() == m_menuState)
            data->ResetToDefaultString(m_windowWidth);
    }
}

// THREAD
Thread *Game::GetThread()
{
    return m_thread;
}


// SETTINGS
map<uint8_t, uint16_t> Game::GetHotkeys() const
{
    return m_hotkeys;
}

void Game::ChangeLanguage(const string language)
{
    if (language == m_language)
        return;

    m_language = language;

    for (auto &data : m_btns)
    {
        delete data;
        data = nullptr;
    }
    m_btns.clear();

    for (auto &data : m_titles)
    {
        delete data;
        data = nullptr;
    }
    m_titles.clear();

    for (auto &data : m_inputs)
    {
        delete data;
        data = nullptr;
    }
    m_inputs.clear();

    for (auto &data : m_hotkeyMenu)
    {
        delete data;
        data = nullptr;
    }
    m_hotkeyMenu.clear();

    SaveGeneral();
    InitMenu();
    InitHotkeys();
}

void Game::SaveGeneral()
{
    ofstream file("./settings/general.json");

    if (file.is_open())
    {
        json jsonData = {{"language", m_language},
                         {"resolutionWidth", m_windowWidth},
                         {"resolutionHeight", m_windowHeight},
                         {"resolutionStyle", m_windowStyle}};

        file << jsonData;
        file.close();
    }
}

// DIALOG
void Game::SetDialogSprite(unique_ptr<Sprite> sprite)
{
    m_dialogSprites.push_back(move(sprite));
}

void Game::SetDialogText(unique_ptr<SelectableText> text)
{
    m_dialogTexts.push_back(move(text));
}

void Game::ClearDialog()
{
    m_dialogSprites.clear();
    m_dialogTexts.clear();
    m_selectedTextID = 0;
}

sf::Text *Game::RenderDialog()
{
    sf::Text *prevTxt;
    for (const auto &data : m_dialogSprites)
    {
        m_window->draw(*(data.get()->GetSprite()));
    }

    for (const auto &data : m_dialogTexts)
    {
        auto text = data.get()->GetText();

        text->setColor(sf::Color(255, 255, 255));

        auto textID = data.get()->GetID();
        prevTxt = data.get()->GetText();

        if (m_selectedTextID == textID)
            text->setColor(sf::Color(0, 255, 0));

        m_window->draw(*prevTxt);
    }

    return prevTxt;
}

const vector<unique_ptr<SelectableText>> *Game::GetDialogText() const
{
    return &m_dialogTexts;
}

MenuState Game::GetMenuState() const
{
    return m_menuState;
}

void Game::SetMenuState(const MenuState menuState)
{
    m_menuState = menuState;
}

uint8_t Game::GetDialogSelectedID() const
{
    for (const auto &data : m_saveFiles)
    {
        auto textID = data.get()->GetID();

        if (m_selectedTextID == textID)
        {
            return data.get()->GetSelectedID();
        }
    }
    return 0;
}

void Game::SetSelectedTextID(const uint8_t ID)
{
    m_selectedTextID = ID;
}

uint8_t Game::GetSelectedTextID() const
{
    return m_selectedTextID;
}
