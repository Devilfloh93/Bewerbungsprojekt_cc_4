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

bool Utilities::isNum(const sf::Uint32 character) const
{
    if ((character > 47 && character < 58))
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

void Utilities::SetSpriteAndTextPos(const uint16_t width,
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

bool Utilities::UpdateSpriteAndText(const bool firstMenuEntry,
                                    const Alignment alignment,
                                    const uint16_t width,
                                    sf::Sprite *sprite,
                                    sf::Text *prevText,
                                    sf::Sprite *prevSprite,
                                    sf::Text *text,
                                    const bool useDefaultSpaceBetweenBtns)
{
    auto windowWidth = CalculateAlignmentWindowWidth(width, alignment);
    float spaceBetween = 0.0F;

    if (useDefaultSpaceBetweenBtns)
        spaceBetween = CalculateSpaceBetweenMenu(Alignment::Middle);
    else
        spaceBetween = CalculateSpaceBetweenMenu(alignment);

    if (firstMenuEntry)
        SetSpriteAndTextPos(windowWidth, sprite, prevText, text, spaceBetween);
    else
        SetSpriteAndTextPos(windowWidth, sprite, prevSprite, text, spaceBetween);

    return false;
}

void Utilities::SetSpriteAndTextPos(const uint16_t width,
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
        btnObj->setPosition(sf::Vector2f((width / 2U) - ((btnObjLSize.x * btnObjScale.x) / 2U), btnPos.y));

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

uint16_t Utilities::CalculateAlignmentWindowWidth(const uint16_t width, const Alignment alignment) const
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

float Utilities::CalculateSpaceBetweenMenu(const Alignment alignment) const
{

    if (alignment != Alignment::Middle)
        return 0.0F;

    return 50.0F;
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

void Utilities::SetSFText(sf::Text *text,
                          const uint8_t size,
                          const vector<Font *> &fonts,
                          const uint8_t fontID,
                          const nlohmann::json_abi_v3_11_2::json &jsonDataLanguage,
                          const uint8_t languageID,
                          string_view language)
{
    auto font = GetFont(fonts, fontID);
    auto languageText = GetLanguageText(jsonDataLanguage, languageID, language);

    text->setFont(*font);
    text->setCharacterSize(size);
    text->setString(format("{}", languageText));
}

void Utilities::SetSFText(sf::Text *text, const uint8_t size, const vector<Font *> &fonts, const uint8_t fontID)
{
    auto font = GetFont(fonts, fontID);

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
                            const vector<Texture *> &textures,
                            const uint8_t textureID,
                            const sf::IntRect &rectangle,
                            float x,
                            float y)
{
    auto texture = GetTexture(textures, textureID);

    sprite->setTexture(*texture);
    sprite->setTextureRect(rectangle);
    sprite->setPosition(x, y);
}

void Utilities::SetSFSprite(sf::Sprite *sprite,
                            const vector<Texture *> &textures,
                            const uint8_t textureID,
                            const sf::IntRect &rectangle,
                            const sf::Vector2f &factors)
{
    auto texture = GetTexture(textures, textureID);

    sprite->setTexture(*texture);
    sprite->setTextureRect(rectangle);
    sprite->setScale(factors);
}

const sf::Font *Utilities::GetFont(const vector<Font *> &fonts, const uint8_t fontID) const
{
    for (const auto &data : fonts)
    {
        if (fontID == data->GetID())
            return data->GetFont();
    }
    return nullptr;
}

const sf::Texture *Utilities::GetTexture(const vector<Texture *> &textures, const uint8_t textureID) const
{
    for (const auto &data : textures)
    {
        if (data->GetID() == textureID)
            return data->GetTexture();
    }
    return nullptr;
}

sf::Text *Utilities::GetTitle(const vector<Title *> &titles, const MenuState menuState) const
{
    for (const auto &data : titles)
    {
        if (data->GetMenuState() == menuState)
            return data->GetText();
    }
    return nullptr;
}

sf::Text *Utilities::GetInput(const vector<Input *> &inputs, const MenuState menuState) const
{
    sf::Text *input = nullptr;
    for (const auto &data : inputs)
    {
        if (data->GetMenuState() == menuState)
            input = data->GetText();
    }
    return input;
}

AnimTextureCombined Utilities::GetAnim(const vector<Anim *> &anim, const uint8_t animID) const
{
    AnimTextureCombined animData;
    for (const auto &data : anim)
    {
        if (data->GetID() == animID)
            animData = data->GetAnim();
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

string Utilities::GetLanguageText(const json &jsonData, const uint8_t languageID, string_view language) const
{
    for (const auto &data : jsonData)
    {
        uint8_t jsonLanguageId = data["id"];
        if (jsonLanguageId == languageID)
            return data[language]["title"];
    }
    return "";
}

string_view Utilities::GetItemName(const Game &game, const uint8_t itemID) const
{
    auto itemCfg = game.GetItemCfg();

    for (const auto &data : itemCfg)
    {
        auto ID = data->GetID();
        if (ID == itemID)
            return data->GetName();
    }

    return "";
}

Hotkey Utilities::GetHotkey(const Game &game, const sf::Keyboard::Key &key) const
{
    auto hotkeys = game.GetHotkeys();

    for (const auto &data : hotkeys)
    {
        auto hotkey = static_cast<Hotkey>(data.first);
        auto keyID = data.second;

        if (key == keyID)
        {
            return hotkey;
        }
    }
    return Hotkey::Unknown;
}

ItemRemoved Utilities::RemoveItem(map<uint8_t, uint16_t> &items, const uint8_t ID, const uint16_t count)
{
    for (auto &[key, value] : items)
    {
        if (key == ID && value >= count)
        {
            uint16_t newValue = value - count;
            if (newValue > 0)
            {
                value = newValue;
                return ItemRemoved::Updated;
            }
            else
            {
                items.erase(key);
                return ItemRemoved::Removed;
            }
        }
    }

    return ItemRemoved::Failed;
}

string Utilities::GetMessageFormat(const Game &game, const uint16_t messageFormatID)
{
    auto messageFormat = game.GetMessageFormat();

    for (const auto &data : *messageFormat)
    {
        auto ID = data.get()->GetID();
        if (ID == messageFormatID)
            return data.get()->GetFormat();
    }

    return "";
}

bool Utilities::CheckMenuState(const vector<MenuState> &menuState, const MenuState currentState) const
{
    for (const auto &data : menuState)
    {
        if (currentState == data)
            return true;
    }
    return false;
}

bool Utilities::CheckTextClicked(const sf::Vector2f &mousePos,
                                 const sf::Vector2f &txtPos,
                                 const sf::Vector2f &txtLSize) const
{
    if (mousePos.x > txtPos.x && mousePos.x < txtPos.x + txtLSize.x && mousePos.y > txtPos.y &&
        mousePos.y < txtPos.y + txtLSize.y)
        return true;

    return false;
}

bool Utilities::CheckSpriteClicked(const sf::Vector2f &mousePos,
                                   const sf::Vector2f &btnPos,
                                   const sf::Vector2f &btnLSize,
                                   const sf::Vector2f &btnScale) const
{
    if (mousePos.x > btnPos.x && mousePos.x < btnPos.x + (btnLSize.x * btnScale.x) && mousePos.y > btnPos.y &&
        mousePos.y < btnPos.y + (btnLSize.y * btnScale.y))
        return true;

    return false;
}

bool Utilities::CheckCreatureIsNearSurfacePos(const sf::Vector2f &creaturePos,
                                              const sf::Vector2f &surfacePos,
                                              float tileSize) const
{
    if (creaturePos.x >= surfacePos.x - tileSize && creaturePos.x <= surfacePos.x + tileSize &&
        creaturePos.y >= surfacePos.y - tileSize && creaturePos.y <= surfacePos.y + tileSize)
        return true;

    return false;
}

bool Utilities::CheckCreatureIsNearItemPos(const sf::Vector2f &creaturePos,
                                           const sf::Vector2f &creatureSize,
                                           const sf::Vector2f &itemPos,
                                           const sf::Vector2f &itemSize) const
{
    if ((creaturePos.x + (creatureSize.x / 2)) >= itemPos.x && creaturePos.x <= itemPos.x + (itemSize.x / 2) &&
        creaturePos.y + (creatureSize.y / 2) >= itemPos.y && creaturePos.y <= itemPos.y + (itemSize.y / 2))
        return true;

    return false;
}

string Utilities::GetAnsiString(const uint16_t key) const
{
    auto sfmlHotkey = sf::Keyboard::delocalize(static_cast<sf::Keyboard::Key>(key));
    return (sf::Keyboard::getDescription(sfmlHotkey).toAnsiString());
}
