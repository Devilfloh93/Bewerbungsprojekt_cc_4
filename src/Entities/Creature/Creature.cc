#include "Creature.h"

Creature::Creature(sf::Sprite *sprite,
                   const float health,
                   const float speed,
                   const uint8_t animID,
                   const bool moving,
                   const vector<string> dialogIntro,
                   const vector<string> dialogOutro,
                   const vector<string> dialogOffensive)
    : Unit(sprite, health, speed, animID), m_moving(moving), m_dialogIntro(dialogIntro), m_dialogOutro(dialogOutro),
      m_dialogOffensive(dialogOffensive)
{
    m_useable = false;
}

bool Creature::GetUseable() const
{
    return m_useable;
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
