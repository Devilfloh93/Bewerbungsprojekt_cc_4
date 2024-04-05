#pragma once
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

using namespace std;

class World
{

public:
    World(const sf::Sprite &sprite,
          const Collision collision,
          const vector<uint8_t> itemOutputID,
          const TextureProgData textureProgData);
    ~World() = default;

    sf::Sprite GetSprite() const;
    Collision GetCollision() const;
    vector<uint8_t> GetItemOutputID() const;
    bool GetUseable() const;

    void UpdateTextureRect();
    void UpdatePosition();
    void SetUseable(const bool useable);

private:
    sf::Sprite m_sprite;
    Collision m_collision;
    vector<uint8_t> m_itemOutputID;
    TextureProgData m_textureProgData;
    bool m_useable;
};
