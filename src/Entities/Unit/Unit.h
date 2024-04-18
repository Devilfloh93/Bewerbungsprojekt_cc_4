#include "Sprite.h"

class Unit : public Sprite
{
public:
    Unit(sf::Sprite *sprite, const float health, const float speed, const uint8_t animID);
    ~Unit() = default;

    void SetSpeed(const float speed);

protected:
    float m_health;
    float m_speed;
    // ANIM
    uint8_t m_animID;
};
