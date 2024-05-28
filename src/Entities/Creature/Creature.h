#pragma once
#include "Unit.h"
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

enum class Fraction
{
    Neutral = 0,
    Friend,
    Enemy
};

class Creature : public Unit
{

public:
    Creature(unique_ptr<sf::Sprite> sprite,
             const float health,
             const float speed,
             const uint8_t animID,
             const bool moveable,
             const vector<string> dialogIntro,
             const vector<string> dialogOutro,
             const vector<string> dialogOffensive,
             const bool interactable,
             const float maxMoveRange,
             const Fraction fraction,
             const Guid guid);
    ~Creature() = default;

    bool GetInteractable() const;
    bool GetMoveable() const;

    sf::Vector2f GetSpawnPos() const;
    float GetMaxMoveRange() const;

    Fraction GetFraction() const;

    vector<string> GetDialogIntro() const;
    vector<string> GetDialogOutro() const;
    vector<string> GetDialogOffensive() const;

private:
    bool m_interactable;
    bool m_moveable;
    float m_maxMoveRange;
    sf::Vector2f m_spawnPos;
    vector<string> m_dialogIntro;
    vector<string> m_dialogOutro;
    vector<string> m_dialogOffensive;
    Fraction m_fraction;
};
