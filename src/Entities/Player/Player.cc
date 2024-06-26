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

Player::Player(unique_ptr<sf::Sprite> sprite,
               const uint8_t animID,
               const string_view name,
               const uint8_t id,
               const Guid guid)
    : Unit(move(sprite), 100.0F, 1.0F, animID, guid), m_name(name), m_ID(id)
{
    Init();
}

Player::Player(unique_ptr<sf::Sprite> sprite, const uint8_t animID, const uint8_t id, const Guid guid)
    : Unit(move(sprite), 100.0F, 1.0F, animID, guid), m_ID(id)
{
    Init();
}

// INIT
void Player::Init()
{
    m_sprite->setPosition(80.0F, 80.0F);
    m_survivalStats = {.water = 100.0F, .food = 100.0F};
    m_trader = nullptr;
    m_objectInFront = nullptr;
    m_creatureInFront = nullptr;
}

// INFO
uint8_t Player::GetID() const
{
    return m_ID;
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
            break;
        }
    }

    if (newItem)
        m_items[ID] = count;
}

uint16_t Player::GetItemCount(const uint8_t ID)
{
    return m_items[ID];
}

map<uint8_t, uint16_t> Player::GetItems() const
{
    return m_items;
}

void Player::UseItem(Game &game)
{
    Utilities utilities;
    auto selectedDialogID = game.GetDialogSelectedID(SelectedTextCategorie::Nothing);
    if (selectedDialogID == 0)
    {
        game.AddMessage(15, MessageType::Error);
        return;
    }
    string name = "";
    bool itemUsed = false;

    auto itemCfg = game.GetItemCfg();
    for (const auto &data : itemCfg)
    {
        auto ID = data->GetID();
        if (selectedDialogID == ID)
        {
            name = data->GetName();
            itemUsed = true;
            auto usableFnc = data->GetUsableFnc();
            auto usableValue = data->GetUsableValue();

            switch (usableFnc)
            {
            case ItemFnc::Food:
                if (m_survivalStats.food + usableValue > 100)
                    m_survivalStats.food = 100;
                else
                    m_survivalStats.food += usableValue;
                break;
            case ItemFnc::Water:
                if (m_survivalStats.water + usableValue > 100)
                    m_survivalStats.water = 100;
                else
                    m_survivalStats.water += usableValue;
                break;
            case ItemFnc::Health:
                if (m_health + usableValue > 100)
                    m_health = 100;
                else
                    m_health += usableValue;
                break;

            default:
                itemUsed = false;
                break;
            }

            break;
        }
    }

    if (itemUsed)
    {
        auto removed = utilities.RemoveItem(m_items, selectedDialogID, 1);

        if (removed != ItemRemoved::Failed)
        {
            game.AddMessage(18, MessageType::Success, 1, name);

            if (removed == ItemRemoved::Updated)
            {
                auto message = format("{}", m_items[selectedDialogID]);
                game.UpdateDialog(SelectedTextCategorie::Nothing, message);
            }
            else if (removed == ItemRemoved::Removed)
                game.UpdateDialog(SelectedTextCategorie::Nothing);
        }
    }
    else
        game.AddMessage(19, MessageType::Error, name);
}

void Player::HandleMove(Game *game)
{
    game->ExecuteMove(this);
    game->CollectItem();
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

    if (game->GetPlaying() && game->GetMenuState().first == MenuState::Playing)
    {
        if (m_survivalStats.food - statDecay.food < 0U)
            m_survivalStats.food = 0;
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
            game.SetMenuState(MenuState::Trader, true);
            InitTraderItems(game);
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
                        case Move::Up:
                            itemPos = {playerPos.x, playerPos.y + posIncrease};
                            break;
                        case Move::Down:
                            itemPos = {playerPos.x, playerPos.y - posIncrease};
                            break;
                        case Move::Right:
                            itemPos = {playerPos.x - posIncrease, playerPos.y};
                            break;
                        case Move::Left:
                            itemPos = {playerPos.x + posIncrease, playerPos.y};
                            break;
                        case Move::NotMoving:
                            switch (m_lastMove)
                            {
                            case Move::Up:
                                itemPos = {playerPos.x, playerPos.y + posIncrease};
                                break;
                            case Move::Down:
                                itemPos = {playerPos.x, playerPos.y - posIncrease};
                                break;
                            case Move::Right:
                                itemPos = {playerPos.x - posIncrease, playerPos.y};
                                break;
                            case Move::Left:
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

void Player::InitTraderItems(Game &game)
{
    bool firstIcon = true;
    Utilities utilities;
    sf::Vector2f prevPos;
    auto itemCfg = game.GetItemCfg();
    auto fonts = game.GetFont();
    auto windowSizeWidth = game.GetWindowSize().width;
    auto textures = game.GetTexture();
    auto sellingItems = m_trader->GetSellingItem();
    auto buyingItems = m_trader->GetBuyingItem();
    auto titles = game.GetTitles();
    auto input = game.GetInput();
    uint16_t selectableID = 1;

    auto prevTxt = utilities.GetInput(input, MenuState::Trader);

    if (prevTxt == nullptr)
        prevTxt = utilities.GetTitle(titles, MenuState::Trader);

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

                            auto messageFormat = utilities.GetMessageFormat(game, 8);
                            auto message = vformat(messageFormat, make_format_args(value, value1));

                            auto spriteUnique = itemSprite.get();
                            auto textUnique = itemText.get();

                            utilities.SetSFText(textUnique, 15U, fonts, fontID, message);

                            utilities.SetSFSprite(spriteUnique, texture, textureData);
                            auto alignemntWidth =
                                utilities.CalculateAlignmentWindowWidth(windowSizeWidth, Alignment::Right);

                            if (firstIcon)
                            {
                                utilities.SetTextBeforeIcon(alignemntWidth, prevTxt, *spriteUnique, *textUnique);
                                firstIcon = false;
                            }
                            else
                                utilities.SetTextBeforeIcon(*spriteUnique, *textUnique, prevPos);

                            prevPos = spriteUnique->getGlobalBounds().getPosition();

                            game.SetDialogSprite(make_unique<SelectableSprite>(move(itemSprite),
                                                                               selectableID,
                                                                               SelectedTextCategorie::Sell));
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

                    auto messageFormat = utilities.GetMessageFormat(game, 2);
                    auto message = vformat(messageFormat, make_format_args(value));

                    auto spriteUnique = itemSprite.get();
                    auto textUnique = itemText.get();

                    utilities.SetSFText(textUnique, 15U, fonts, fontID, message);

                    utilities.SetSFSprite(spriteUnique, texture, textureData);
                    auto alignemntWidth = utilities.CalculateAlignmentWindowWidth(windowSizeWidth, Alignment::Left);

                    if (firstIcon)
                    {
                        utilities.SetTextBeforeIcon(alignemntWidth, prevTxt, *spriteUnique, *textUnique);
                        firstIcon = false;
                    }
                    else
                        utilities.SetTextBeforeIcon(*spriteUnique, *textUnique, prevPos);

                    prevPos = spriteUnique->getGlobalBounds().getPosition();
                    game.SetDialogSprite(
                        make_unique<SelectableSprite>(move(itemSprite), selectableID, SelectedTextCategorie::Buy));
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
    bool firstIcon = true;
    Utilities utilities;
    sf::Vector2f prevPos;
    auto itemCfg = game.GetItemCfg();
    auto fonts = game.GetFont();
    auto windowSizeWidth = game.GetWindowSize().width;
    auto titles = game.GetTitles();
    auto textures = game.GetTexture();
    uint16_t selectableID = 1;

    auto prevTxt = utilities.GetTitle(titles, MenuState::Inventory);

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

                auto spriteUnique = itemSprite.get();
                auto textUnique = itemText.get();

                utilities.SetSFText(textUnique, 15U, fonts, fontID, value);

                utilities.SetSFSprite(spriteUnique, texture, textureData);

                if (firstIcon)
                {
                    utilities.SetTextBeforeIcon(windowSizeWidth, prevTxt, *spriteUnique, *textUnique);
                    firstIcon = false;
                }
                else
                    utilities.SetTextBeforeIcon(*spriteUnique, *textUnique, prevPos);

                prevPos = spriteUnique->getGlobalBounds().getPosition();
                game.SetDialogSprite(
                    make_unique<SelectableSprite>(move(itemSprite), selectableID, SelectedTextCategorie::Nothing));
                game.SetDialogText(
                    make_unique<SelectableText>(move(itemText), selectableID, key, SelectedTextCategorie::Nothing));
                ++selectableID;
                break;
            }
        }
    }
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

        while (game->GetZoom().cur != jsonData["zoom"])
        {
            if (game->GetZoom().cur < jsonData["zoom"])
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
    game->UpdateViewPosition();
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
                         {"zoom", game->GetZoom().cur}};

        file << jsonData;
        file.close();
    }
}

// TRADER
Trader *Player::GetTrader()
{
    return m_trader;
}

// ObjectInFront

void Player::SetWorldObjInFront(World *worldObj)
{
    m_objectInFront = worldObj;
}

void Player::SetCreatureObjInFront(Creature *creatureObj)
{
    m_creatureInFront = creatureObj;
}
