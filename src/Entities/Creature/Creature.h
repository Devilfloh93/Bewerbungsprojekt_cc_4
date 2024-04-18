#include "Unit.h"

class Creature : public Unit
{

public:
    Creature(sf::Sprite *sprite, const float health, const float speed, const uint8_t animID);
    ~Creature() = default;

    bool GetUseable() const;

private:
    bool m_useable;
};
