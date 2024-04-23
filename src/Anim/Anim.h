#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>

using namespace std;

struct AnimTexture
{
    sf::IntRect notMoving; // Not Moving
    sf::IntRect anim01;    // Moving Feet 01
    sf::IntRect anim02;    // Moving Feet 02
};

struct AnimTextureCombined
{
    AnimTexture up;
    AnimTexture down;
    AnimTexture left;
    AnimTexture right;
};

class Anim
{

public:
    Anim(const uint8_t id, const uint8_t textureID);
    ~Anim() = default;

    uint8_t GetID() const;
    uint8_t GetTextureID() const;
    AnimTexture GetAnimUp() const;
    AnimTexture GetAnimDown() const;
    AnimTexture GetAnimLeft() const;
    AnimTexture GetAnimRight() const;
    AnimTextureCombined GetAnim() const;

    void SetAnimTexture(const string_view state, const AnimTexture &animTexture);

private:
    uint8_t m_ID;
    uint8_t m_textureID;
    AnimTexture m_animUp;
    AnimTexture m_animDown;
    AnimTexture m_animLeft;
    AnimTexture m_animRight;
};
