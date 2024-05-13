#include "Utilities.h"
#include <SFML/Graphics.hpp>

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

void Utilities::SetTitlePos(const uint16_t width, sf::Text *title, sf::Text *text, float spaceBetweenInputs)
{
    auto titleLSize = title->getLocalBounds().getSize();
    auto titlePos = title->getGlobalBounds().getPosition();
    auto textLSize = text->getLocalBounds().getSize();

    text->setPosition(
        sf::Vector2f((width / 2U) - (textLSize.x / 2U), (titlePos.y + titleLSize.y) + spaceBetweenInputs));
}

void Utilities::SetBtnAndTextPos(const uint16_t width,
                                 sf::Sprite *btnObj,
                                 sf::Text *title,
                                 sf::Text *btntext,
                                 float spaceBetweenBtn)
{
    auto btnObjLSize = btnObj->getLocalBounds().getSize();
    auto btnObjScale = btnObj->getScale();
    auto titleLSize = title->getLocalBounds().getSize();
    auto titlePos = title->getGlobalBounds().getPosition();
    auto btnTextLSize = btntext->getLocalBounds().getSize();

    btnObj->setPosition(sf::Vector2f((width / 2U) - ((btnObjLSize.x * btnObjScale.x) / 2U),
                                     (titlePos.y + titleLSize.y) + spaceBetweenBtn));

    auto btnObjPos = btnObj->getGlobalBounds().getPosition();
    auto btnObjSize = btnObj->getGlobalBounds().getSize();

    btntext->setPosition(sf::Vector2f(btnObjPos.x + ((btnObjSize.x / 2U) - (btnTextLSize.x / 2U)),
                                      btnObjPos.y + ((btnObjSize.y / 2U) - (btnTextLSize.y / 2U))));
}

void Utilities::SetBtnAndTextPos(const uint16_t width,
                                 sf::Sprite *btnObj,
                                 sf::Sprite *btn,
                                 sf::Text *btntext,
                                 float spaceBetweenBtn)
{
    auto btnObjLSize = btnObj->getLocalBounds().getSize();
    auto btnObjScale = btnObj->getScale();
    auto btnLSize = btn->getLocalBounds().getSize();
    auto btnPos = btn->getGlobalBounds().getPosition();
    auto btnTextLSize = btntext->getLocalBounds().getSize();

    if (spaceBetweenBtn > 0)
    {
        btnObj->setPosition(sf::Vector2f((width / 2U) - ((btnObjLSize.x * btnObjScale.x) / 2U),
                                         (btnPos.y + btnLSize.y) + spaceBetweenBtn));
    }
    else
    {
        btnObj->setPosition(sf::Vector2f((width / 2U) - ((btnObjLSize.x * btnObjScale.x) / 2U), btnPos.y));
    }

    auto btnObjPos = btnObj->getGlobalBounds().getPosition();
    auto btnObjSize = btnObj->getGlobalBounds().getSize();

    btntext->setPosition(sf::Vector2f(btnObjPos.x + ((btnObjSize.x / 2U) - (btnTextLSize.x / 2U)),
                                      btnObjPos.y + ((btnObjSize.y / 2U) - (btnTextLSize.y / 2U))));
}

void Utilities::SetTextBeforeIcon(sf::Sprite &icon, sf::Text &text, const sf::Vector2f &prevPos)
{
    auto iconLSize = icon.getLocalBounds().getSize();
    icon.setPosition(prevPos.x, prevPos.y + iconLSize.y);

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

    // TODO REMOVE + 50.0F Y TEMPORARY FIX
    icon.setPosition((width / 2U) - ((iconLSize.x + textLSize.x) / 2), (titlePos.y + titleLSize.y) + 50.0F);

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

uint16_t Utilities::CalculateAlignmentWindowWidth(const uint16_t width, const Alignment alignment)
{
    auto alignmentWidth = width;

    switch (alignment)
    {
    case Alignment::Left:
        alignmentWidth -= (alignmentWidth / 2);
        break;
    case Alignment::Right:
        alignmentWidth += (alignmentWidth / 2);
        break;

    default:
        break;
    }

    return alignmentWidth;
}

float Utilities::CalculateSpaceBetweenMenu(const Alignment alignment)
{
    float spaceBetweenBtn = 50.0F;

    if (alignment != Alignment::Middle)
        spaceBetweenBtn = 0.0F;

    return spaceBetweenBtn;
}

void Utilities::PlayAnimation(sf::Sprite *sprite, sf::Clock &clock, sf::IntRect &anim0, sf::IntRect &anim1)
{
    auto elapsed = clock.getElapsedTime();
    auto elapsedAsMs = elapsed.asMilliseconds();

    if (elapsedAsMs >= 0 && elapsedAsMs < 200)
        sprite->setTextureRect(anim1);
    else if (elapsedAsMs >= 200 && elapsedAsMs < 400)
        sprite->setTextureRect(anim0);
    else
        clock.restart();
}


void Utilities::SetSFText(sf::Text *text, const sf::Font *font, const uint8_t size)
{
    text->setFont(*font);
    text->setCharacterSize(size);
}

void Utilities::SetSFSprite(sf::Sprite *sprite, const sf::Texture *texture, const sf::IntRect &rectangle)
{
    sprite->setTexture(*texture);
    sprite->setTextureRect(rectangle);
}

void Utilities::SetSFSprite(sf::Sprite *sprite,
                            const sf::Texture *texture,
                            const sf::IntRect &rectangle,
                            float x,
                            float y)
{
    sprite->setTexture(*texture);
    sprite->setTextureRect(rectangle);
    sprite->setPosition(x, y);
}

void Utilities::SetSFSprite(sf::Sprite *sprite,
                            const sf::Texture *texture,
                            const sf::IntRect &rectangle,
                            const sf::Vector2f &factors)
{
    sprite->setTexture(*texture);
    sprite->setTextureRect(rectangle);
    sprite->setScale(factors);
}

const sf::Font *Utilities::GetFont(const vector<Font *> &fonts, const uint8_t fontID) const
{
    for (const auto &data : fonts)
    {
        if (fontID == data->GetID())
        {
            return data->GetFont();
        }
    }
    return nullptr;
}

const sf::Texture *Utilities::GetTexture(const vector<Texture *> &textures, const uint8_t textureID) const
{
    for (const auto &data : textures)
    {
        if (data->GetID() == textureID)
        {
            return data->GetTexture();
        }
    }
    return nullptr;
}

AnimTextureCombined Utilities::GetAnim(const vector<Anim *> &anim, const uint8_t animID)
{
    AnimTextureCombined animData;
    for (const auto &data : anim)
    {
        if (data->GetID() == animID)
        {
            animData = data->GetAnim();
            break;
        }
    }
    return animData;
}

const sf::Texture *Utilities::GetAnimTexture(const vector<Anim *> &anim,
                                             const vector<Texture *> &textures,
                                             const uint8_t animID) const
{
    for (const auto &data : anim)
    {
        if (data->GetID() == animID)
        {
            auto textureID = data->GetTextureID();

            return GetTexture(textures, textureID);
        }
    }
    return nullptr;
}

string Utilities::GetLanguageText(const json &jsonData, const uint8_t languageID, string_view language)
{
    string text;
    for (const auto &data : jsonData)
    {
        uint8_t jsonLanguageId = data["id"];
        if (jsonLanguageId == languageID)
        {
            text = data[language]["title"];
            break;
        }
    }
    return text;
}

bool Utilities::CheckMenuState(const vector<MenuState> &menuState, const MenuState currentState)
{
    bool check = false;
    for (const auto &data : menuState)
    {
        if (currentState == data)
        {
            check = true;
            break;
        }
    }
    return check;
}
