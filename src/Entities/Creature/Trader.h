#pragma once
#include "Creature.h"
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

class Trader : public Creature
{
public:
    Trader(unique_ptr<sf::Sprite> sprite,
           const float health,
           const float speed,
           const uint8_t animID,
           const bool moving,
           const vector<string> dialogIntro,
           const vector<string> dialogOutro,
           const vector<string> dialogOffensive,
           const bool interactable,
           const map<uint8_t, uint16_t> sellingItem,
           const map<uint8_t, uint16_t> buyingItem);
    ~Trader() = default;

    map<uint8_t, uint16_t> GetSellingItem() const;
    map<uint8_t, uint16_t> GetBuyingItem() const;

    void SetSelectedItemID(const uint8_t ID);

private:
    map<uint8_t, uint16_t> m_sellingItem;
    map<uint8_t, uint16_t> m_buyingItem;
    uint8_t m_selectedItemID;
};
