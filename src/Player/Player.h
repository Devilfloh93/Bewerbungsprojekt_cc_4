#pragma once
#include "Game.h"
#include "Stats.h"
#include <SFML/Graphics.hpp>
#include <World.h>
#include <cstdint>
#include <memory>
#include <string>

class Game;
struct SurvivalStats
{
    float health;
    float water;
    float food;
};

struct MoveAllowed
{
    bool up;
    bool down;
    bool left;
    bool right;
};

enum class PlayerMove
{
    NotMoving = 0,
    Up,
    Down,
    Left,
    Right
};

class Player : public Sprite
{
public:
    Player(sf::Sprite *sprite, const uint8_t animID);
    ~Player() = default;

    // INFO
    uint8_t GetID() const;

    // MOVE
    PlayerMove GetMove() const;
    PlayerMove GetLastMove() const;
    void HandleMove(sf::Clock &clock, Game &game);
    void SetMove(const PlayerMove move);
    void SetSpeed(const float speed);

    // STATS
    float GetStatValue(const StatType type) const;
    void UpdateStats(const sf::RenderWindow &window, const Game &game);

    // ITEMS
    void UseItem(Game &game);

    // DRAW
    void DrawStats(sf::RenderWindow &window, const Game &game);
    void DrawInventoryItems(sf::RenderWindow &window, const vector<ItemCfg *> &itemCfg);

    // COLLISION
    void CheckCollision(Game &game);

    // DATASTORE
    void Load();
    void Save();

private:
    // INFO
    uint8_t m_ID;
    string m_name;
    // MOVE
    float m_baseSpeed;
    float m_speed;
    PlayerMove m_move;
    PlayerMove m_lastMove;
    MoveAllowed m_moveAllowed;
    // STATS
    SurvivalStats m_survivalStats;
    // ITEMS
    map<uint32_t, uint16_t> m_items;
    // COLLISION
    World *m_objectInFront;
    // ANIM
    uint8_t m_animID;
};
