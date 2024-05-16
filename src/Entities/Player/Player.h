#pragma once
#include "Game.h"
#include "Stats.h"
#include "Trader.h"
#include <SFML/Graphics.hpp>
#include <World.h>
#include <cstdint>
#include <string>

class Game;
class Trader;
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
    Player(unique_ptr<sf::Sprite> sprite, const uint8_t animID, const string_view name, const uint8_t id);
    Player(unique_ptr<sf::Sprite> sprite, const uint8_t animID, const uint8_t id);
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
    void AddItem(const uint8_t ID, const uint16_t count);
    bool RemoveItem(const uint8_t ID, const uint16_t count);
    uint16_t GetItemCount(const uint8_t ID);

    // Render
    void CheckRenderHotkey(Game *game);

    void InitInventoryItems(Game &game);
    void InitTraderItems(Game &game);

    // COLLISION
    void CheckCollision(Game *game);
    bool CheckInFront(const bool interactable, const sf::Vector2f &objPos, const sf::Vector2f &objSize);
    bool CheckInFront(const bool interactable,
                      const sf::Vector2f &objPos,
                      const sf::Vector2f &objSize,
                      const CollisionData objCollision);

    // DATASTORE
    void Load(const uint8_t id, Game *game);
    void Save(Game *game);

    // TRADER
    Trader *GetTrader();

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
    map<uint8_t, uint16_t> m_items;
    // COLLISION
    World *m_objectInFront;
    Creature *m_creatureInFront;
    // TRADER
    Trader *m_trader;
};
