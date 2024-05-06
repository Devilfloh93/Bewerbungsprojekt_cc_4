#include "Creature.h"

Creature::Creature(unique_ptr<sf::Sprite> sprite,
                   const float health,
                   const float speed,
                   const uint8_t animID,
                   const bool moving,
                   const vector<string> dialogIntro,
                   const vector<string> dialogOutro,
                   const vector<string> dialogOffensive,
                   const bool interactable)
    : Unit(move(sprite), health, speed, animID), m_moving(moving), m_dialogIntro(dialogIntro),
      m_dialogOutro(dialogOutro), m_dialogOffensive(dialogOffensive), m_interactable(interactable)
{
}

bool Creature::GetInteractable() const
{
    return m_interactable;
}

bool Creature::GetMoving() const
{
    return m_moving;
}

vector<string> Creature::GetDialogIntro() const
{
    return m_dialogIntro;
}

vector<string> Creature::GetDialogOutro() const
{
    return m_dialogOutro;
}

vector<string> Creature::GetDialogOffensive() const
{
    return m_dialogOffensive;
}

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
