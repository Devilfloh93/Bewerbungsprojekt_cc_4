#include "Player.h"
#include "Stats.h"
#include "Utilities.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>

Player::Player(sf::Sprite *sprite, const uint8_t animID, const string_view name, const uint8_t id, sf::Text *hotkeyDraw)
    : Unit(sprite, 100.0F, 1.0F, animID), m_name(name), m_ID(id), m_hotkeyDraw(hotkeyDraw)
{
    Init();
}

Player::Player(sf::Sprite *sprite, const uint8_t animID, const uint8_t id, sf::Text *hotkeyDraw)
    : Unit(sprite, 100.0F, 1.0F, animID), m_ID(id), m_hotkeyDraw(hotkeyDraw)
{
    Init();
}

// INIT
void Player::Init()
{
    m_sprite->setPosition(80.0F, 80.0F);
    m_survivalStats = {.water = 100.0F, .food = 100.0F};

    m_move = PlayerMove::NotMoving;
    m_lastMove = m_move;
    m_objectInFront = nullptr;
    m_creatureInFront = nullptr;
}

// INFO
uint8_t Player::GetID() const
{
    return m_ID;
}

// MOVE
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

void Player::CollectItem(Game *game)
{
    auto items = game->GetItem();
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();

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
                    m_items[ID] = count;

                remove = true;
            }

            if (remove)
            {
                game->RemoveItems(i);
                break;
            }
        }
    }
}

void Player::HandleMove(sf::Clock &clock, Game *game)
{
    Utilities utilities;
    MovementTexture moveAnim;
    auto anim = game->GetAnim();
    auto tileSize = game->GetTileSize();
    auto width = game->GetGameWidth();
    auto height = game->GetGameHeight();
    auto playerPos = m_sprite->getPosition();

    for (const auto &data : anim)
    {
        if (data->GetID() == m_animID)
        {
            moveAnim = data->GetMoveAnim();
            break;
        }
    }

    switch (m_move)
    {
    case PlayerMove::Left:
        utilities.PlayAnimation(m_sprite, clock, moveAnim.left00, moveAnim.left01);

        if (playerPos.x - m_speed > 0 + (tileSize / 2) && m_moveAllowed.left)
            m_sprite->setPosition(playerPos.x - m_speed, playerPos.y);
        else
            m_sprite->setTextureRect(moveAnim.left00);
        break;
    case PlayerMove::Right:
        utilities.PlayAnimation(m_sprite, clock, moveAnim.right00, moveAnim.right01);

        if (playerPos.x + m_speed < width - tileSize && m_moveAllowed.right)
            m_sprite->setPosition(playerPos.x + m_speed, playerPos.y);
        else
            m_sprite->setTextureRect(moveAnim.right00);
        break;
    case PlayerMove::Down:
        utilities.PlayAnimation(m_sprite, clock, moveAnim.down02, moveAnim.down01);

        if (playerPos.y + m_speed < height - tileSize && m_moveAllowed.down)
            m_sprite->setPosition(playerPos.x, playerPos.y + m_speed);
        else
            m_sprite->setTextureRect(moveAnim.down00);

        break;
    case PlayerMove::Up:
        utilities.PlayAnimation(m_sprite, clock, moveAnim.up02, moveAnim.up01);

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

    CollectItem(game);

    m_moveAllowed.up = true;
    m_moveAllowed.down = true;
    m_moveAllowed.left = true;
    m_moveAllowed.right = true;
}

// STATS
float Player::GetStatValue(const StatType type) const
{
    float value = 0.0F;
    switch (type)
    {
    case StatType::Health:
        value = m_health;
        break;
    case StatType::Food:
        value = m_survivalStats.food;
        break;
    case StatType::Water:
        value = m_survivalStats.water;
        break;

    default:
        break;
    }

    return value;
}

void Player::UpdateStats(Game *game)
{
    auto statDecay = game->GetStatDecay();

    if (game->GetPlaying() && game->GetMenuState() == MenuState::Playing)
    {
        if (m_survivalStats.food - statDecay.food < 0U)
            m_survivalStats.water = 0;
        else
            m_survivalStats.food -= statDecay.food;

        if (m_survivalStats.water - statDecay.water < 0U)
            m_survivalStats.water = 0;
        else
            m_survivalStats.water -= statDecay.water;
    }
}


// ITEMS
void Player::Interact(Game &game)
{
    auto itemCfg = game.GetItemCfg();
    auto playerPos = m_sprite->getPosition();
    sf::Vector2f itemPos = {0.0F, 0.0F};

    if (m_objectInFront != nullptr)
    {
        auto interactable = m_objectInFront->GetInteractable();

        if (interactable)
        {
            auto itemOutputID = m_objectInFront->GetItemOutputID();

            m_objectInFront->UpdatePosition();
            m_objectInFront->UpdateTextureRect();
            m_objectInFront->SetInteractable(false);
            m_objectInFront->SetSaveIt(true);

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

                        auto item = new ItemGround(itemSprite, itemID, dist(gen));
                        game.SetItems(item);
                    }
                }
            }
        }
    }
}

// DRAW
void Player::DrawHotkey(sf::RenderWindow &window, Game *game)
{
    if (m_objectInFront != nullptr)
    {
        if (m_objectInFront->GetInteractable())
        {
            auto hotkeys = game->GetHotkeys();

            for (const auto &data : game->GetHotkeys())
            {
                if (data.first == "interact")
                {
                    auto hotkeyString =
                        (sf::Keyboard::getDescription(static_cast<sf::Keyboard::Scancode>(data.second))).toAnsiString();

                    m_hotkeyDraw->setString(hotkeyString);
                }
            }

            m_hotkeyDraw->setPosition((m_sprite->getPosition().x - (m_hotkeyDraw->getLocalBounds().getSize().x / 2)) +
                                          (m_sprite->getLocalBounds().getSize().x / 2),
                                      m_sprite->getPosition().y - 15);

            window.draw(*m_hotkeyDraw);
        }
    }
}

void Player::DrawStats(sf::RenderWindow &window, Game *game)
{
    auto width = (game->GetView()->getCenter().x - (game->GetView()->getSize().x / 2)) + 5.0F;

    size_t i = 1;
    size_t j = 1;

    for (const auto &data : game->GetStats())
    {
        auto textureSize = data->GetTextureSize();
        auto type = data->GetType();
        auto sprite = data->GetSprite();
        auto height = (game->GetView()->getCenter().y + (game->GetView()->getSize().y / 2)) - 10.0F;
        auto textureRectSize = sprite->getTextureRect().getSize();
        auto textureRectPos = sprite->getTextureRect().getPosition();

        if (type == StatType::Empty)
        {
            height -= i * 10;
            ++i;
        }
        else
        {
            auto statValue = GetStatValue(type);
            uint8_t newTextureRectSizeX = (statValue * textureSize.x) / 100;
            if (textureRectSize.x > 0)
            {
                auto textureRect =
                    sf::IntRect{textureRectPos.x, textureRectPos.y, newTextureRectSizeX, textureRectSize.y};
                sprite->setTextureRect(textureRect);
            }
            height -= j * 10;
            ++j;
        }

        sprite->setPosition(sf::Vector2f{width, height});
        window.draw(*sprite);
    }
}

void Player::DrawInventoryItems(sf::RenderWindow &window,
                                const vector<ItemCfg *> &itemCfg,
                                sf::Text *previousTxt,
                                const uint16_t width)
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
                    utilities.SetTextBeforeIcon(width, previousTxt, itemSprite, itemText);
                    prevSprite = itemSprite;
                    firstIcon = false;
                }
                else
                    utilities.SetTextBeforeIcon(itemSprite, itemText, prevSprite);


                break;
            }
        }
        window.draw(itemText);
        window.draw(itemSprite);
    }
}

// COLLISION
void Player::CheckCollision(Game *game)
{
    Utilities utilities;
    auto world = game->GetWorld();
    auto creature = game->GetCreature();

    for (const auto &data : world)
    {
        bool objectInFront = false;
        auto objPos = data->GetSprite()->getPosition();

        if (utilities.InViewRange(game, objPos))
        {
            auto objCollision = data->GetCollision();
            auto objSize = data->GetSprite()->getLocalBounds().getSize();
            auto interactable = data->GetInteractable();

            if (objCollision.x != 0 && objCollision.y != 0)
                objectInFront = CheckInFront(interactable, objPos, objSize, objCollision);
            else
                objectInFront = CheckInFront(interactable, objPos, objSize);

            if (objectInFront)
            {
                m_objectInFront = data;
                break;
            }
            else
                m_objectInFront = nullptr;
        }
    }

    for (const auto &data : creature)
    {
        bool objectInFront = false;

        auto objPos = data->GetSprite()->getPosition();

        if (utilities.InViewRange(game, objPos))
        {
            auto objSize = data->GetSprite()->getLocalBounds().getSize();
            auto interactable = data->GetUseable();

            objectInFront = CheckInFront(interactable, objPos, objSize);

            if (objectInFront)
            {
                m_creatureInFront = data;
                break;
            }
            else
                m_creatureInFront = nullptr;
        }
    }
}

bool Player::CheckInFront(const bool interactable,
                          const sf::Vector2f &objPos,
                          const sf::Vector2f &objSize,
                          const Collision objCollision)
{
    Utilities utilities;
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();

    auto canMoveDownUpCollisionX = utilities.CanMoveDownUpCollisionX(playerPos.x, objPos.x, objCollision.x);
    auto canMoveUpCollisionY = utilities.CanMoveUpCollisionY(playerPos.y, objPos.y, objSize.y, objCollision.y, m_speed);

    if (!canMoveDownUpCollisionX && !canMoveUpCollisionY)
    {
        m_moveAllowed.up = false;
        if (interactable && (m_lastMove == PlayerMove::Up || m_move == PlayerMove::Up))
            return true;
    }

    auto canMoveDownCollisionY = utilities.CanMoveDownCollisionY(playerPos.y, objPos.y, objCollision.y, m_speed);

    if (!canMoveDownUpCollisionX && !canMoveDownCollisionY)
    {
        m_moveAllowed.down = false;
        if (interactable && (m_lastMove == PlayerMove::Down || m_move == PlayerMove::Down))
            return true;
    }

    auto canMoveLeftCollisionX = utilities.CanMoveLeftCollisionX(playerPos.x, objPos.x, objCollision.x, m_speed);
    auto canMoveRightLeftCollisionY =
        utilities.CanMoveRightLeftCollisionY(playerPos.y, objPos.y, objSize.y, objCollision.y);

    if (!canMoveLeftCollisionX && !canMoveRightLeftCollisionY)
    {
        m_moveAllowed.left = false;
        if (interactable && (m_lastMove == PlayerMove::Left || m_move == PlayerMove::Left))
            return true;
    }

    auto canMoveRightCollisionX = utilities.CanMoveRightCollisionX(playerPos.x, objPos.x, m_speed);

    if (!canMoveRightCollisionX && !canMoveRightLeftCollisionY)
    {
        m_moveAllowed.right = false;
        if (interactable && (m_lastMove == PlayerMove::Right || m_move == PlayerMove::Right))
            return true;
    }

    return false;
}

bool Player::CheckInFront(const bool interactable, const sf::Vector2f &objPos, const sf::Vector2f &objSize)
{
    Utilities utilities;
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();

    auto canMoveDownUpX = utilities.CanMoveDownUpX(playerPos.x, playerSize.x, objPos.x, objSize.x);
    auto canMoveUpY = utilities.CanMoveUpY(playerPos.y, playerSize.y, objPos.y, objSize.y, m_speed);

    if (!canMoveDownUpX && !canMoveUpY)
    {
        m_moveAllowed.up = false;
        if (interactable && (m_lastMove == PlayerMove::Up || m_move == PlayerMove::Up))
            return true;
    }

    auto canMoveDownY = utilities.CanMoveDownY(playerPos.y, playerSize.y, objPos.y, objSize.y, m_speed);

    if (!canMoveDownUpX && !canMoveDownY)
    {
        m_moveAllowed.down = false;
        if (interactable && (m_lastMove == PlayerMove::Down || m_move == PlayerMove::Down))
            return true;
    }

    auto canMoveLeftX = utilities.CanMoveLeftX(playerPos.x, objPos.x, objSize.x, m_speed);
    auto canMoveRightLeftY = utilities.CanMoveRightLeftY(playerPos.y, playerSize.y, objSize.y, objPos.y);

    if (!canMoveLeftX && !canMoveRightLeftY)
    {
        m_moveAllowed.left = false;
        if (interactable && (m_lastMove == PlayerMove::Left || m_move == PlayerMove::Left))
            return true;
    }

    auto canMoveRightX = utilities.CanMoveRightX(playerPos.x, playerSize.x, objPos.x, m_speed);

    if (!canMoveRightX && !canMoveRightLeftY)
    {
        m_moveAllowed.right = false;
        if (interactable && (m_lastMove == PlayerMove::Right || m_move == PlayerMove::Right))
            return true;
    }

    return false;
}

// DATASTORE
void Player::Load(const uint8_t id, Game *game)
{
    auto view = game->GetView();
    auto path = format("./save/{}/player.json", id);

    ifstream file(path);

    if (file.is_open())
    {
        auto jsonData = json::parse(file);

        m_name = jsonData["name"];
        m_health = jsonData["health"];
        m_survivalStats.water = jsonData["water"];
        m_survivalStats.food = jsonData["food"];
        m_sprite->setPosition(jsonData["playerPosX"], jsonData["playerPosY"]);
        view->setCenter(jsonData["viewPosX"], jsonData["viewPosY"]);

        while (game->GetZoom() != jsonData["zoom"])
        {
            if (game->GetZoom() < jsonData["zoom"])
            {
                view->zoom(0.5F);
                game->SetZoom(1U);
            }
            else
            {
                view->zoom(2.0F);
                game->SetZoom(-1);
            }
        }

        for (const auto &data : jsonData["items"])
        {
            m_items[data[0]] = data[1];
        }

        file.close();
    }
}

void Player::Save(const bool destroy, Game *game)
{
    auto path = format("./save/{}/player.json", m_ID);

    ofstream file(path);

    if (file.is_open())
    {
        json jsonData = {{"name", m_name},
                         {"health", m_health},
                         {"water", m_survivalStats.water},
                         {"food", m_survivalStats.food},
                         {"playerPosX", m_sprite->getPosition().x},
                         {"playerPosY", m_sprite->getPosition().y},
                         {"items", m_items},
                         {"viewPosX", game->GetView()->getCenter().x},
                         {"viewPosY", game->GetView()->getCenter().y},
                         {"zoom", game->GetZoom()}};

        file << jsonData;
        file.close();
    }

    if (destroy)
    {
        delete m_hotkeyDraw;
        m_hotkeyDraw = nullptr;
    }
}
