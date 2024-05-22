#include "Creature.h"

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

Creature::Creature(unique_ptr<sf::Sprite> sprite,
                   const float health,
                   const float speed,
                   const uint8_t animID,
                   const bool moveable,
                   const vector<string> dialogIntro,
                   const vector<string> dialogOutro,
                   const vector<string> dialogOffensive,
                   const bool interactable,
                   const float maxMoveRange)
    : Unit(move(sprite), health, speed, animID), m_moveable(moveable), m_dialogIntro(dialogIntro),
      m_dialogOutro(dialogOutro), m_dialogOffensive(dialogOffensive), m_interactable(interactable),
      m_maxMoveRange(maxMoveRange)
{
    m_spawnPos = m_sprite.get()->getPosition();
}

bool Creature::GetInteractable() const
{
    return m_interactable;
}

bool Creature::GetMoveable() const
{
    return m_moveable;
}

sf::Vector2f Creature::GetSpawnPos() const
{
    return m_spawnPos;
}

float Creature::GetMaxMoveRange() const
{
    return m_maxMoveRange;
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
