#pragma once
#include "Gui.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Sprite
{
public:
    Sprite(unique_ptr<sf::Sprite> sprite);
    ~Sprite() = default;

    sf::Sprite *GetSprite() const;

protected:
    unique_ptr<sf::Sprite> m_sprite;
};

class SelectableSprite : public Sprite
{
public:
    SelectableSprite(unique_ptr<sf::Sprite> sprite,
                     const uint16_t selectedTextID,
                     const SelectedTextCategorie selectedCategorie);
    ~SelectableSprite() = default;

    uint16_t GetSelectedTextID() const;
    SelectedTextCategorie GetSelectedCategorie() const;

protected:
    uint16_t m_selectedTextID;
    SelectedTextCategorie m_selectedCategorie;
};
