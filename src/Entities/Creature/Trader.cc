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
