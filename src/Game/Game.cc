#include "Game.h"
#include "Utilities.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <random>

using json = nlohmann::json;
using namespace std;

Game::Game(const uint16_t windowWidth, const uint16_t windowHeight)
    : m_windowWidth(windowWidth), m_windowHeight(windowHeight), Gui(MenuState::Main)
{
    m_player = nullptr;
    m_maxZoom = 3U;
    m_playing = false;
    m_defaultPlayerTextureID = 2U;
    m_statDecay = {.food = 0.2F, .water = 0.5F};
    m_gameWidth = 8800U;
    m_gameHeight = 4800U;
    m_windowZoomHeight = m_windowHeight;
    m_windowZoomWidth = m_windowWidth;
    m_tileSize = 32U;
    m_maxTiles = ((m_gameWidth * m_gameHeight) / m_tileSize) / m_tileSize;
}

void Game::SetMenuState(MenuState state)
{
    m_menuState = state;
}

// RUNNING
bool Game::GetPlaying() const
{
    return m_playing;
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
uint16_t Game::GetWindowZoomHeight() const
{
    return m_windowZoomHeight;
}

uint16_t Game::GetWindowZoomWidth() const
{
    return m_windowZoomWidth;
}

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

// ZOOM
void Game::SetZoom(const uint8_t zoom)
{
    m_zoom += zoom;
}

void Game::SetZoom(const uint8_t zoom, const float zoomLevel)
{
    m_view.zoom(zoomLevel);
    m_zoom += zoom;
}

void Game::UpdateZoom(const float delta)
{
    if (delta > 0U)
    {
        if (m_zoom < m_maxZoom)
        {
            SetZoom(1U, 0.5F);
            m_windowZoomWidth /= 2U;
            m_windowZoomHeight /= 2U;
        }
    }
    else
    {
        if (m_zoom > 0U)
        {
            SetZoom(-1, 2.0F);
            m_windowZoomWidth *= 2U;
            m_windowZoomHeight *= 2U;
        }
    }

    UpdateView();
}

// VECTOR
vector<AllTextures *> Game::GetTexture() const
{
    return m_textures;
}

vector<World *> Game::GetWorld() const
{
    return m_world;
}

vector<ItemCfg *> Game::GetItemCfg() const
{
    return m_itemCfg;
}

vector<Item *> Game::GetItem() const
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

vector<sf::Text *> Game::GetSaveFiles() const
{
    return m_saveFiles;
}

vector<Btn *> Game::GetBtn() const
{
    return m_btns;
}

// VIEW
sf::View Game::GetView() const
{
    return m_view;
}

void Game::UpdateView()
{
    auto size = m_view.getSize();
    auto center = m_view.getCenter();

    if (center.x >= m_gameWidth - (size.x / 2U))
    {
        m_view.setCenter({m_gameWidth - (size.x / 2U), center.y});
        center = m_view.getCenter();
    }
    if (center.x <= 0U + (size.x / 2U))
    {
        m_view.setCenter({size.x / 2U, center.y});
        center = m_view.getCenter();
    }
    if (center.y <= 0U + (size.y / 2U))
    {
        m_view.setCenter({center.x, size.y / 2U});
        center = m_view.getCenter();
    }
    if (center.y >= m_gameHeight - (size.y / 2U))
    {
        m_view.setCenter({center.x, m_gameHeight - (size.y / 2U)});
    }
}

void Game::HandleViewPosition(const sf::RenderWindow &window)
{
    // get the current mouse position in the window
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    // convert it to world coordinates
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    auto center = m_view.getCenter();
    auto size = m_view.getSize();

    auto moveX = worldPos.x - center.x;
    auto moveY = worldPos.y - center.y;

    // RIGHT
    if (moveX > 0U)
    {
        if (moveX + center.x >= m_gameWidth - (size.x / 2U))
            m_view.setCenter({m_gameWidth - (size.x / 2U), center.y});
        else
            m_view.move({moveX, 0U});
    }
    else // LEFT
    {
        if (center.x + (moveX) <= 0U + (size.x / 2U))
            m_view.setCenter({size.x / 2U, center.y});
        else
            m_view.move({moveX, 0U});
    }
    center = m_view.getCenter();

    // UP
    if (moveY < 0U)
    {
        if (moveY + center.y <= 0U + (size.y / 2U))
            m_view.setCenter({center.x, size.y / 2U});
        else
            m_view.move({0U, moveY});
    }
    else // DOWN
    {
        if (moveY + center.y >= m_gameHeight - (size.y / 2U))
            m_view.setCenter({center.x, m_gameHeight - (size.y / 2U)});
        else
            m_view.move({0U, moveY});
    }
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
    auto sprite = new sf::Sprite();
    sf::Texture *texture;

    for (const auto &data : m_textures)
    {
        auto texID = data->GetID();
        if (texID == m_defaultPlayerTextureID)
        {
            texture = data->GetTexture();
        }
    }

    sprite->setTexture(*texture);

    for (const auto &data : m_anim)
    {
        if (data->GetTextureID() == m_defaultPlayerTextureID)
        {
            sprite->setTextureRect(data->GetMoveAnim().down00);
            break;
        }
    }

    m_player = new Player(sprite, m_defaultPlayerTextureID, id);

    return true;
}

bool Game::CreatePlayer()
{
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


    auto sprite = new sf::Sprite();
    sf::Texture *texture;

    for (const auto &data : m_textures)
    {
        auto texID = data->GetID();
        if (texID == m_defaultPlayerTextureID)
        {
            texture = data->GetTexture();
        }
    }

    sprite->setTexture(*texture);

    for (const auto &data : m_anim)
    {
        if (data->GetTextureID() == m_defaultPlayerTextureID)
        {
            sprite->setTextureRect(data->GetMoveAnim().down00);
            break;
        }
    }

    m_player = new Player(sprite, m_defaultPlayerTextureID, playerName, countFolders);
    return true;
}


// STATS
StatDecay Game::GetStatDecay() const
{
    return m_statDecay;
}

// ITEMS
void Game::SetItems(Item *item)
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
void Game::InitPlayer(sf::RenderWindow &window, Game &game)
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
        if (LoadPlayer(m_saveGameID))
        {
            m_player->Load(m_saveGameID);
            cout << "Player Load Done!" << endl;
            startGame = true;
        }
    }

    if (startGame)
    {
        // Thread Init
        m_thread = new Thread(window, m_player, game);
        cout << "Thread Init Done!" << endl;

        m_playing = true;
        m_menuState = MenuState::Playing;

        m_player->Save();
    }
}

void Game::InitViews()
{
    m_defaultCenter = sf::Vector2f(m_windowWidth / 2U, m_windowHeight / 2U);

    m_menuView.setSize(sf::Vector2f(m_windowWidth, m_windowHeight));
    m_menuView.setCenter(m_defaultCenter);

    m_view.setSize(sf::Vector2f(m_windowWidth, m_windowHeight));
    m_view.setCenter(m_defaultCenter);

    auto center = m_view.getCenter();

    m_view.zoom(0.5F);
    m_zoom = 1U;
    m_windowZoomWidth /= 2U;
    m_windowZoomHeight /= 2U;

    m_view.move({-(center.x / 2), -(center.y / 2)});
}

void Game::InitItemCfg()
{
    ifstream file("./data/item/item.json");

    if (file.is_open())
    {
        auto jsonData = nlohmann::ordered_json::parse(file);

        for (const auto &data : jsonData)
        {
            uint8_t ID = data["id"];
            string name = data["name"];
            uint8_t maxDrop = data["maxDrop"];
            sf::Texture *texture;
            uint8_t textureID = data["textureID"];
            auto textureData = sf::IntRect(data["textureData"][0],
                                           data["textureData"][1],
                                           data["textureData"][2],
                                           data["textureData"][3]);

            for (const auto &data1 : m_textures)
            {
                auto texID = data1->GetID();
                if (texID == textureID)
                {
                    texture = data1->GetTexture();
                }
            }

            auto itemCfg = new ItemCfg(texture, textureData, ID, name, maxDrop);
            m_itemCfg.push_back(itemCfg);
        }
        file.close();
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
            auto texture = new sf::Texture();
            uint8_t ID = data["id"];
            string path = data["path"];

            texture->loadFromFile(path);

            auto textures = new AllTextures(texture, ID);
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
            auto font = new sf::Font();
            uint8_t ID = data["id"];
            string path = data["path"];

            font->loadFromFile(path);

            auto fonts = new Font(ID, font);
            m_fonts.push_back(fonts);
        }
        file.close();
    }
}

void Game::InitMenu()
{
    enum class Element
    {
        Nothing = 0,
        Title,
        Input
    };

    ifstream fileTitle("./data/menu/title.json");
    ifstream fileTitleCfg("./data/menu/titleDefaultCfg.json");

    ifstream fileBtn("./data/menu/btn.json");
    ifstream fileBtnCfg("./data/menu/btnDefaultCfg.json");

    ifstream fileInput("./data/menu/input.json");
    ifstream fileInputCfg("./data/menu/inputDefaultCfg.json");

    if (fileTitle.is_open() && fileBtn.is_open() && fileInput.is_open() && fileTitleCfg.is_open() &&
        fileBtnCfg.is_open() && fileInputCfg.is_open())
    {
        auto jsonDataTitle = nlohmann::ordered_json::parse(fileTitle);
        auto jsonDataTitleCfg = json::parse(fileTitleCfg);

        auto jsonDataBtn = nlohmann::ordered_json::parse(fileBtn);
        auto jsonDataBtnCfg = json::parse(fileBtnCfg);

        auto jsonDataInput = nlohmann::ordered_json::parse(fileInput);
        auto jsonDataInputCfg = json::parse(fileInputCfg);

        for (const auto &dataTitle : jsonDataTitle)
        {
            auto titleText = new sf::Text();
            Element element = Element::Title;

            uint8_t fontSize = jsonDataTitleCfg["fontSize"];
            uint8_t fontID = jsonDataTitleCfg["fontID"];

            MenuState state = dataTitle["state"];
            string text = dataTitle["name"];
            sf::Font *font;
            Utilities utilities;
            sf::Sprite *prevBtn;
            sf::Text *inputText;

            for (const auto &data : m_fonts)
            {
                auto fonID = data->GetID();
                if (fonID == fontID)
                {
                    font = data->GetFont();
                    break;
                }
            }

            titleText->setFont(*font);
            titleText->setCharacterSize(fontSize);
            titleText->setString(text);

            utilities.SetTitlePos(m_windowWidth, titleText);

            auto titles = new Title(state, titleText);
            m_titles.push_back(titles);

            for (const auto &dataInput : jsonDataInput)
            {
                bool addInput = false;
                vector<MenuState> inputState = dataInput["state"];

                for (const auto &data : inputState)
                {
                    if (state == data)
                    {
                        addInput = true;
                        break;
                    }
                }

                if (addInput)
                {
                    inputText = new sf::Text();

                    fontID = jsonDataInputCfg["fontID"];
                    fontSize = jsonDataInputCfg["fontSize"];
                    uint8_t maxChars = jsonDataInputCfg["maxChars"];

                    text = dataInput["name"];

                    for (const auto &data : m_fonts)
                    {
                        auto fonID = data->GetID();
                        if (fonID == fontID)
                        {
                            font = data->GetFont();
                            break;
                        }
                    }

                    inputText->setFont(*font);
                    inputText->setCharacterSize(fontSize);

                    utilities.SetTitlePos(m_windowWidth, titleText, inputText);

                    element = Element::Input;

                    auto inputs = new Input(state, inputText, maxChars, text);
                    m_inputs.push_back(inputs);
                }
            }

            for (const auto &dataBtn : jsonDataBtn)
            {
                bool addBtn = false;
                vector<MenuState> btnState = dataBtn["state"];
                for (const auto &data : btnState)
                {
                    if (state == data)
                    {
                        addBtn = true;
                        break;
                    }
                }

                if (addBtn)
                {
                    auto btnText = new sf::Text();
                    auto btn = new sf::Sprite();

                    auto scale = sf::Vector2f{jsonDataBtnCfg["scale"][0], jsonDataBtnCfg["scale"][1]};
                    fontSize = jsonDataBtnCfg["fontSize"];
                    uint8_t textureID = jsonDataBtnCfg["textureID"];
                    fontID = jsonDataBtnCfg["fontID"];

                    auto textureRect = sf::IntRect{dataBtn["textureData"][0],
                                                   dataBtn["textureData"][1],
                                                   dataBtn["textureData"][2],
                                                   dataBtn["textureData"][3]};
                    text = dataBtn["name"];
                    BtnFunc btnFnc = dataBtn["fnc"];

                    sf::Texture *texture;

                    for (const auto &data : m_textures)
                    {
                        auto texID = data->GetID();
                        if (texID == textureID)
                        {
                            texture = data->GetTexture();
                            break;
                        }
                    }

                    for (const auto &data : m_fonts)
                    {
                        auto fonID = data->GetID();
                        if (fonID == fontID)
                        {
                            font = data->GetFont();
                            break;
                        }
                    }

                    btn->setTexture(*texture);
                    btn->setTextureRect(textureRect);
                    btn->setScale(scale);

                    btnText->setFont(*font);
                    btnText->setCharacterSize(fontSize);
                    btnText->setString(text);

                    if (element == Element::Title)
                        utilities.SetBtnAndTextPos(m_windowWidth, btn, titleText, btnText);
                    else if (element == Element::Input)
                        utilities.SetBtnAndTextPos(m_windowWidth, btn, inputText, btnText);
                    else
                        utilities.SetBtnAndTextPos(m_windowWidth, btn, prevBtn, btnText);

                    prevBtn = btn;
                    element = Element::Nothing;

                    auto btns = new Btn(state, btnFnc, btnText, btn);
                    m_btns.push_back(btns);
                }
            }
        }

        fileTitle.close();
        fileBtn.close();
        fileInput.close();
        fileTitleCfg.close();
        fileBtnCfg.close();
        fileInputCfg.close();
    }
}

void Game::InitSurface()
{
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

                for (const auto &data1 : m_textures)
                {
                    auto texID = data1->GetID();
                    if (texID == textureID)
                    {
                        texture = data1->GetTexture();
                    }
                }

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
                    auto tileSprite = new sf::Sprite();

                    tileSprite->setTextureRect(textureData);
                    tileSprite->setTexture(*texture);
                    tileSprite->setPosition(j * m_tileSize, k * m_tileSize);

                    auto surfaces = new Surface(tileSprite, speed);
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

            MovementTexture animTexture = {.up00 = {data["textureUp"][0][0], data["textureUp"][0][1], 16, 16},
                                           .up01 = {data["textureUp"][1][0], data["textureUp"][1][1], 16, 16},
                                           .up02 = {data["textureUp"][2][0], data["textureUp"][2][1], 16, 16},
                                           .down00 = {data["textureDown"][0][0], data["textureDown"][0][1], 16, 16},
                                           .down01 = {data["textureDown"][1][0], data["textureDown"][1][1], 16, 16},
                                           .down02 = {data["textureDown"][2][0], data["textureDown"][2][1], 16, 16},
                                           .left00 = {data["textureLeft"][0][0], data["textureLeft"][0][1], 16, 16},
                                           .left01 = {data["textureLeft"][1][0], data["textureLeft"][1][1], 16, 16},
                                           .left02 = {data["textureLeft"][2][0], data["textureLeft"][2][1], 16, 16},
                                           .right00 = {data["textureRight"][0][0], data["textureRight"][0][1], 16, 16},
                                           .right01 = {data["textureRight"][1][0], data["textureRight"][1][1], 16, 16},
                                           .right02 = {data["textureRight"][2][0], data["textureRight"][2][1], 16, 16}};

            auto anim = new Anim(id, textureId, animTexture);
            m_anim.push_back(anim);
        }
        file.close();
    }
}

void Game::InitWorld()
{
    ifstream file("./data/world/world.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        sf::Texture *texture;
        for (const auto &data : jsonData)
        {

            Collision collision = {.x = data["textureData"][4], .y = data["textureData"][5]};
            vector<uint8_t> itemOutputID = data["itemOutputID"];
            sf::IntRect textureData = {data["textureData"][0],
                                       data["textureData"][1],
                                       data["textureData"][2],
                                       data["textureData"][3]};
            TextureProgData textureProgData = {
                .rect = sf::IntRect{data["textureProgData"][0],
                                    data["textureProgData"][1],
                                    data["textureProgData"][2],
                                    data["textureProgData"][3]},
                .collision = Collision{.x = data["textureProgData"][4], .y = data["textureProgData"][5]}};

            uint8_t textureID = data["textureID"];

            for (const auto &data2 : m_textures)
            {
                auto texID = data2->GetID();
                if (texID == textureID)
                {
                    texture = data2->GetTexture();
                }
            }

            for (const auto &data1 : data["pos"])
            {
                auto tileSprite = new sf::Sprite();

                tileSprite->setTexture(*texture);
                tileSprite->setTextureRect(textureData);
                tileSprite->setPosition(data1[0], data1[1]);

                auto world = new World(tileSprite, collision, itemOutputID, textureProgData);
                m_world.push_back(world);
            }
        }
        file.close();
    }
}

void Game::InitDrawStats()
{
    ifstream file("./data/player/stat.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            sf::Texture *texture;
            uint8_t add = data["add"];
            uint8_t textureID = data["textureID"];
            auto textureRect = sf::IntRect{data["textureData"][0],
                                           data["textureData"][1],
                                           data["textureData"][2],
                                           data["textureData"][3]};
            StatType type = data["type"];

            for (const auto &data : m_textures)
            {
                if (data->GetID() == textureID)
                {
                    texture = data->GetTexture();
                }
            }

            for (size_t i = 0; i < add; ++i)
            {
                auto sprite = new sf::Sprite();

                sprite->setTexture(*texture);
                sprite->setTextureRect(textureRect);
                auto textureSize = textureRect.getSize();

                auto stats = new Stats(sprite, textureSize, type);
                m_stats.push_back(stats);
            }
        }
        file.close();
    }
}

// DRAW
void Game::DrawItems(sf::RenderWindow &window)
{
    for (const auto &data : m_items)
    {
        auto sprite = data->GetSprite();

        window.draw(*sprite);
    }
}

void Game::DrawSurface(sf::RenderWindow &window, Player *player)
{
    auto playerSprite = player->GetSprite();
    auto playerPos = playerSprite->getPosition();

    for (auto &data : m_surfaces)
    {
        auto sprite = data->GetSprite();
        auto spritePos = sprite->getPosition();
        auto tileSize = m_tileSize / 2;

        if (playerPos.x >= spritePos.x - tileSize && playerPos.x <= spritePos.x + tileSize &&
            playerPos.y >= spritePos.y - tileSize && playerPos.y <= spritePos.y + tileSize)
        {
            auto speed = data->GetSpeed();
            player->SetSpeed(speed);
        }

        window.draw(*sprite);
    }
}

void Game::DrawWorld(sf::RenderWindow &window)
{
    for (auto &data : m_world)
    {
        auto sprite = data->GetSprite();

        window.draw(*sprite);
    }
}

void Game::DrawMenu(sf::RenderWindow &window)
{
    window.setView(m_menuView);
    Utilities utilities;
    sf::Sprite *lastbtn;

    for (const auto &data : m_titles)
    {
        bool firstBtn = true;
        sf::Text *previousTxt = data->GetText();

        if (m_menuState == data->GetMenuState())
            window.draw(*(data->GetText()));

        if (m_menuState == MenuState::Inventory)
            m_player->DrawInventoryItems(window, m_itemCfg);

        for (const auto &data1 : m_inputs)
        {
            if (m_menuState == data1->GetMenuState())
                window.draw(*(data1->GetText()));
        }

        for (const auto &data1 : m_saveFiles)
        {
            if (m_menuState == MenuState::OpenLoad)
            {
                utilities.SetTitlePos(m_windowWidth, previousTxt, data1);
                window.draw(*data1);
                previousTxt = data1;
            }
        }

        for (const auto &data1 : m_btns)
        {
            bool showBtn = false;
            auto menuStates = data1->GetMenuState();

            if (m_menuState == menuStates)
            {
                if (m_menuState == MenuState::OpenLoad)
                {
                    if (firstBtn)
                    {
                        utilities.SetBtnAndTextPos(m_windowWidth, data1->GetSprite(), previousTxt, data1->GetText());
                        lastbtn = data1->GetSprite();
                        firstBtn = false;
                    }
                    else
                        utilities.SetBtnAndTextPos(m_windowWidth, data1->GetSprite(), lastbtn, data1->GetText());
                }

                window.draw(*(data1->GetSprite()));
                window.draw(*(data1->GetText()));
            }
        }
    }
}

// LOAD SAVE FILE
void Game::CreateLoadMenu()
{
    for (auto &data : m_saveFiles)
    {
        delete data;
        data = nullptr;
    }
    m_saveFiles.clear();

    sf::Font *font;
    auto count = CountSaveFolders();

    for (const auto &data : m_fonts)
    {
        if (data->GetID() == 0)
        {
            font = data->GetFont();
        }
    }

    for (size_t i = 1; i < count; ++i)
    {
        auto text = new sf::Text(format("{}. Savegame", i), *font, 20U);
        m_saveFiles.push_back(text);
    }
}

uint8_t Game::GetSaveGameID() const
{
    return m_saveGameID;
}

void Game::SetSaveGameID(const uint8_t id)
{
    m_saveGameID = id;
}

// RESIZE
void Game::ResizeWindow(sf::RenderWindow &window)
{
    window.setSize(sf::Vector2u(m_windowWidth, m_windowHeight));

    m_defaultCenter = sf::Vector2f(m_windowWidth / 2U, m_windowHeight / 2U);

    m_menuView.setSize(sf::Vector2f(m_windowWidth, m_windowHeight));
    m_menuView.setCenter(m_defaultCenter);

    ResizeMenu();
}

void Game::ResizeMenu()
{
    Utilities utilities;
    sf::Sprite *prevBtn;

    for (const auto &data : m_titles)
    {
        auto titleState = data->GetMenuState();
        bool firstBtn = true;

        auto text = data->GetText();
        utilities.SetTitlePos(m_windowWidth, text);

        for (const auto &data2 : m_btns)
        {
            auto btnState = data2->GetMenuState();

            if (titleState == btnState)
            {
                auto sprite = data2->GetSprite();
                auto btnText = data2->GetText();
                if (firstBtn)
                    utilities.SetBtnAndTextPos(m_windowWidth, sprite, text, btnText);
                else
                    utilities.SetBtnAndTextPos(m_windowWidth, sprite, prevBtn, btnText);

                prevBtn = sprite;
                firstBtn = false;
            }
        }
    }
}

// FOLDER
void Game::CreateFolder()
{
    auto new_directory_path = filesystem::current_path();
    new_directory_path /= "save";

    if (!filesystem::exists(new_directory_path))
    {
        filesystem::create_directory(new_directory_path);
    }
}

void Game::CreateSaveFolder(const uint8_t id)
{
    auto new_directory_path = filesystem::current_path();
    new_directory_path /= format("save/{}", id);

    if (!filesystem::exists(new_directory_path))
    {
        filesystem::create_directory(new_directory_path);
    }
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
        {
            data->ResetToDefaultString(m_windowWidth);
        }
    }
}

// THREAD
Thread *Game::GetThread()
{
    return m_thread;
}
