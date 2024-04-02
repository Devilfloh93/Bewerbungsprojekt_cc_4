#include "Anim.h"

Anim::Anim(const uint8_t id, const uint8_t textureID, const MovementTexture moveAnim)
    : m_ID(id), m_textureID(textureID), m_moveAnim(moveAnim)
{
}

uint8_t Anim::GetID() const
{
    return m_ID;
}

uint8_t Anim::GetTextureID() const
{
    return m_textureID;
}

MovementTexture Anim::GetMoveAnim() const
{
    return m_moveAnim;
}
