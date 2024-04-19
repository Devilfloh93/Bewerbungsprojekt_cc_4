#pragma once
#include "Sprite.h"
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

using namespace std;

class World : public Sprite
{

public:
    World(sf::Sprite *sprite,
          const Collision collision,
          const vector<uint8_t> itemOutputID,
          const TextureProgData textureProgData,
          const bool saving);
    ~World() = default;

    Collision GetCollision() const;
    vector<uint8_t> GetItemOutputID() const;
    bool GetUseable() const;

    bool GetSaveIt() const;
    void SetSaveIt(const bool saving);

    void UpdateTextureRect();
    void UpdatePosition();
    void SetUseable(const bool useable);

private:
    Collision m_collision;
    vector<uint8_t> m_itemOutputID;
    TextureProgData m_textureProgData;
    bool m_saveIt;
    bool m_useable;
};
