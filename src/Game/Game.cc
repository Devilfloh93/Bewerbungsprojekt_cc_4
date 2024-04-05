#include "Game.h"
#include "Utilities.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <random>

using json = nlohmann::json;
using namespace std;

Game::Game(const uint16_t windowWidth, const uint16_t windowHeight)
    : m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
    m_menuState = MenuState::Main;
    m_maxZoom = 3U;
    m_playing = false;
    m_defaultPlayerTextureID = 2U;
    m_statDecay = {.food = 1.2F, .water = 1.7F};
    m_gameWidth = 8800U;
    m_gameHeight = 4800U;
    m_tileSize = 32U;
    m_maxTiles = ((m_gameWidth * m_gameHeight) / m_tileSize) / m_tileSize;
}

MenuState Game::GetMenuState() const
{
    return m_menuState;
}

void Game::SetPlaying(const bool playing)
{
    m_playing = playing;
}

StatDecay Game::GetStatDecay() const
{
    return m_statDecay;
}

bool Game::GetPlaying() const
{
    return m_playing;
}

void Game::SetMenuState(const MenuState menuState)
{
    m_menuState = menuState;
}

uint8_t Game::GetMaxZoom() const
{
    return m_maxZoom;
}

uint8_t Game::GetZoom() const
{
    return m_zoom;
}

void Game::SetZoom(const uint8_t zoom)
{
    m_zoom += zoom;
}

void Game::SetZoom(const uint8_t zoom, const float zoomLevel)
{
    m_view.zoom(zoomLevel);
    m_zoom += zoom;
}

sf::View Game::GetView() const
{
    return m_view;
}

sf::View Game::GetMenuView() const
{
    return m_menuView;
}

uint16_t Game::GetWindowWidth() const
{
    return m_windowWidth;
}

uint16_t Game::GetWindowHeight() const
{
    return m_windowHeight;
}

uint32_t Game::GetMaxTiles() const
{
    return m_maxTiles;
}

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

vector<Anim *> Game::GetAnim() const
{
    return m_anim;
}

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

void Game::Quit(sf::RenderWindow &window)
{
    window.close();
}

void Game::SetWindowHeight(const uint16_t height)
{
    m_windowHeight = height;
}
void Game::SetWindowWidth(const uint16_t width)
{
    m_windowWidth = width;
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

    m_view.move({-(center.x / 2), -(center.y / 2)});
}

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
        center = m_view.getCenter();
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

void Game::InitItemCfg()
{
    ifstream file("./data/itemCfg.json");

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

void Game::DrawItems(sf::RenderWindow &window)
{
    for (const auto &data : m_items)
    {
        auto sprite = data->GetSprite();

        window.draw(sprite);
    }
}


void Game::InitTexture()
{
    ifstream file("./data/textureCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            auto texture = new sf::Texture();
            uint8_t ID = data["id"];
            string path = data["path"];

            texture->loadFromFile(path);

            auto textures = new Texture(ID, texture);
            m_textures.push_back(textures);
        }
        file.close();
    }
}

void Game::InitFont()
{
    ifstream file("./data/fontCfg.json");

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

Player Game::InitPlayer()
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

    sprite->setPosition(80.0F, 80.0F);

    auto player = Player(sprite,
                         "PlayerName",
                         SurvivalStats{.health = 100.0F, .water = 100.0F, .food = 100.0F},
                         1.0F,
                         m_defaultPlayerTextureID);
    return player;
}

void Game::InitMenu()
{
    Utilities utilities;
    sf::Sprite *prevBtn;

    ifstream file("./data/menuCfg.json");

    vector<nlohmann::json_abi_v3_11_2::ordered_json> menuTitles;
    vector<nlohmann::json_abi_v3_11_2::ordered_json> menuButtons;

    if (file.is_open())
    {
        auto jsonData = nlohmann::ordered_json::parse(file);

        for (auto it = jsonData.begin(); it != jsonData.end(); ++it)
        {
            if (it.key() == "Titles")
                utilities.ProcessJSON(*it, menuTitles);

            if (it.key() == "Buttons")
                utilities.ProcessJSON(*it, menuButtons);
        }

        for (const auto &data : menuTitles)
        {
            auto titleText = new sf::Text();

            bool firstBtn = true;
            MenuState state = data["state"];
            uint8_t fontSize = data["fontSize"];
            string text = data["name"];
            uint8_t fontID = data["fontID"];
            sf::Font *font;

            for (const auto &data5 : m_fonts)
            {
                auto fonID = data5->GetID();
                if (fonID == fontID)
                {
                    font = data5->GetFont();
                }
            }

            titleText->setFont(*font);
            titleText->setCharacterSize(fontSize);
            titleText->setString(text);

            utilities.SetTitlePos(m_windowWidth, titleText);

            auto titles = new Title(state, titleText);
            m_titles.push_back(titles);

            for (const auto &data1 : menuButtons)
            {
                bool addBtn = false;
                vector<MenuState> btnState = data1["state"];
                for (const auto &data2 : btnState)
                {
                    if (state == data2)
                    {
                        addBtn = true;
                        break;
                    }
                }

                if (addBtn)
                {
                    auto btnText = new sf::Text();
                    auto btn = new sf::Sprite();

                    auto textureRect = sf::IntRect{data1["textureData"][0],
                                                   data1["textureData"][1],
                                                   data1["textureData"][2],
                                                   data1["textureData"][3]};
                    auto scale = sf::Vector2f{data1["scale"][0], data1["scale"][1]};
                    text = data1["name"];
                    fontSize = data1["fontSize"];
                    BtnFunc btnFnc = data1["fnc"];
                    uint8_t textureID = data1["textureID"];
                    fontID = data1["fontID"];
                    sf::Texture *texture;

                    for (const auto &data3 : m_textures)
                    {
                        auto texID = data3->GetID();
                        if (texID == textureID)
                        {
                            texture = data3->GetTexture();
                        }
                    }

                    for (const auto &data4 : m_fonts)
                    {
                        auto fonID = data4->GetID();
                        if (fonID == fontID)
                        {
                            font = data4->GetFont();
                        }
                    }

                    btn->setTexture(*texture);
                    btn->setTextureRect(textureRect);
                    btn->setScale(scale);

                    btnText->setFont(*font);
                    btnText->setCharacterSize(fontSize);
                    btnText->setString(text);

                    if (firstBtn)
                        utilities.SetBtnAndTextPos(m_windowWidth, btn, titleText, btnText);
                    else
                        utilities.SetBtnAndTextPos(m_windowWidth, btn, prevBtn, btnText);

                    prevBtn = btn;
                    firstBtn = false;

                    auto buttons = new Button(state, btnFnc, btnText, btn);
                    m_buttons.push_back(buttons);
                }
            }
        }

        file.close();
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

    ifstream file("./data/surfaceCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (size_t i = 0; i < m_maxTiles; ++i)
        {
            sf::Sprite tileSprite;
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
                    tileSprite.setTextureRect(textureData);
                    tileSprite.setTexture(*texture);
                    tileSprite.setPosition(j * m_tileSize, k * m_tileSize);

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
    ifstream file("./data/characterAnimCfg.json");

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

    sf::Sprite tileSprite;

    ifstream file("./data/worldCfg.json");

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

            tileSprite.setTexture(*texture);
            tileSprite.setTextureRect(textureData);

            for (const auto &data1 : data["pos"])
            {
                tileSprite.setPosition(data1[0], data1[1]);

                auto world = new World(tileSprite, collision, itemOutputID, textureProgData);
                m_world.push_back(world);
            }
        }
        file.close();
    }
}

bool Game::HandleBtnClicked(sf::RenderWindow &window)
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

        if (worldPos.x > btnPos.x && worldPos.x < btnPos.x + (btnLSize.x * btnScale.x) && worldPos.y > btnPos.y &&
            worldPos.y < btnPos.y + (btnLSize.y * btnScale.y) && m_menuState == state)
        {
            switch (data->GetBtnFnc())
            {
            case BtnFunc::Play:
                m_playing = true;
                m_menuState = MenuState::Playing;
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
            default:
                break;
            }
            break;
        }
    }
    return breakLoop;
}

void Game::DrawSurface(sf::RenderWindow &window, Player &player)
{
    auto playerSprite = player.GetSprite();
    auto playerPos = playerSprite->getPosition();

    for (auto &data : m_surfaces)
    {
        auto sprite = data->GetSprite();
        auto spritePos = sprite.getPosition();
        auto tileSize = m_tileSize / 2;

        if (playerPos.x >= spritePos.x - tileSize && playerPos.x <= spritePos.x + tileSize &&
            playerPos.y >= spritePos.y - tileSize && playerPos.y <= spritePos.y + tileSize)
        {
            auto speed = data->GetSpeed();
            player.SetSpeed(speed);
        }

        window.draw(sprite);
    }
}

void Game::DrawWorld(sf::RenderWindow &window)
{
    for (auto &data : m_world)
    {
        auto sprite = data->GetSprite();

        window.draw(sprite);
    }
}

void Game::DrawMenu(sf::RenderWindow &window)
{
    window.setView(m_menuView);

    for (const auto &data : m_titles)
    {
        if (m_menuState == data->GetMenuState())
            window.draw(*(data->GetText()));

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

void Game::DrawMenu(sf::RenderWindow &window, Player &player)
{
    window.setView(m_menuView);

    for (const auto &data : m_titles)
    {
        if (m_menuState == data->GetMenuState())
            window.draw(*(data->GetText()));

        if (m_menuState == MenuState::Inventory)
            player.DrawInventoryItems(window, m_itemCfg);

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
