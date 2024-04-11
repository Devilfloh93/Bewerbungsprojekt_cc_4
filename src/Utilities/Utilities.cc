#include "Utilities.h"

Utilities::Utilities()
{
}

void Utilities::SetTitlePos(const uint16_t width, sf::Text *title)
{
    auto titleLSize = title->getLocalBounds().getSize();
    title->setPosition(sf::Vector2f((width / 2U) - (titleLSize.x / 2U), 0.0F));
}

void Utilities::SetInputPos(const uint16_t width, sf::Text *input)
{
    auto inputLSize = input->getLocalBounds().getSize();
    input->setPosition(sf::Vector2f((width / 2U) - (inputLSize.x / 2U), input->getPosition().y));
}

void Utilities::SetTitlePos(const uint16_t width, sf::Text *title, sf::Text *text)
{
    auto titleLSize = title->getLocalBounds().getSize();
    auto titlePos = title->getGlobalBounds().getPosition();

    text->setPosition(sf::Vector2f((width / 2U), (titlePos.y + titleLSize.y) + 50.0F));
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

void Utilities::SetTextBeforeIcon(const uint16_t x, const uint16_t y, sf::Sprite &icon, sf::Text &text)
{
    icon.setPosition(x, y);

    auto iconLSize = icon.getLocalBounds().getSize();
    auto textLSize = text.getLocalBounds().getSize();
    auto iconPos = icon.getPosition();

    text.setPosition(sf::Vector2f(iconPos.x + (iconLSize.x + (iconLSize.x / 2)), iconPos.y));
}
