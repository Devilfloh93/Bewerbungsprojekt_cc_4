#include "Unit.h"
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

class Creature : public Unit
{

public:
    Creature(sf::Sprite *sprite,
             const float health,
             const float speed,
             const uint8_t animID,
             const bool moving,
             const vector<string> dialogIntro,
             const vector<string> dialogOutro,
             const vector<string> dialogOffensive,
             const bool interactable);
    ~Creature() = default;

    bool GetInteractable() const;
    bool GetMoving() const;

    vector<string> GetDialogIntro() const;
    vector<string> GetDialogOutro() const;
    vector<string> GetDialogOffensive() const;

private:
    bool m_interactable;
    bool m_moving;
    vector<string> m_dialogIntro;
    vector<string> m_dialogOutro;
    vector<string> m_dialogOffensive;
};

class Trader : public Creature
{
public:
    Trader(sf::Sprite *sprite,
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

private:
    map<uint8_t, uint16_t> m_sellingItem;
    map<uint8_t, uint16_t> m_buyingItem;
};
