#include "World.h"

World::World(unique_ptr<sf::Sprite> sprite,
             const uint8_t id,
             const CollisionData collision,
             const vector<uint8_t> itemOutputID,
             const TextureProgData textureProgData,
             const bool saving)
    : Sprite(move(sprite)), m_ID(id), m_collision(collision), m_itemOutputID(itemOutputID),
      m_textureProgData(textureProgData), m_saveIt(saving)
{
    if (itemOutputID.size() > 0 && !m_saveIt)
        m_interactable = true;
    else
        m_interactable = false;
}

CollisionData World::GetCollision() const
{
    return m_collision;
}

vector<uint8_t> World::GetItemOutputID() const
{
    return m_itemOutputID;
}

void World::UpdateTextureRect()
{
    if (m_textureProgData.rect.getSize().x > 0 && m_textureProgData.rect.getSize().y > 0)
        m_sprite->setTextureRect(m_textureProgData.rect);
}

uint8_t World::GetID() const
{
    return m_ID;
}

bool World::GetInteractable() const
{
    return m_interactable;
}

bool World::GetSaveIt() const
{
    return m_saveIt;
}

void World::SetSaveIt(const bool saving)
{
    m_saveIt = saving;
}

void World::SetInteractable(const bool interactable)
{
    m_interactable = interactable;
}

void World::UpdatePosition()
{
    auto pos = m_sprite->getPosition();
    auto size = m_sprite->getLocalBounds().getSize();
    auto textureSize = m_textureProgData.rect.getSize();

    /**
     * TODO: Rework SetPosition calculate when texture gets overridden by other texture
    */
    m_sprite->setPosition(pos.x + ((size.x / 2) - (textureSize.x / 2)), pos.y + (size.y - textureSize.y));
    m_collision.x = m_textureProgData.collision.x;
    m_collision.y = m_textureProgData.collision.y;
}
