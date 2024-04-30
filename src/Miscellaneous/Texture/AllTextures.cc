#include "AllTextures.h"

AllTextures::AllTextures(sf::Texture *texture, const uint8_t ID) : Texture(texture), m_ID(ID)
{
}


uint8_t AllTextures::GetID() const
{
    return m_ID;
}
