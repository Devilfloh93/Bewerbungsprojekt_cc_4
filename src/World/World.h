#pragma once
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>

class World
{

public:
    World(const sf::Sprite &sprite,
          const Collision collision,
          const std::uint8_t itemOutputID,
          const TextureProgData textureProgData);
    ~World() = default;

    sf::Sprite GetSprite() const;
    Collision GetCollision() const;
    std::uint8_t GetItemOutputID() const;

    void UpdateTextureRect();
    void UpdatePosition();

private:
    sf::Sprite m_sprite;
    Collision m_collision;
    std::uint8_t m_itemOutputID;
    TextureProgData m_textureProgData;
};

void InitWorld(std::vector<std::unique_ptr<World>> &world, const std::vector<std::unique_ptr<Texture>> &textures);

void DrawWorld(sf::RenderWindow &window, const std::vector<std::unique_ptr<World>> &world);
