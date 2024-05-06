#include "Texture.h"

Texture::Texture(unique_ptr<sf::Texture> texture, const uint8_t ID) : m_texture(move(texture)), m_ID(ID)
{
}

uint8_t Texture::GetID() const
{
    return m_ID;
}

const sf::Texture *Texture::GetTexture() const
{
    return m_texture.get();
}
