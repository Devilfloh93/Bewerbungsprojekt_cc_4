#include "Anim.h"

Anim::Anim(const uint8_t id, const uint8_t textureID) : m_ID(id), m_textureID(textureID)
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

AnimTexture Anim::GetAnimUp() const
{
    return m_animUp;
}

AnimTexture Anim::GetAnimDown() const
{
    return m_animDown;
}

AnimTexture Anim::GetAnimLeft() const
{
    return m_animLeft;
}

AnimTexture Anim::GetAnimRight() const
{
    return m_animRight;
}

AnimTextureCombined Anim::GetAnim() const
{
    AnimTextureCombined animTexture = {.up = m_animUp, .down = m_animDown, .left = m_animLeft, .right = m_animRight};
    return animTexture;
}

void Anim::SetAnimTexture(const string_view state, const AnimTexture &animTexture)
{
    if (state == "Up")
        m_animUp = animTexture;

    if (state == "Down")
        m_animDown = animTexture;

    if (state == "Left")
        m_animLeft = animTexture;

    if (state == "Right")
        m_animRight = animTexture;
}
