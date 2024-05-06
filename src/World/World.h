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
    World(unique_ptr<sf::Sprite> sprite,
          const uint8_t id,
          const CollisionData collision,
          const vector<uint8_t> itemOutputID,
          const TextureProgData textureProgData,
          const bool saving);
    ~World() = default;

    CollisionData GetCollision() const;
    vector<uint8_t> GetItemOutputID() const;
    bool GetInteractable() const;
    uint8_t GetID() const;

    bool GetSaveIt() const;
    void SetSaveIt(const bool saving);

    void UpdateTextureRect();
    void UpdatePosition();
    void SetInteractable(const bool interactable);

private:
    uint8_t m_ID;
    CollisionData m_collision;
    vector<uint8_t> m_itemOutputID;
    TextureProgData m_textureProgData;
    bool m_saveIt;
    bool m_interactable;
};
