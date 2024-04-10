#pragma once
#include "Game.h"
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

    PlayerMove GetMove() const;
    PlayerMove GetLastMove() const;
    void CheckCollision(Game &game);
    void InitDrawStats(const Game &game);

    void DrawStats(sf::RenderWindow &window, const Game &game);
    void UpdateStats(const sf::RenderWindow &window, const Game &game);
    void SetMove(const PlayerMove move);
    void SetSpeed(const float speed);

    void HandleMove(sf::Clock &clock, Game &game);

    void UseItem(Game &game);

    void DrawInventoryItems(sf::RenderWindow &window, const vector<ItemCfg *> &itemCfg);

    void Load();
    void Save();
    void CreateFolder();

private:
    string m_name;
    SurvivalStats m_survivalStats;
    float m_baseSpeed;
    float m_speed;
    PlayerMove m_move;
    PlayerMove m_lastMove;
    map<uint32_t, uint16_t> m_items;
    vector<sf::Sprite *> m_statSprites;
    vector<sf::Sprite *> m_statBackgroundSprites;
    uint8_t m_animID;
    MoveAllowed m_moveAllowed;
    World *m_objectInFront;
    uint8_t m_stateTextureSize;
};
