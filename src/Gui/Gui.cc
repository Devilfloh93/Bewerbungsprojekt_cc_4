#include "Gui.h"

void SetTitlePos(sf::Text &text)
{
    text.setPosition(sf::Vector2f((gameWidth / 2U) - (text.getLocalBounds().getSize().x / 2U), 0.0F));
}

void SetBtnAndTextPos(sf::Sprite &btnObj, sf::Text &title, sf::Text &btntext)
{
    btnObj.setPosition(
        sf::Vector2f((gameWidth / 2U) - ((btnObj.getLocalBounds().getSize().x * btnObj.getScale().x) / 2U),
                     (title.getGlobalBounds().getPosition().y + title.getLocalBounds().getSize().y) + 50.0F));

    btntext.setPosition(
        sf::Vector2f(btnObj.getGlobalBounds().getPosition().x +
                         ((btnObj.getGlobalBounds().getSize().x / 2U) - (btntext.getLocalBounds().getSize().x / 2U)),
                     btnObj.getGlobalBounds().getPosition().y +
                         ((btnObj.getGlobalBounds().getSize().y / 2U) - (btntext.getLocalBounds().getSize().y / 2U))));
}

void SetBtnAndTextPos(sf::Sprite &btnObj, sf::Sprite &button, sf::Text &btntext)
{
    btnObj.setPosition(
        sf::Vector2f((gameWidth / 2U) - ((btnObj.getLocalBounds().getSize().x * btnObj.getScale().x) / 2U),
                     (button.getGlobalBounds().getPosition().y + button.getLocalBounds().getSize().y) + 50.0F));

    btntext.setPosition(
        sf::Vector2f(btnObj.getGlobalBounds().getPosition().x +
                         ((btnObj.getGlobalBounds().getSize().x / 2U) - (btntext.getLocalBounds().getSize().x / 2U)),
                     btnObj.getGlobalBounds().getPosition().y +
                         ((btnObj.getGlobalBounds().getSize().y / 2U) - (btntext.getLocalBounds().getSize().y / 2U))));
}
