#pragma once
#include "Unit.h"
#include <cstdint>
#include <string>
#include <vector>

using namespace std;


class Creature : public Unit
{

public:
    Creature(unique_ptr<sf::Sprite> sprite,
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
