#pragma once
#include "Sprite.h"
#include <SFML/Graphics.hpp>
#include <cstdint>

using namespace std;

class ItemGround : public Sprite
{
public:
    ItemGround(sf::Sprite *sprite, const uint8_t ID, const uint16_t count);
    ~ItemGround() = default;

    uint8_t GetID() const;
    uint16_t GetCount() const;

private:
    uint8_t m_ID;
    uint16_t m_count;
};
