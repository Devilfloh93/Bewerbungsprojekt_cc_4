#include "Unit.h"

class Creature : public Unit
{

public:
    Creature(sf::Sprite *sprite, const float health, const float speed, const uint8_t animID, const bool moving);
    ~Creature() = default;

    bool GetUseable() const;
    bool GetMoving() const;

private:
    bool m_useable;
    bool m_moving;
};
