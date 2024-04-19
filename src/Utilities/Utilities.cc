#include "Utilities.h"

using ClockRes = chrono::microseconds;

Utilities::Utilities()
{
}

bool Utilities::isAlpha(const sf::Uint32 character) const
{
    if ((character > 64 && character < 91) || (character > 96 && character < 123))
        return true;
    return false;
}


void Utilities::SetInputPos(const uint16_t width, sf::Text *input)
{
    auto inputLSize = input->getLocalBounds().getSize();
    auto inputPos = input->getPosition();
    input->setPosition(sf::Vector2f((width / 2U) - (inputLSize.x / 2U), inputPos.y));
}

void Utilities::SetTitlePos(const uint16_t width, sf::Text *title)
{
    auto titleLSize = title->getLocalBounds().getSize();
    title->setPosition(sf::Vector2f((width / 2U) - (titleLSize.x / 2U), 0.0F));
}

void Utilities::SetTitlePos(const uint16_t width, sf::Text *title, sf::Text *text)
{
    auto titleLSize = title->getLocalBounds().getSize();
    auto titlePos = title->getGlobalBounds().getPosition();
    auto textLSize = text->getLocalBounds().getSize();

    text->setPosition(sf::Vector2f((width / 2U) - (textLSize.x / 2U), (titlePos.y + titleLSize.y) + 50.0F));
}

void Utilities::SetBtnAndTextPos(const uint16_t width, sf::Sprite *btnObj, sf::Text *title, sf::Text *btntext)
{
    auto btnObjLSize = btnObj->getLocalBounds().getSize();
    auto btnObjScale = btnObj->getScale();
    auto titleLSize = title->getLocalBounds().getSize();
    auto titlePos = title->getGlobalBounds().getPosition();
    auto btnTextLSize = btntext->getLocalBounds().getSize();

    btnObj->setPosition(
        sf::Vector2f((width / 2U) - ((btnObjLSize.x * btnObjScale.x) / 2U), (titlePos.y + titleLSize.y) + 50.0F));

    auto btnObjPos = btnObj->getGlobalBounds().getPosition();
    auto btnObjSize = btnObj->getGlobalBounds().getSize();

    btntext->setPosition(sf::Vector2f(btnObjPos.x + ((btnObjSize.x / 2U) - (btnTextLSize.x / 2U)),
                                      btnObjPos.y + ((btnObjSize.y / 2U) - (btnTextLSize.y / 2U))));
}

void Utilities::SetBtnAndTextPos(const uint16_t width, sf::Sprite *btnObj, sf::Sprite *btn, sf::Text *btntext)
{
    auto btnObjLSize = btnObj->getLocalBounds().getSize();
    auto btnObjScale = btnObj->getScale();
    auto btnLSize = btn->getLocalBounds().getSize();
    auto btnPos = btn->getGlobalBounds().getPosition();
    auto btnTextLSize = btntext->getLocalBounds().getSize();

    btnObj->setPosition(
        sf::Vector2f((width / 2U) - ((btnObjLSize.x * btnObjScale.x) / 2U), (btnPos.y + btnLSize.y) + 50.0F));

    auto btnObjPos = btnObj->getGlobalBounds().getPosition();
    auto btnObjSize = btnObj->getGlobalBounds().getSize();

    btntext->setPosition(sf::Vector2f(btnObjPos.x + ((btnObjSize.x / 2U) - (btnTextLSize.x / 2U)),
                                      btnObjPos.y + ((btnObjSize.y / 2U) - (btnTextLSize.y / 2U))));
}

void Utilities::SetTextBeforeIcon(sf::Sprite &icon, sf::Text &text, sf::Sprite &prevIcon)
{
    auto prevIconPos = prevIcon.getGlobalBounds().getPosition();
    auto iconLSize = icon.getLocalBounds().getSize();
    icon.setPosition(prevIconPos.x, prevIconPos.y + iconLSize.y);

    iconLSize = icon.getLocalBounds().getSize();
    auto textLSize = text.getLocalBounds().getSize();
    auto iconPos = icon.getPosition();

    text.setPosition(sf::Vector2f(iconPos.x + (iconLSize.x + (iconLSize.x / 2)), iconPos.y));
}

void Utilities::SetTextBeforeIcon(const uint16_t width, sf::Text *title, sf::Sprite &icon, sf::Text &text)
{
    auto titleLSize = title->getLocalBounds().getSize();
    auto titlePos = title->getGlobalBounds().getPosition();
    auto iconLSize = icon.getLocalBounds().getSize();
    auto textLSize = text.getLocalBounds().getSize();

    // TODO REMOVE + 150.0F Y TEMPORARY FIX
    icon.setPosition((width / 2U) - ((iconLSize.x + textLSize.x) / 2), (titlePos.y + titleLSize.y) + 150.0F);

    auto iconPos = icon.getPosition();

    text.setPosition(sf::Vector2f(iconPos.x + (iconLSize.x + (iconLSize.x / 2)), iconPos.y));
}

void Utilities::SetTextBeforeIcon(const uint16_t x, const uint16_t y, sf::Sprite &icon, sf::Text &text)
{
    icon.setPosition(x, y);

    auto iconLSize = icon.getLocalBounds().getSize();
    auto textLSize = text.getLocalBounds().getSize();
    auto iconPos = icon.getPosition();

    text.setPosition(sf::Vector2f(iconPos.x + (iconLSize.x + (iconLSize.x / 2)), iconPos.y));
}

// RIGHT MOVE X
bool Utilities::CanMoveRightX(const float playerPos,
                              const float playerSize,
                              const float objPos,
                              const float speed) const
{
    if (playerPos + playerSize + speed >= objPos && playerPos <= objPos)
        return false;
    return true;
}

bool Utilities::CanMoveRightCollisionX(const float playerPos, const float objPos, const float speed) const
{
    if (playerPos + speed >= objPos && playerPos <= objPos)
        return false;
    return true;
}

// RIGHT LEFT MOVE Y
bool Utilities::CanMoveRightLeftY(const float playerPos,
                                  const float playerSize,
                                  const float objSize,
                                  const float objPos) const
{
    if (playerPos <= objPos + objSize && playerPos + playerSize >= objPos)
        return false;
    return true;
}

bool Utilities::CanMoveRightLeftCollisionY(const float playerPos,
                                           const float objPos,
                                           const float objSize,
                                           const uint8_t objCollision) const
{
    if (playerPos <= objPos + objSize && playerPos >= objPos + objCollision)
        return false;
    return true;
}

// LEFT MOVE X
bool Utilities::CanMoveLeftX(const float playerPos, const float objPos, const float objSize, const float speed) const
{
    if (playerPos >= objPos && playerPos - speed <= objPos + objSize)
        return false;
    return true;
}

bool Utilities::CanMoveLeftCollisionX(const float playerPos,
                                      const float objPos,
                                      const uint8_t objCollision,
                                      const float speed) const
{
    if (playerPos >= objPos && playerPos - speed <= objPos + objCollision)
        return false;
    return true;
}

// DOWN UP MOVE X
bool Utilities::CanMoveDownUpX(const float playerPos,
                               const float playerSize,
                               const float objPos,
                               const float objSize) const
{
    if (playerPos + playerSize >= objPos && playerPos <= objPos + objSize)
        return false;
    return true;
}

bool Utilities::CanMoveDownUpCollisionX(const float playerPos, const float objPos, const uint8_t objCollision) const
{
    if (playerPos >= objPos && playerPos <= objPos + objCollision)
        return false;
    return true;
}

// UP MOVE
bool Utilities::CanMoveUpY(const float playerPos,
                           const float playerSize,
                           const float objPos,
                           const float objSize,
                           const float speed) const
{
    if (playerPos - speed <= objPos + objSize && playerPos + playerSize >= objPos)
        return false;
    return true;
}

bool Utilities::CanMoveUpCollisionY(const float playerPos,
                                    const float objPos,
                                    const float objSize,
                                    const uint8_t objCollision,
                                    const float speed) const
{
    if (playerPos - speed <= objPos + objSize && playerPos >= objPos + objCollision)
        return false;
    return true;
}

// DOWN MOVE
bool Utilities::CanMoveDownY(const float playerPos,
                             const float playerSize,
                             const float objPos,
                             const float objSize,
                             const float speed) const
{
    if (playerPos <= objPos + objSize && (playerPos + playerSize) + speed >= objPos)
        return false;
    return true;
}

bool Utilities::CanMoveDownCollisionY(const float playerPos,
                                      const float objPos,
                                      const uint8_t objCollision,
                                      const float speed) const
{
    if (playerPos <= objPos + objCollision && playerPos + speed >= objPos + objCollision)
        return false;
    return true;
}

void Utilities::PerformanceTestBegin()
{
    m_startTime = ClockType::now();
}

void Utilities::PerformanceTestEnd()
{
    m_endTime = ClockType::now();
    const auto elapsed_time = chrono::duration_cast<ClockRes>(m_endTime - m_startTime).count();
    cout << "Elapsed time: " << elapsed_time << '\n';
}

bool Utilities::InViewRange(Game *game, const sf::Vector2f &spritePos)
{
    auto drawPuffer = game->GetDrawPuffer();
    auto view = game->GetView();

    auto viewCenter = view->getCenter();
    auto viewSizeX = ((view->getSize().x / 2) + drawPuffer);
    auto viewSizeY = ((view->getSize().y / 2) + drawPuffer);

    if (spritePos.x >= (viewCenter.x - viewSizeX) && spritePos.x <= (viewCenter.x + viewSizeX) &&
        spritePos.y >= (viewCenter.y - viewSizeY) && spritePos.y <= (viewCenter.y + viewSizeY))
    {
        return true;
    }
    return false;
}

void Utilities::PlayAnimation(sf::Sprite *sprite, sf::Clock &clock, sf::IntRect &anim0, sf::IntRect &anim1)
{
    auto elapsed = clock.getElapsedTime();

    if (elapsed.asMilliseconds() >= 0 && elapsed.asMilliseconds() < 200)
        sprite->setTextureRect(anim1);
    else if (elapsed.asMilliseconds() >= 200 && elapsed.asMilliseconds() < 400)
        sprite->setTextureRect(anim0);
    else
        clock.restart();
}
