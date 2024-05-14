#include "Player.h"
#include "Collision.h"
#include "Stats.h"
#include "Utilities.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <random>

/**
 * @brief Construct a new Player:: Player object
 *
 * @param sprite
 * @param animID
 * @param name Playername of the player created by himself
 * @param id unique PlayerID
 */

Player::Player(unique_ptr<sf::Sprite> sprite, const uint8_t animID, const string_view name, const uint8_t id)
    : Unit(move(sprite), 100.0F, 1.0F, animID), m_name(name), m_ID(id)
{
    Init();
}

Player::Player(unique_ptr<sf::Sprite> sprite, const uint8_t animID, const uint8_t id)
    : Unit(move(sprite), 100.0F, 1.0F, animID), m_ID(id)
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
    m_trader = nullptr;
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

            if ((playerPos.x + (playerSize.x / 2)) >= itemPos.x && playerPos.x <= itemPos.x + (itemSize.x / 2) &&
                playerPos.y + (playerSize.y / 2) >= itemPos.y && playerPos.y <= itemPos.y + (itemSize.y / 2))
            {
                AddItem(ID, count);
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

void Player::AddItem(const uint8_t ID, const uint16_t count)
{
    bool newItem = true;
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
}

bool Player::RemoveItem(const uint8_t ID, const uint16_t count)
{
    for (const auto &[key, value] : m_items)
    {
        if (key == ID && value >= count)
        {
            uint16_t newValue = value - count;
            if (newValue > 0)
                m_items[key] = newValue;
            else
                m_items.erase(key);

            return true;
        }
    }

    return false;
}

uint16_t Player::GetItemCount(const uint8_t ID)
{
    return m_items[ID];
}

void Player::HandleMove(sf::Clock &clock, Game *game)
{
    Utilities utilities;
    auto anim = game->GetAnim();
    auto tileSize = game->GetTileSize();
    auto width = game->GetGameWidth();
    auto height = game->GetGameHeight();
    auto playerPos = m_sprite->getPosition();
    auto sprite = m_sprite.get();

    auto animData = utilities.GetAnim(anim, m_animID);

    switch (m_move)
    {
    case PlayerMove::Left:
        if (playerPos.x - m_speed > 0 + (tileSize / 2) && m_moveAllowed.left)
        {
            utilities.PlayAnimation(sprite, clock, animData.left.notMoving, animData.left.anim01);
            m_sprite->setPosition(playerPos.x - m_speed, playerPos.y);
        }
        else
            m_sprite->setTextureRect(animData.left.notMoving);
        break;
    case PlayerMove::Right:
        if (playerPos.x + m_speed < width - tileSize && m_moveAllowed.right)
        {
            utilities.PlayAnimation(sprite, clock, animData.right.notMoving, animData.right.anim01);
            m_sprite->setPosition(playerPos.x + m_speed, playerPos.y);
        }
        else
            m_sprite->setTextureRect(animData.right.notMoving);
        break;
    case PlayerMove::Down:
        if (playerPos.y + m_speed < height - tileSize && m_moveAllowed.down)
        {
            utilities.PlayAnimation(sprite, clock, animData.down.anim01, animData.down.anim02);
            m_sprite->setPosition(playerPos.x, playerPos.y + m_speed);
        }
        else
            m_sprite->setTextureRect(animData.down.notMoving);

        break;
    case PlayerMove::Up:
        if (playerPos.y - m_speed > 0 + (tileSize / 2) && m_moveAllowed.up)
        {
            utilities.PlayAnimation(sprite, clock, animData.up.anim01, animData.up.anim02);
            m_sprite->setPosition(playerPos.x, playerPos.y - m_speed);
        }
        else
            m_sprite->setTextureRect(animData.up.notMoving);

        break;
    case PlayerMove::NotMoving:
        switch (m_lastMove)
        {
        case PlayerMove::Down:
            m_sprite->setTextureRect(animData.down.notMoving);
            break;
        case PlayerMove::Up:
            m_sprite->setTextureRect(animData.up.notMoving);
            break;
        case PlayerMove::Left:
            m_sprite->setTextureRect(animData.left.notMoving);
            break;
        case PlayerMove::Right:
            m_sprite->setTextureRect(animData.right.notMoving);
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
    Utilities utilities;
    auto itemCfg = game.GetItemCfg();
    auto playerPos = m_sprite->getPosition();
    sf::Vector2f itemPos = {0.0F, 0.0F};

    if (m_creatureInFront != nullptr)
    {
        auto tradeable = m_creatureInFront->GetInteractable();

        if (tradeable)
        {
            m_trader = static_cast<Trader *>(m_creatureInFront);
            InitTraderItems(game);
            game.SetMenuState(MenuState::Trader);
        }
    }

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
                auto textureID = data->GetTextureID();
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

                        game.CreateGroundItem(textureID, textureData, itemPos.x, itemPos.y, itemID, dist(gen));
                    }
                }
            }
        }
    }
}

// Render
void Player::CheckRenderHotkey(Game *game)
{
    if (m_objectInFront != nullptr)
    {
        if (m_objectInFront->GetInteractable())
        {
            game->RenderHotkey();
        }
    }

    if (m_creatureInFront != nullptr)
    {
        if (m_creatureInFront->GetInteractable())
        {
            game->RenderHotkey();
        }
    }
}

/***
 * TODO: REWORK RENDER TRADER ITEMS
*/
void Player::InitTraderItems(Game &game)
{
    game.ClearDialog();

    bool firstIcon = true;
    Utilities utilities;
    sf::Vector2f prevPos;
    auto itemCfg = game.GetItemCfg();
    auto fonts = game.GetFont();
    auto width = game.GetWindowWidth();
    auto textures = game.GetTexture();
    auto sellingItems = m_trader->GetSellingItem();
    auto buyingItems = m_trader->GetBuyingItem();
    auto titles = game.GetTitles();
    auto input = game.GetInput();
    sf::Text *previousTxt;
    uint16_t selectableID = 1;

    for (const auto &data : titles)
    {

        if (data->GetMenuState() == MenuState::Trader)
        {
            previousTxt = data->GetText();
            break;
        }
    }

    for (const auto &data : input)
    {
        if (data->GetMenuState() == MenuState::Trader)
        {
            previousTxt = data->GetText();
        }
    }

    for (const auto &[key, value] : sellingItems)
    {
        if (value > 0)
        {
            for (const auto &[key1, value1] : m_items)
            {
                if (key == key1)
                {
                    for (const auto &data : itemCfg)
                    {
                        auto ID = data->GetID();
                        if (key == ID)
                        {
                            auto textureID = data->GetTextureID();
                            auto itemSprite = make_unique<sf::Sprite>();
                            auto itemText = make_unique<sf::Text>();

                            auto texture = utilities.GetTexture(textures, textureID);

                            auto textureData = data->GetTextureData();
                            auto fontID = data->GetFontID();

                            auto font = utilities.GetFont(fonts, fontID);

                            auto text = format("Selling: {} - Inventory: {}", value, value1);
                            auto spriteUnique = itemSprite.get();
                            auto textUnique = itemText.get();

                            utilities.SetSFText(textUnique, font, 15, text);

                            utilities.SetSFSprite(spriteUnique, texture, textureData);
                            auto alignemntWidth = utilities.CalculateAlignmentWindowWidth(width, Alignment::Right);

                            if (firstIcon)
                            {
                                utilities.SetTextBeforeIcon(alignemntWidth, previousTxt, *spriteUnique, *textUnique);
                                firstIcon = false;
                            }
                            else
                                utilities.SetTextBeforeIcon(*spriteUnique, *textUnique, prevPos);

                            prevPos = spriteUnique->getGlobalBounds().getPosition();

                            game.SetDialogSprite(make_unique<Sprite>(move(itemSprite)));
                            game.SetDialogText(make_unique<SelectableText>(move(itemText),
                                                                           selectableID,
                                                                           key,
                                                                           SelectedTextCategorie::Sell));
                            ++selectableID;
                            break;
                        }
                    }
                }
            }
        }
    }

    firstIcon = true;

    for (const auto &[key, value] : buyingItems)
    {
        if (value > 0)
        {
            for (const auto &data : itemCfg)
            {
                auto ID = data->GetID();
                if (key == ID)
                {
                    auto textureID = data->GetTextureID();
                    auto itemSprite = make_unique<sf::Sprite>();
                    auto itemText = make_unique<sf::Text>();

                    auto texture = utilities.GetTexture(textures, textureID);
                    auto textureData = data->GetTextureData();
                    auto fontID = data->GetFontID();

                    auto font = utilities.GetFont(fonts, fontID);

                    auto text = format("Buying: {}", value);
                    auto spriteUnique = itemSprite.get();
                    auto textUnique = itemText.get();

                    utilities.SetSFText(textUnique, font, 15, text);

                    utilities.SetSFSprite(spriteUnique, texture, textureData);
                    auto alignemntWidth = utilities.CalculateAlignmentWindowWidth(width, Alignment::Left);

                    if (firstIcon)
                    {
                        utilities.SetTextBeforeIcon(alignemntWidth, previousTxt, *spriteUnique, *textUnique);
                        firstIcon = false;
                    }
                    else
                        utilities.SetTextBeforeIcon(*spriteUnique, *textUnique, prevPos);

                    prevPos = spriteUnique->getGlobalBounds().getPosition();
                    game.SetDialogSprite(make_unique<Sprite>(move(itemSprite)));
                    game.SetDialogText(
                        make_unique<SelectableText>(move(itemText), selectableID, key, SelectedTextCategorie::Buy));
                    ++selectableID;
                    break;
                }
            }
        }
    }
}

void Player::InitInventoryItems(Game &game)
{
    game.ClearDialog();
    bool firstIcon = true;
    Utilities utilities;
    sf::Vector2f prevPos;
    auto itemCfg = game.GetItemCfg();
    auto fonts = game.GetFont();
    auto width = game.GetWindowWidth();
    auto titles = game.GetTitles();
    auto textures = game.GetTexture();
    sf::Text *previousTxt;
    uint16_t selectableID = 1;

    for (const auto &data : titles)
    {
        if (data->GetMenuState() == MenuState::Inventory)
        {
            previousTxt = data->GetText();
            break;
        }
    }

    for (const auto &[key, value] : m_items)
    {
        for (const auto &data : itemCfg)
        {
            auto ID = data->GetID();
            if (key == ID)
            {
                auto textureID = data->GetTextureID();
                auto itemSprite = make_unique<sf::Sprite>();
                auto itemText = make_unique<sf::Text>();
                auto texture = utilities.GetTexture(textures, textureID);
                auto textureData = data->GetTextureData();
                auto fontID = data->GetFontID();

                auto font = utilities.GetFont(fonts, fontID);
                auto spriteUnique = itemSprite.get();
                auto textUnique = itemText.get();

                utilities.SetSFText(textUnique, font, 15, value);

                utilities.SetSFSprite(spriteUnique, texture, textureData);

                if (firstIcon)
                {
                    utilities.SetTextBeforeIcon(width, previousTxt, *spriteUnique, *textUnique);
                    firstIcon = false;
                }
                else
                    utilities.SetTextBeforeIcon(*spriteUnique, *textUnique, prevPos);

                prevPos = spriteUnique->getGlobalBounds().getPosition();
                game.SetDialogSprite(make_unique<Sprite>(move(itemSprite)));
                game.SetDialogText(
                    make_unique<SelectableText>(move(itemText), selectableID, key, SelectedTextCategorie::Nothing));
                ++selectableID;
                break;
            }
        }
    }
}

// COLLISION
void Player::CheckCollision(Game *game)
{
    Collision collision;
    bool objectInFront = false;
    auto world = game->GetWorld();
    auto creature = game->GetCreature();

    for (const auto &data : world)
    {
        auto objPos = data->GetSprite()->getPosition();

        if (collision.InViewRange(game, objPos))
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
        auto objPos = data->GetSprite()->getPosition();

        if (collision.InViewRange(game, objPos))
        {
            auto objSize = data->GetSprite()->getLocalBounds().getSize();
            auto interactable = data->GetInteractable();

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
                          const CollisionData objCollision)
{
    Collision collision;
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();

    auto canMoveDownUpX = collision.CanMoveDownUpX(playerPos.x, objPos.x, objCollision.x);
    auto canMoveUpY = collision.CanMoveUpY(playerPos.y, objPos.y, objSize.y, objCollision.y, m_speed);

    if (!canMoveDownUpX && !canMoveUpY)
    {
        m_moveAllowed.up = false;
        if (interactable && (m_lastMove == PlayerMove::Up || m_move == PlayerMove::Up))
            return true;
    }

    auto canMoveDownY = collision.CanMoveDownY(playerPos.y, objPos.y, objCollision.y, m_speed);

    if (!canMoveDownUpX && !canMoveDownY)
    {
        m_moveAllowed.down = false;
        if (interactable && (m_lastMove == PlayerMove::Down || m_move == PlayerMove::Down))
            return true;
    }

    auto canMoveLeftX = collision.CanMoveLeftX(playerPos.x, objPos.x, objCollision.x, m_speed);
    auto canMoveRightLeftY = collision.CanMoveRightLeftY(playerPos.y, objPos.y, objSize.y, objCollision.y);

    if (!canMoveLeftX && !canMoveRightLeftY)
    {
        m_moveAllowed.left = false;
        if (interactable && (m_lastMove == PlayerMove::Left || m_move == PlayerMove::Left))
            return true;
    }

    auto canMoveRightX = collision.CanMoveRightX(playerPos.x, objPos.x, m_speed);

    if (!canMoveRightX && !canMoveRightLeftY)
    {
        m_moveAllowed.right = false;
        if (interactable && (m_lastMove == PlayerMove::Right || m_move == PlayerMove::Right))
            return true;
    }

    return false;
}

bool Player::CheckInFront(const bool interactable, const sf::Vector2f &objPos, const sf::Vector2f &objSize)
{
    Collision collision;
    auto playerPos = m_sprite->getPosition();
    auto playerSize = m_sprite->getLocalBounds().getSize();

    auto canMoveDownUpX = collision.CanMoveDownUpX(playerPos.x, playerSize.x, objPos.x, objSize.x);
    auto canMoveUpY = collision.CanMoveUpY(playerPos.y, playerSize.y, objPos.y, objSize.y, m_speed);

    if (!canMoveDownUpX && !canMoveUpY)
    {
        m_moveAllowed.up = false;
        if (interactable && (m_lastMove == PlayerMove::Up || m_move == PlayerMove::Up))
            return true;
    }

    auto canMoveDownY = collision.CanMoveDownY(playerPos.y, playerSize.y, objPos.y, objSize.y, m_speed);

    if (!canMoveDownUpX && !canMoveDownY)
    {
        m_moveAllowed.down = false;
        if (interactable && (m_lastMove == PlayerMove::Down || m_move == PlayerMove::Down))
            return true;
    }

    auto canMoveLeftX = collision.CanMoveLeftX(playerPos.x, objPos.x, objSize.x, m_speed);
    auto canMoveRightLeftY = collision.CanMoveRightLeftY(playerPos.y, playerSize.y, objSize.y, objPos.y);

    if (!canMoveLeftX && !canMoveRightLeftY)
    {
        m_moveAllowed.left = false;
        if (interactable && (m_lastMove == PlayerMove::Left || m_move == PlayerMove::Left))
            return true;
    }

    auto canMoveRightX = collision.CanMoveRightX(playerPos.x, playerSize.x, objPos.x, m_speed);

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

    game->UpdateView();
}

void Player::Save(Game *game)
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
}

// TRADER
Trader *Player::GetTrader()
{
    return m_trader;
}
