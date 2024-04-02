#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

struct MovementTexture
{
    sf::IntRect up00; // Not Moving
    sf::IntRect up01; // Left Moving Feet
    sf::IntRect up02; // Right Moving Feet
    sf::IntRect down00;
    sf::IntRect down01;
    sf::IntRect down02;
    sf::IntRect left00;
    sf::IntRect left01;
    sf::IntRect left02;
    sf::IntRect right00;
    sf::IntRect right01;
    sf::IntRect right02;
};

class Anim
{

public:
    Anim(const uint8_t id, const uint8_t textureID, const MovementTexture moveAnim);
    ~Anim() = default;

    uint8_t GetID() const;
    uint8_t GetTextureID() const;
    MovementTexture GetMoveAnim() const;

private:
    uint8_t m_ID;
    uint8_t m_textureID;
    MovementTexture m_moveAnim;
};
