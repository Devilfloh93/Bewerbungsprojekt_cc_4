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

class Player : public Unit
{
public:
    Player(sf::Sprite *sprite, const uint8_t animID, const string_view name, const uint8_t id);
    Player(sf::Sprite *sprite, const uint8_t animID, const uint8_t id);
    ~Player() = default;

    // INIT
    void Init();

    // INFO
    uint8_t GetID() const;

    // MOVE
    PlayerMove GetMove() const;
    PlayerMove GetLastMove() const;
    void HandleMove(sf::Clock &clock, Game *game);
    void SetMove(const PlayerMove move);

    // STATS
    float GetStatValue(const StatType type) const;
    void UpdateStats(Game *game);

    // ITEMS
    void Interact(Game &game);
    void CollectItem(Game *game);

    // DRAW
    void DrawStats(sf::RenderWindow &window, Game *game);
    void DrawInventoryItems(sf::RenderWindow &window,
                            const vector<ItemCfg *> &itemCfg,
                            sf::Text *previousTxt,
                            const uint16_t width);

    // COLLISION
    void CheckCollision(Game *game);
    bool CheckMove(const bool isUsable, const sf::Vector2f &objPos, const sf::Vector2f &objSize);
    bool CheckMove(const bool isUsable,
                   const sf::Vector2f &objPos,
                   const sf::Vector2f &objSize,
                   const Collision objCollision);

    // DATASTORE
    void Load(const uint8_t id, Game *game);
    void Save();

private:
    // INFO
    uint8_t m_ID;
    string m_name;
    // MOVE
    PlayerMove m_move;
    PlayerMove m_lastMove;
    MoveAllowed m_moveAllowed;
    // STATS
    SurvivalStats m_survivalStats;
    // ITEMS
    map<uint32_t, uint16_t> m_items;
    // COLLISION
    World *m_objectInFront;
    Creature *m_creatureInFront;
};
