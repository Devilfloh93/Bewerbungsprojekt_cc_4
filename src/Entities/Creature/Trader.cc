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
               const map<uint8_t, uint16_t> buyingItem)
    : Creature(move(sprite), health, speed, animID, moving, dialogIntro, dialogOutro, dialogOffensive, interactable),
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
    auto itemCfg = game.GetItemCfg();
    auto dialogText = game.GetDialogText();
    auto selectedDialogID = game.GetDialogSelectedID(dialogText, SelectedTextCategorie::Buy);

    auto maxCount = m_buyingItem[selectedDialogID];

    game.GetPlayer()->AddItem(selectedDialogID, maxCount);
    m_buyingItem[selectedDialogID] -= maxCount;
    auto text = format("Buying: {}", m_buyingItem[selectedDialogID]);
    UpdateTrader(game, dialogText, SelectedTextCategorie::Buy, text);
}

void Trader::Sell(Game &game)
{
    auto player = game.GetPlayer();
    auto itemCfg = game.GetItemCfg();
    auto dialogText = game.GetDialogText();
    auto selectedDialogID = game.GetDialogSelectedID(dialogText, SelectedTextCategorie::Sell);

    auto maxCount = m_sellingItem[selectedDialogID];
    auto removed = player->RemoveItem(selectedDialogID, maxCount);

    if (removed)
    {
        m_sellingItem[selectedDialogID] -= maxCount;
        auto inventoryValue = player->GetItemCount(selectedDialogID);
        auto text = format("Selling: {} - Inventory: {}", m_sellingItem[selectedDialogID], inventoryValue);
        UpdateTrader(game, dialogText, SelectedTextCategorie::Sell, text);
    }
    else
        cout << "Not Enough Items to sell!" << endl;
}

void Trader::UpdateTrader(Game &game,
                          const vector<unique_ptr<SelectableText>> *vec,
                          const SelectedTextCategorie selectedCategorie,
                          const string &text)
{
    for (auto const &data : *vec)
    {
        auto ID = data.get()->GetID();
        auto categorie = data.get()->GetSelectedCategorie();

        if (ID == game.GetSelectedTextID() && categorie == selectedCategorie)
        {
            data.get()->GetText()->setString(text);
            break;
        }
    }
    game.SetSelectedTextID(0);
}
