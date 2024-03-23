#include "Game.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <random>

using json = nlohmann::json;

Game::Game(const std::uint16_t windowWidth, const std::uint16_t windowHeight)
    : m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
    m_menuState = MenuState::Main;
    m_maxZoom = 3U;
    m_playing = false;

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

bool Game::GetPlaying() const
{
    return m_playing;
}

void Game::SetMenuState(const MenuState menuState)
{
    m_menuState = menuState;
}

std::uint8_t Game::GetMaxZoom() const
{
    return m_maxZoom;
}

std::uint8_t Game::GetZoom() const
{
    return m_zoom;
}

void Game::SetZoom(const std::uint8_t zoom)
{
    m_zoom += zoom;
}

void Game::SetZoom(const std::uint8_t zoom, const float zoomLevel)
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

std::uint16_t Game::GetWindowWidth() const
{
    return m_windowWidth;
}

std::uint16_t Game::GetWindowHeight() const
{
    return m_windowHeight;
}

std::uint32_t Game::GetMaxTiles() const
{
    return m_maxTiles;
}

std::uint8_t Game::GetTileSize() const
{
    return m_tileSize;
}

std::uint16_t Game::GetGameWidth() const
{
    return m_gameWidth;
}
std::uint16_t Game::GetGameHeight() const
{
    return m_gameHeight;
}

std::vector<World *> Game::GetWorld() const
{
    return m_world;
}

std::vector<ItemCfg *> Game::GetItemCfg() const
{
    return m_itemCfg;
}

std::vector<Item *> Game::GetItem() const
{
    return m_items;
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

void Game::SetWindowHeight(const std::uint16_t height)
{
    m_windowHeight = height;
}
void Game::SetWindowWidth(const std::uint16_t width)
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
    std::ifstream file("./data/itemCfg.json");

    std::vector<nlohmann::json_abi_v3_11_2::ordered_json> jsonItems;

    if (file.is_open())
    {
        auto jsonData = nlohmann::ordered_json::parse(file);

        for (const auto &data : jsonData)
        {
            std::uint8_t ID = data["id"];
            std::string name = data["name"];
            std::uint8_t maxDrop = data["maxDrop"];
            sf::Texture *texture;
            std::uint8_t textureID = data["textureID"];
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
    std::ifstream file("./data/textureCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            auto texture = new sf::Texture();
            std::uint8_t ID = data["id"];
            std::string path = data["path"];

            texture->loadFromFile(path);

            auto textures = new Texture(ID, texture);
            m_textures.push_back(textures);
        }
        file.close();
    }
}

void Game::InitFont()
{
    std::ifstream file("./data/fontCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (const auto &data : jsonData)
        {
            auto font = new sf::Font();
            std::uint8_t ID = data["id"];
            std::string path = data["path"];

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
        if (texID == 2)
        {
            texture = data->GetTexture();
        }
    }

    sprite->setTexture(*texture);
    sprite->setTextureRect(sf::IntRect(8U, 8U, 16U, 16U));
    sprite->setPosition(80.0F, 80.0F);

    auto player = Player(sprite, "PlayerName", PlayerSurvivalStats{100.0F, 100.0F, 100.0F}, 1.0F);
    return player;
}

void Game::InitMenu()
{
    sf::Sprite prevBtn;

    std::ifstream file("./data/menus.json");

    std::vector<nlohmann::json_abi_v3_11_2::ordered_json> menuTitles;
    std::vector<nlohmann::json_abi_v3_11_2::ordered_json> menuButtons;

    if (file.is_open())
    {
        auto jsonData = nlohmann::ordered_json::parse(file);

        for (auto it = jsonData.begin(); it != jsonData.end(); ++it)
        {
            if (it.key() == "Titles")
                ProcessJSON(*it, menuTitles);

            if (it.key() == "Buttons")
                ProcessJSON(*it, menuButtons);
        }

        for (const auto &data : menuTitles)
        {
            sf::Text titleText;
            sf::Text btnText;
            sf::Sprite btn;

            bool firstButton = true;
            MenuState state = data["state"];
            std::uint8_t fontSize = data["fontSize"];
            std::string text = data["name"];
            std::uint8_t fontID = data["fontID"];
            sf::Font *font;

            for (const auto &data5 : m_fonts)
            {
                auto fonID = data5->GetID();
                if (fonID == fontID)
                {
                    font = data5->GetFont();
                }
            }

            titleText.setFont(*font);
            titleText.setCharacterSize(fontSize);
            titleText.setString(text);

            Menu::SetTitlePos(m_windowWidth, titleText);

            auto titles = new Title(state, titleText);
            m_titles.push_back(titles);

            for (const auto &data1 : menuButtons)
            {
                bool addBtn = false;
                for (const auto &data2 : data1["state"])
                {
                    if (state == data2)
                    {
                        addBtn = true;
                        break;
                    }
                }

                if (addBtn)
                {
                    auto textureRect = sf::IntRect{data1["textureData"][0],
                                                   data1["textureData"][1],
                                                   data1["textureData"][2],
                                                   data1["textureData"][3]};
                    auto scale = sf::Vector2f{data1["scale"][0], data1["scale"][1]};
                    text = data1["name"];
                    fontSize = data1["fontSize"];
                    std::vector<MenuState> state = data1["state"];
                    BtnFunc btnFnc = data1["fnc"];
                    std::uint8_t textureID = data1["textureID"];
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

                    btn.setTexture(*texture);
                    btn.setTextureRect(textureRect);
                    btn.setScale(scale);

                    btnText.setFont(*font);
                    btnText.setCharacterSize(fontSize);
                    btnText.setString(text);

                    if (firstButton)
                        Menu::SetBtnAndTextPos(m_windowWidth, btn, titleText, btnText);
                    else
                        Menu::SetBtnAndTextPos(m_windowWidth, btn, prevBtn, btnText);

                    prevBtn = btn;
                    firstButton = false;

                    auto buttons = new Button(state, btnFnc, btnText, btn);
                    m_buttons.push_back(buttons);
                }
            }
        }

        file.close();
    }
    else
    {
    }
}

void Game::InitSurface()
{
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

    // Random Grass Texture Selector
    std::uniform_int_distribution<> dist(0U, 3U);

    size_t j = 0;
    size_t k = 0;

    std::ifstream file("./data/surfaceCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        for (size_t i = 0; i < m_maxTiles; ++i)
        {
            sf::Sprite tileSprite;
            sf::Texture *texture;
            std::uint8_t id;
            std::uint8_t textureID;
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
                {
                    // FULL WATER
                    canCreate = true;
                }
                else if (id == 1 && j == 0)
                {
                    // LEFT WATER
                    canCreate = true;
                }
                else if (id == 2 && j * m_tileSize == (m_gameWidth - m_tileSize))
                {
                    // RIGHT WATER
                    canCreate = true;
                }
                else if (id == 3 && k == 0)
                {
                    // TOP WATER
                    canCreate = true;
                }
                else if (id == 4 && k * m_tileSize == (m_gameHeight - m_tileSize))
                {
                    // BOT WATER
                    canCreate = true;
                }
                else if (id == 5 && rnd == 0)
                {
                    // RND GRASS
                    canCreate = true;
                }
                else if (id == 6 && rnd == 1)
                {
                    // RND GRASS
                    canCreate = true;
                }
                else if (id == 7 && rnd == 2)
                {
                    // RND GRASS
                    canCreate = true;
                }
                else if (id == 8 && rnd == 3)
                {
                    // RND GRASS
                    canCreate = true;
                }

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

void Game::InitWorld()
{

    sf::Sprite tileSprite;

    std::ifstream file("./data/worldCfg.json");

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        sf::Texture *texture;
        for (const auto &data : jsonData)
        {
            auto collision = Collision{.x = data["textureData"][4], .y = data["textureData"][5]};
            std::uint8_t itemOutputID = data["itemOutputID"];
            auto textureData = sf::IntRect(data["textureData"][0],
                                           data["textureData"][1],
                                           data["textureData"][2],
                                           data["textureData"][3]);
            auto textureProgData = TextureProgData{
                .rect = sf::IntRect{data["textureProgData"][0],
                                    data["textureProgData"][1],
                                    data["textureProgData"][2],
                                    data["textureProgData"][3]},
                .collision = Collision{.x = data["textureProgData"][4], .y = data["textureProgData"][5]}};

            std::uint8_t textureID = data["textureID"];

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
        auto btnPos = data->GetSprite().getPosition();
        auto btnLSize = data->GetSprite().getLocalBounds().getSize();
        auto btnScale = data->GetSprite().getScale();
        auto data1 = data->GetMenuState();

        bool clickBtn = false;
        for (const auto &data2 : data1)
        {
            if (m_menuState == data2)
            {
                clickBtn = true;
                break;
            }
        }

        if (worldPos.x > btnPos.x && worldPos.x < btnPos.x + (btnLSize.x * btnScale.x) && worldPos.y > btnPos.y &&
            worldPos.y < btnPos.y + (btnLSize.y * btnScale.y) && clickBtn)
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
                window.close();
                breakLoop = true;
                break;
            case BtnFunc::Options:
                m_menuState = MenuState::Options;
                /*
                    TODO: ADD OPTIONS TO OPTION MENU
                */
                // game.SetWindowWidth(1920U);
                // game.SetWindowHeight(1080U);
                // windowWidth = game.GetWindowWidth();
                // windowHeight = game.GetWindowHeight();
                // window.setSize(sf::Vector2u(windowWidth, windowHeight));
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
            window.draw(data->GetText());

        for (const auto &data1 : m_buttons)
        {
            bool showBtn = false;
            auto menuStates = data1->GetMenuState();
            for (const auto &data3 : menuStates)
            {
                if (m_menuState == data3)
                {
                    showBtn = true;
                    break;
                }
            }

            if (showBtn)
            {
                window.draw(data1->GetSprite());
                window.draw(data1->GetText());
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
            window.draw(data->GetText());

        if (m_menuState == MenuState::Inventory)
            player.DrawInventoryItems(window, m_itemCfg);

        for (const auto &data1 : m_buttons)
        {
            bool showBtn = false;
            auto menuStates = data1->GetMenuState();
            for (const auto &data3 : menuStates)
            {
                if (m_menuState == data3)
                {
                    showBtn = true;
                    break;
                }
            }

            if (showBtn)
            {
                window.draw(data1->GetSprite());
                window.draw(data1->GetText());
            }
        }
    }
}
