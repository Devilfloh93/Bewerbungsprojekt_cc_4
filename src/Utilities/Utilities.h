#pragma once
#include "Game.h"
#include "nlohmann/json.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using json = nlohmann::json;
using namespace std;
using ClockType = chrono::steady_clock;

class Utilities
{

public:
    Utilities();
    ~Utilities() = default;

    bool isAlpha(const sf::Uint32 character) const;

    void SetTitlePos(const uint16_t width, sf::Text *text);
    void SetTitlePos(const uint16_t width, sf::Text *title, sf::Text *text);
    void SetInputPos(const uint16_t width, sf::Text *input);
    void SetBtnAndTextPos(const uint16_t width, sf::Sprite *btnObj, sf::Text *title, sf::Text *btntext);
    void SetBtnAndTextPos(const uint16_t width, sf::Sprite *btnObj, sf::Sprite *btn, sf::Text *btntext);
    void SetTextBeforeIcon(sf::Sprite &icon, sf::Text &text, sf::Sprite &prevIcon);
    void SetTextBeforeIcon(const uint16_t x, const uint16_t y, sf::Sprite &icon, sf::Text &text);
    void SetTextBeforeIcon(const uint16_t width, sf::Text *title, sf::Sprite &icon, sf::Text &text);

    // RIGHT MOVE X
    bool CanMoveRightX(const float playerPos, const float playerSize, const float objPos, const float speed) const;
    bool CanMoveRightCollisionX(const float playerPos, const float objPos, const float speed) const;

    // RIGHT LEFT  MOVE Y
    bool CanMoveRightLeftY(const float playerPos,
                           const float playerSize,
                           const float objSize,
                           const float objPos) const;
    bool CanMoveRightLeftCollisionY(const float playerPos,
                                    const float objPos,
                                    const float objSize,
                                    const uint8_t objCollision) const;

    // LEFT MOVE X
    bool CanMoveLeftX(const float playerPos, const float objPos, const float objSize, const float speed) const;
    bool CanMoveLeftCollisionX(const float playerPos,
                               const float objPos,
                               const uint8_t objCollision,
                               const float speed) const;

    // DOWN UP MOVE X
    bool CanMoveDownUpX(const float playerPos, const float playerSize, const float objPos, const float objSize) const;
    bool CanMoveDownUpCollisionX(const float playerPos, const float objPos, const uint8_t objCollision) const;

    // UP MOVE
    bool CanMoveUpY(const float playerPos,
                    const float playerSize,
                    const float objPos,
                    const float objSize,
                    const float speed) const;
    bool CanMoveUpCollisionY(const float playerPos,
                             const float objPos,
                             const float objSize,
                             const uint8_t objCollision,
                             const float speed) const;

    // DOWN MOVE
    bool CanMoveDownY(const float playerPos,
                      const float playerSize,
                      const float objPos,
                      const float objSize,
                      const float speed) const;
    bool CanMoveDownCollisionY(const float playerPos,
                               const float objPos,
                               const uint8_t objCollision,
                               const float speed) const;

    void PerformanceTestBegin();
    void PerformanceTestEnd();

    bool CheckInViewRange(Game *game, const sf::Vector2f &spritePos);

private:
    ClockType::time_point m_startTime;
    ClockType::time_point m_endTime;
};
