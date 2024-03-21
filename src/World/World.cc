#include "World.h"

World::World(const sf::Sprite &sprite,
             const Collision collision,
             const std::uint8_t itemOutputID,
             const TextureProgData textureProgData)
    : m_sprite(sprite), m_collision(collision), m_itemOutputID(itemOutputID), m_textureProgData(textureProgData)
{
    if (itemOutputID > 0)
    {
        m_useable = true;
    }
    else
    {
        m_useable = false;
    }
}

sf::Sprite World::GetSprite() const
{
    return m_sprite;
}

Collision World::GetCollision() const
{
    return m_collision;
}

std::uint8_t World::GetItemOutputID() const
{
    return m_itemOutputID;
}

void World::UpdateTextureRect()
{
    if (m_textureProgData.rect.getSize().x > 0 && m_textureProgData.rect.getSize().y > 0)
        m_sprite.setTextureRect(m_textureProgData.rect);
}

bool World::GetUseable() const
{
    return m_useable;
}

void World::SetUseable(const bool useable)
{
    m_useable = useable;
}

void World::UpdatePosition()
{
    auto pos = m_sprite.getPosition();
    auto size = m_sprite.getLocalBounds().getSize();
    auto textureSize = m_textureProgData.rect.getSize();

    /**
     * TODO: Rework SetPosition calculate when texture gets overridden by other texture
    */
    m_sprite.setPosition(pos.x + ((size.x / 2) - (textureSize.x / 2)), pos.y + (size.y - textureSize.y));
    m_collision.x = m_textureProgData.collision.x;
    m_collision.y = m_textureProgData.collision.y;
}
