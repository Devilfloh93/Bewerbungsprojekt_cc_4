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
    auto countFolders = CountFolders();
    if (countFolders <= 0)
        return false;

    string_view playerName = "";
    for (const auto &data : m_inputs)
    {
        if (m_menuState == data->GetMenuState())
            playerName = data->GetName();
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

// THREAD
Thread *Game::GetThread() const
{
    return m_thread;
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

    vector<nlohmann::json_abi_v3_11_2::ordered_json> jsonItems;

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
    ifstream fileBtn("./data/menu/button.json");
    ifstream fileInput("./data/menu/input.json");

    if (fileTitle.is_open() && fileBtn.is_open() && fileInput.is_open())
    {
        auto jsonDataTitle = nlohmann::ordered_json::parse(fileTitle);
        auto jsonDataBtn = nlohmann::ordered_json::parse(fileBtn);
        auto jsonDataInput = nlohmann::ordered_json::parse(fileInput);

        for (const auto &dataTitle : jsonDataTitle)
        {
            auto titleText = new sf::Text();
            Element element = Element::Title;

            MenuState state = dataTitle["state"];
            uint8_t fontSize = dataTitle["fontSize"];
            string text = dataTitle["name"];
            uint8_t fontID = dataTitle["fontID"];
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
                vector<MenuState> btnState = dataInput["state"];
                for (const auto &data : btnState)
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
                    fontID = dataInput["fontID"];
                    fontSize = dataInput["fontSize"];
                    uint8_t maxChars = dataInput["maxChars"];

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

                    auto inputs = new Input(state, inputText, maxChars);
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

                    auto textureRect = sf::IntRect{dataBtn["textureData"][0],
                                                   dataBtn["textureData"][1],
                                                   dataBtn["textureData"][2],
                                                   dataBtn["textureData"][3]};
                    auto scale = sf::Vector2f{dataBtn["scale"][0], dataBtn["scale"][1]};
                    text = dataBtn["name"];
                    fontSize = dataBtn["fontSize"];
                    BtnFunc btnFnc = dataBtn["fnc"];
                    uint8_t textureID = dataBtn["textureID"];
                    fontID = dataBtn["fontID"];
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

                    auto buttons = new Button(state, btnFnc, btnText, btn);
                    m_buttons.push_back(buttons);
                }
            }
        }

        fileTitle.close();
        fileBtn.close();
        fileInput.close();
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

    for (const auto &data : m_titles)
    {
        if (m_menuState == data->GetMenuState())
            window.draw(*(data->GetText()));

        if (m_menuState == MenuState::Inventory)
            m_player->DrawInventoryItems(window, m_itemCfg);

        for (const auto &data1 : m_buttons)
        {
            bool showBtn = false;
            auto menuStates = data1->GetMenuState();

            if (m_menuState == menuStates)
            {
                window.draw(*(data1->GetSprite()));
                window.draw(*(data1->GetText()));
            }
        }
    }
}

// GUI
bool Game::HandleBtnClicked(sf::RenderWindow &window, Game &game)
{
    // get the current mouse position in the window
    auto pixelPos = sf::Mouse::getPosition(window);
    // convert it to world coordinates
    auto worldPos = window.mapPixelToCoords(pixelPos);
    bool breakLoop = false;

    for (const auto &data : m_buttons)
    {
        auto btnPos = data->GetSprite()->getPosition();
        auto btnLSize = data->GetSprite()->getLocalBounds().getSize();
        auto btnScale = data->GetSprite()->getScale();
        auto state = data->GetMenuState();
        bool created = false;

        // ADD SELECT LOAD FILE AND TAKE THE FOLDER ID AND REPLACE ID WITH IT -> btnFnc::Load
        uint8_t id = 1;

        if (worldPos.x > btnPos.x && worldPos.x < btnPos.x + (btnLSize.x * btnScale.x) && worldPos.y > btnPos.y &&
            worldPos.y < btnPos.y + (btnLSize.y * btnScale.y) && m_menuState == state)
        {
            switch (data->GetBtnFnc())
            {
            case BtnFunc::Play:
                // Player Init
                created = game.CreatePlayer();
                if (created)
                {
                    cout << "Player Init Done!" << endl;

                    CreateFolder(m_player->GetID());

                    // Thread Init
                    m_thread = new Thread(window, m_player, game);
                    cout << "Thread Init Done!" << endl;

                    m_playing = true;
                    m_menuState = MenuState::Playing;
                }
                breakLoop = true;
                break;
            case BtnFunc::Resume:
                m_menuState = MenuState::Playing;
                breakLoop = true;
                break;
            case BtnFunc::Quit:
                Quit(window);
                break;
            case BtnFunc::Options:
                m_menuState = MenuState::Options;
                breakLoop = true;
                break;
            case BtnFunc::Back:
                if (m_playing)
                {
                    if (m_menuState == MenuState::Inventory)
                        m_menuState = MenuState::Playing;
                    else
                        m_menuState = MenuState::Pause;
                }
                else
                    m_menuState = MenuState::Main;

                breakLoop = true;
                break;
            case BtnFunc::Resolution:
                m_windowWidth = 1920U;
                m_windowHeight = 1080U;
                ResizeWindow(window);
                breakLoop = true;
                break;
            case BtnFunc::Fullscreen:
                breakLoop = true;
                break;
            case BtnFunc::Save:
                m_menuState = MenuState::Save;
                m_player->Save();
                breakLoop = true;
                break;
            case BtnFunc::OpenLoad:
                m_menuState = MenuState::OpenLoad;

                breakLoop = true;
                break;
            case BtnFunc::Create:
                m_menuState = MenuState::Create;
                breakLoop = true;
                break;
            case BtnFunc::Load:
                m_menuState = MenuState::Load;

                created = game.LoadPlayer(id);
                if (created)
                {
                    cout << "Player Init Done!" << endl;
                    m_player->Load(id);
                    cout << "Player Load Done!" << endl;

                    // Thread Init
                    m_thread = new Thread(window, m_player, game);
                    cout << "Thread Init Done!" << endl;

                    m_playing = true;
                    m_menuState = MenuState::Playing;
                }
                breakLoop = true;
                break;
            default:
                break;
            }
            break;
        }
    }
    return breakLoop;
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

        cout << "ButtonSize: " << m_buttons.size() << endl;

        for (const auto &data2 : m_buttons)
        {
            cout << "Button: " << data2 << endl;
            auto btnState = data2->GetMenuState();

            if (titleState == btnState)
            {
                cout << "Resize Button" << endl;
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

void Game::CreateFolder(const uint8_t id)
{
    auto new_directory_path = filesystem::current_path();
    new_directory_path /= format("save/{}", id);

    if (!filesystem::exists(new_directory_path))
    {
        filesystem::create_directory(new_directory_path);
    }
}

uint8_t Game::CountFolders()
{
    auto directory_path = filesystem::current_path();
    directory_path /= format("save");

    if (filesystem::exists(directory_path))
    {
        int8_t count = distance(filesystem::directory_iterator(directory_path), filesystem::directory_iterator{});

        return count + 1;
    }
    return 0;
}

// QUIT
void Game::Quit(sf::RenderWindow &window)
{
    window.close();
}
