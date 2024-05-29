#include "Trader.h"


/**
 * @brief Construct a new Creature:: Creature object
 *
 * @param sprite
 * @param health healtpoints the creature has
 * @param speed how many pixels the creature can move with one movement command
 * @param animID which animation it should play while running
 * @param moving true / false if creature is allowed to move
 * @param dialogIntro     |
 * @param dialogOutro     | Dialog Optionen when interacting with the creature or random messages while playing (WIP)
 * @param dialogOffensive |
 * @param interactable true / false can Player interact with this creature like trading
 * @param sellingItem stores all sellingItem defined in data/entities/trader/trader.json
 * @param buyingItem stores all buyingItem defined in data/entities/trader/trader.json
 */

Trader::Trader(unique_ptr<sf::Sprite> sprite,
               const float health,
               const float speed,
               const uint8_t animID,
               const bool moving,
               const vector<string> dialogIntro,
               const vector<string> dialogOutro,
               const vector<string> dialogOffensive,
               const bool interactable,
               const map<uint8_t, uint16_t> sellingItem,
               const map<uint8_t, uint16_t> buyingItem,
               const float maxMoveRange,
               const Faction faction,
               const Guid guid)
    : Creature(move(sprite),
               health,
               speed,
               animID,
               moving,
               dialogIntro,
               dialogOutro,
               dialogOffensive,
               interactable,
               maxMoveRange,
               faction,
               guid),
      m_sellingItem(sellingItem), m_buyingItem(buyingItem)
{
}

map<uint8_t, uint16_t> Trader::GetSellingItem() const
{
    return m_sellingItem;
}

map<uint8_t, uint16_t> Trader::GetBuyingItem() const
{
    return m_buyingItem;
}

void Trader::Buy(Game &game)
{
    Utilities utilities;
    auto selectedDialogID = game.GetDialogSelectedID(SelectedTextCategorie::Buy);

    auto input = game.GetInputString();
    if (input.size() == 0)
    {
        game.AddMessage(1, MessageType::Error);
        return;
    }

    if (selectedDialogID == 0)
    {
        game.AddMessage(16, MessageType::Error);
        return;
    }

    uint16_t inputNum = stoi(input);

    auto maxCount = m_buyingItem[selectedDialogID];

    if (maxCount >= inputNum)
    {
        auto itemName = utilities.GetItemName(game, selectedDialogID);
        game.GetPlayer()->AddItem(selectedDialogID, inputNum);

        auto removed = utilities.RemoveItem(m_buyingItem, selectedDialogID, inputNum);

        game.AddMessage(3, MessageType::Success, inputNum, itemName);

        if (removed == ItemRemoved::Updated)
        {
            auto messageFormat = utilities.GetMessageFormat(game, 2);
            auto message = vformat(messageFormat, make_format_args(m_buyingItem[selectedDialogID]));
            game.UpdateDialog(SelectedTextCategorie::Buy, message);
        }
        else if (removed == ItemRemoved::Removed)
            game.UpdateDialog(SelectedTextCategorie::Buy);
    }
    else
        game.AddMessage(4, MessageType::Error);
}

void Trader::Sell(Game &game)
{
    Utilities utilities;
    auto player = game.GetPlayer();
    auto selectedDialogID = game.GetDialogSelectedID(SelectedTextCategorie::Sell);

    auto input = game.GetInputString();

    if (input.size() == 0)
    {
        game.AddMessage(5, MessageType::Error);
        return;
    }

    if (selectedDialogID == 0)
    {
        game.AddMessage(17, MessageType::Error);
        return;
    }

    uint16_t inputNum = stoi(input);

    auto maxCount = m_sellingItem[selectedDialogID];

    if (maxCount >= inputNum)
    {
        auto playerItems = player->GetItems();
        auto removed = utilities.RemoveItem(playerItems, selectedDialogID, inputNum);

        if (removed != ItemRemoved::Failed)
        {
            auto itemName = utilities.GetItemName(game, selectedDialogID);
            removed = utilities.RemoveItem(m_sellingItem, selectedDialogID, inputNum);

            game.AddMessage(9, MessageType::Success, inputNum, itemName);

            if (removed == ItemRemoved::Updated)
            {
                auto inventoryValue = player->GetItemCount(selectedDialogID);
                auto messageFormat = utilities.GetMessageFormat(game, 8);
                auto message =
                    vformat(messageFormat, make_format_args(m_sellingItem[selectedDialogID], inventoryValue));
                game.UpdateDialog(SelectedTextCategorie::Sell, message);
            }
            else if (removed == ItemRemoved::Removed)
                game.UpdateDialog(SelectedTextCategorie::Sell);
        }
        else
            game.AddMessage(6, MessageType::Error);
    }
    else
        game.AddMessage(7, MessageType::Error);
}
