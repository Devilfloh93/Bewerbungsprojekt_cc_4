#include "Player.h"
#include "Utilities.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>

Player::Player(sf::Sprite *sprite, const uint8_t animID) : Sprite(sprite), m_animID(animID)
{
    m_sprite->setPosition(80.0F, 80.0F);
    m_survivalStats = {.health = 100.0F, .water = 100.0F, .food = 100.0F};
    m_baseSpeed = 1.0F;
    m_speed = m_baseSpeed;
    m_move = PlayerMove::NotMoving;
    m_lastMove = m_move;
    m_objectInFront = nullptr;
}

PlayerMove Player::GetMove() const
{
    return m_move;
}

PlayerMove Player::GetLastMove() const
{
    return m_lastMove;
}

void Player::SetMove(const PlayerMove move)
{
    m_lastMove = m_move;
    m_move = move;
}

void Player::SetSpeed(const float speed)
{
    m_speed = speed;
}

void Player::Load()
{
    vector<string> saves;
    map<uint32_t, uint16_t> savedItems;

    string str;
    ifstream file("./save/save.txt");

    if (file.is_open())
    {
        while (getline(file, str))
        {
            bool push = true;
            if (str.find('|') != str.npos)
            {
                auto pos = str.find('|');
                str.replace(pos, 1, "  ");

                istringstream iss(str);
                uint32_t column1;
                uint16_t column2;
                iss >> column1 >> column2;
                savedItems[column1] = column2;
                push = false;
            }

            if (push)
            {
                saves.push_back(str);
            }
        }

        m_name = saves[0];
        m_survivalStats.health = stof(saves[1]);
        m_survivalStats.water = stof(saves[2]);
        m_survivalStats.food = stof(saves[3]);
        m_sprite->setPosition(stof(saves[4]), stof(saves[5]));
        m_items = savedItems;

        file.close();
    }
}

void Player::Save()
{
    ofstream file("./save/save.txt");

    if (file.is_open())
    {
        auto text = format("{}\n{}\n{}\n{}\n{}\n{}\n",
                           m_name,
                           m_survivalStats.health,
                           m_survivalStats.water,
                           m_survivalStats.food,
                           m_sprite->getPosition().x,
                           m_sprite->getPosition().y);
        file << text;

        for (const auto &data : m_items)
        {
            file << data.first << '|' << data.second << '\n';
        }

        file.close();
    }
}

void Player::CreateFolder()
{
    auto new_directory_path = filesystem::current_path();
    new_directory_path /= "save";

    if (!filesystem::exists(new_directory_path))
    {
        filesystem::create_directory(new_directory_path);
    }
}


void Player::CheckCollision(Game &game)
{
    auto world = game.GetWorld();
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();

    for (const auto &data : world)
    {
        auto objPos = data->GetSprite()->getPosition();
        auto objCollision = data->GetCollision();
        auto objSize = data->GetSprite()->getLocalBounds().getSize();
        auto isUsable = data->GetUseable();

        if (objCollision.x != 0 && objCollision.y != 0)
        {
            if (playerPos.x >= objPos.x && playerPos.x <= objPos.x + objCollision.x &&
                playerPos.y - m_speed <= objPos.y + objSize.y && playerPos.y >= objPos.y + objCollision.y)
            {
                m_moveAllowed.up = false;
                if (isUsable && m_move == PlayerMove::Up)
                {
                    m_objectInFront = data;
                }
            }

            if (playerPos.x >= objPos.x && playerPos.x <= objPos.x + objCollision.x &&
                playerPos.y <= objPos.y + objCollision.y && playerPos.y + m_speed >= objPos.y + objCollision.y)
            {
                m_moveAllowed.down = false;
                if (isUsable && m_move == PlayerMove::Down)
                {
                    m_objectInFront = data;
                }
            }

            if (playerPos.x >= objPos.x && playerPos.x - m_speed <= objPos.x + objCollision.x &&
                playerPos.y <= objPos.y + objSize.y && playerPos.y >= objPos.y + objCollision.y)
            {
                m_moveAllowed.left = false;
                if (isUsable && m_move == PlayerMove::Left)
                {
                    m_objectInFront = data;
                }
            }


            if (playerPos.x + m_speed >= objPos.x && playerPos.x <= objPos.x && playerPos.y <= objPos.y + objSize.y &&
                playerPos.y >= objPos.y + objCollision.y)
            {
                m_moveAllowed.right = false;
                if (isUsable && m_move == PlayerMove::Right)
                {
                    m_objectInFront = data;
                }
            }
        }
        else
        {
            if ((playerPos.x + playerSize.x) >= objPos.x && playerPos.x <= objPos.x + objSize.x &&
                playerPos.y - m_speed <= objPos.y + objSize.y && playerPos.y + playerSize.y >= objPos.y)
            {
                m_moveAllowed.up = false;
                if (isUsable && m_move == PlayerMove::Up)
                {
                    m_objectInFront = data;
                }
            }

            if ((playerPos.x + playerSize.x) >= objPos.x && playerPos.x <= objPos.x + objSize.x &&
                playerPos.y <= objPos.y + objSize.y && (playerPos.y + playerSize.y) + m_speed >= objPos.y)
            {
                m_moveAllowed.down = false;
                if (isUsable && m_move == PlayerMove::Down)
                {
                    m_objectInFront = data;
                }
            }

            if (playerPos.x >= objPos.x && playerPos.x - m_speed <= objPos.x + objSize.x &&
                playerPos.y <= objPos.y + objSize.y && playerPos.y + playerSize.y >= objPos.y)
            {
                m_moveAllowed.left = false;
                if (isUsable && m_move == PlayerMove::Left)
                {
                    m_objectInFront = data;
                }
            }

            if ((playerPos.x + playerSize.x) + m_speed >= objPos.x && playerPos.x <= objPos.x &&
                playerPos.y <= objPos.y + objSize.y && playerPos.y + playerSize.y >= objPos.y)
            {
                m_moveAllowed.right = false;
                if (isUsable && m_move == PlayerMove::Right)
                {
                    m_objectInFront = data;
                }
            }
        }
    }
}

void Player::UpdateStats(const sf::RenderWindow &window, const Game &game)
{
    sf::Clock clock;
    auto statDecay = game.GetStatDecay();
    while (window.isOpen())
    {
        if (game.GetPlaying() && game.GetMenuState() == MenuState::Playing)
        {
            auto elapsed = clock.getElapsedTime();
            if (elapsed.asSeconds() >= 1)
            {
                if (m_survivalStats.food - statDecay.food >= 0U)
                {
                    m_survivalStats.food -= statDecay.food;
                }
                if (m_survivalStats.water - statDecay.water >= 0U)
                {
                    m_survivalStats.water -= statDecay.water;
                }

                cout << format("Food: {} Water: {} Health: {}",
                               m_survivalStats.food,
                               m_survivalStats.water,
                               m_survivalStats.health)
                     << endl;
                clock.restart();
            }
        }
    }
}

void Player::InitDrawStats(const Game &game)
{
    ifstream file("./data/playerStatCfg.json");

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
            bool background = data["background"];

            for (const auto &data : game.GetTexture())
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

                if (background)
                {
                    m_statBackgroundSprites.push_back(sprite);
                }
                else
                {
                    m_statSprites.push_back(sprite);
                }
            }
        }
        file.close();
    }
}

void Player::DrawStats(sf::RenderWindow &window, const Game &game)
{
    m_stateTextureSize = 66;
    sf::Sprite sprite;
    auto height = (game.GetView().getCenter().y + (game.GetWindowZoomHeight() / 2)) - 10.0F;
    auto width = (game.GetView().getCenter().x - (game.GetWindowZoomWidth() / 2)) + 5.0F;

    for (const auto &data : m_statBackgroundSprites)
    {
        data->setPosition(sf::Vector2f{width, height});

        window.draw(*data);

        height -= 10;
    }

    height = (game.GetView().getCenter().y + (game.GetWindowZoomHeight() / 2)) - 10.0F;
    for (const auto &data : m_statSprites)
    {
        data->setPosition(sf::Vector2f{width, height});
        auto textureRectSize = data->getTextureRect().getSize();
        auto textureRectPos = data->getTextureRect().getPosition();

        uint8_t newTextureRectSizeX = (m_survivalStats.food * m_stateTextureSize) / 100;
        if (textureRectSize.x > 10)
        {
            auto textureRect = sf::IntRect{textureRectPos.x, textureRectPos.y, newTextureRectSizeX, textureRectSize.y};
            data->setTextureRect(textureRect);
        }

        window.draw(*data);

        height -= 10;
    }
}

void Player::DrawInventoryItems(sf::RenderWindow &window, const vector<ItemCfg *> &itemCfg)
{
    bool firstIcon = true;

    Utilities utilities;
    sf::Sprite prevSprite;

    sf::Font font;
    font.loadFromFile("ressources/font/Round9x13.ttf");

    for (size_t i = 0; const auto &[key, value] : m_items)
    {
        sf::Sprite itemSprite;
        sf::Text itemText;
        for (const auto &data : itemCfg)
        {
            auto texture = data->GetTexture();
            auto textureData = data->GetTextureData();
            auto ID = data->GetID();
            if (key == ID)
            {
                itemText.setFont(font);
                itemText.setCharacterSize(15);
                itemText.setString(format("{}", value));

                itemSprite.setTexture(*texture);
                itemSprite.setTextureRect(textureData);

                if (firstIcon)
                {
                    utilities.SetTextBeforeIcon(300, 200, itemSprite, itemText);
                    prevSprite = itemSprite;
                    firstIcon = false;
                }
                else
                {
                    utilities.SetTextBeforeIcon(itemSprite, itemText, prevSprite);
                }

                break;
            }
        }
        window.draw(itemText);
        window.draw(itemSprite);
    }
}

void Player::HandleMove(sf::Clock &clock, Game &game)
{
    auto items = game.GetItem();
    auto anim = game.GetAnim();
    auto elapsed = clock.getElapsedTime();
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();
    auto tileSize = game.GetTileSize();
    auto width = game.GetGameWidth();
    auto height = game.GetGameHeight();
    MovementTexture moveAnim;

    for (const auto &data : anim)
    {
        if (data->GetTextureID() == m_animID)
        {
            moveAnim = data->GetMoveAnim();
            break;
        }
    }

    if (!items.empty())
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            auto sprite = items[i]->GetSprite();
            auto ID = items[i]->GetID();
            auto count = items[i]->GetCount();
            auto itemPos = sprite->getPosition();
            auto itemSize = sprite->getLocalBounds().getSize();

            bool remove = false;
            bool newItem = true;

            if ((playerPos.x + (playerSize.x / 2)) >= itemPos.x && playerPos.x <= itemPos.x + (itemSize.x / 2) &&
                playerPos.y + (playerSize.y / 2) >= itemPos.y && playerPos.y <= itemPos.y + (itemSize.y / 2))
            {
                for (const auto &[key, value] : m_items)
                {
                    if (key == ID)
                    {
                        m_items[key] = value + count;
                        newItem = false;
                    }
                }

                if (newItem)
                {
                    m_items[ID] = count;
                }
                remove = true;
            }

            if (remove)
            {
                game.RemoveItems(i);
                break;
            }
        }
    }


    switch (m_move)
    {
    case PlayerMove::Left:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveAnim.left01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveAnim.left00);
        else
            clock.restart();

        if (playerPos.x - m_speed > 0 + (tileSize / 2) && m_moveAllowed.left)
            m_sprite->setPosition(playerPos.x - m_speed, playerPos.y);
        else
            m_sprite->setTextureRect(moveAnim.left00);
        break;
    case PlayerMove::Right:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveAnim.right01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveAnim.right00);
        else
            clock.restart();

        if (playerPos.x + m_speed < width - tileSize && m_moveAllowed.right)
            m_sprite->setPosition(playerPos.x + m_speed, playerPos.y);
        else
            m_sprite->setTextureRect(moveAnim.right00);
        break;
    case PlayerMove::Down:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveAnim.down01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveAnim.down02);
        else
            clock.restart();

        if (playerPos.y + m_speed < height - tileSize && m_moveAllowed.down)
            m_sprite->setPosition(playerPos.x, playerPos.y + m_speed);
        else
            m_sprite->setTextureRect(moveAnim.down00);

        break;
    case PlayerMove::Up:
        if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
            m_sprite->setTextureRect(moveAnim.up01);
        else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
            m_sprite->setTextureRect(moveAnim.up02);
        else
            clock.restart();

        if (playerPos.y - m_speed > 0 + (tileSize / 2) && m_moveAllowed.up)
            m_sprite->setPosition(playerPos.x, playerPos.y - m_speed);
        else
            m_sprite->setTextureRect(moveAnim.up00);

        break;
    case PlayerMove::NotMoving:
        switch (m_lastMove)
        {
        case PlayerMove::Down:
            m_sprite->setTextureRect(moveAnim.down00);
            break;
        case PlayerMove::Up:
            m_sprite->setTextureRect(moveAnim.up00);
            break;
        case PlayerMove::Left:
            m_sprite->setTextureRect(moveAnim.left00);
            break;
        case PlayerMove::Right:
            m_sprite->setTextureRect(moveAnim.right00);
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    m_moveAllowed.up = true;
    m_moveAllowed.down = true;
    m_moveAllowed.left = true;
    m_moveAllowed.right = true;
}

void Player::UseItem(Game &game)
{
    auto itemCfg = game.GetItemCfg();
    auto playerPos = m_sprite->getPosition();
    sf::Vector2f itemPos = {0.0F, 0.0F};

    if (m_objectInFront != nullptr)
    {
        auto useable = m_objectInFront->GetUseable();

        if (useable)
        {
            auto itemOutputID = m_objectInFront->GetItemOutputID();


            m_objectInFront->UpdatePosition();
            m_objectInFront->UpdateTextureRect();
            m_objectInFront->SetUseable(false);

            random_device rd;  // a seed source for the random number engine
            mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
            uint16_t posIncrease = 0;
            for (const auto &data : itemCfg)
            {
                auto texture = data->GetTexture();
                auto textureData = data->GetTextureData();
                auto itemID = data->GetID();
                auto maxDrop = data->GetMaxDrop();

                // Random Drop Count
                uniform_int_distribution<> dist(1U, maxDrop);

                for (const auto &data1 : itemOutputID)
                {
                    if (data1 == itemID)
                    {
                        posIncrease += 20;
                        switch (m_move)
                        {
                        case PlayerMove::Up:
                            itemPos = {playerPos.x, playerPos.y + posIncrease};
                            break;
                        case PlayerMove::Down:
                            itemPos = {playerPos.x, playerPos.y - posIncrease};
                            break;
                        case PlayerMove::Right:
                            itemPos = {playerPos.x - posIncrease, playerPos.y};
                            break;
                        case PlayerMove::Left:
                            itemPos = {playerPos.x + posIncrease, playerPos.y};
                            break;
                        case PlayerMove::NotMoving:
                            switch (m_lastMove)
                            {
                            case PlayerMove::Up:
                                itemPos = {playerPos.x, playerPos.y + posIncrease};
                                break;
                            case PlayerMove::Down:
                                itemPos = {playerPos.x, playerPos.y - posIncrease};
                                break;
                            case PlayerMove::Right:
                                itemPos = {playerPos.x - posIncrease, playerPos.y};
                                break;
                            case PlayerMove::Left:
                                itemPos = {playerPos.x + posIncrease, playerPos.y};
                                break;
                            default:
                                break;
                            }
                            break;

                        default:
                            break;
                        }

                        auto itemSprite = new sf::Sprite();
                        itemSprite->setTexture(*texture);
                        itemSprite->setTextureRect(textureData);
                        itemSprite->setPosition(itemPos.x, itemPos.y);

                        auto item = new Item(itemSprite, itemID, dist(gen));
                        game.SetItems(item);
                    }
                }
            }
        }
    }
}
