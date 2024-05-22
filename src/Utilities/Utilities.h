#pragma once
#include "Anim.h"
#include "Font.h"
#include "Game.h"
#include "Gui.h"
#include "Texture.h"
#include "Utilities.h"
#include "nlohmann/json.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace std;

class Utilities final
{

public:
    Utilities();
    ~Utilities() = default;

    bool isAlpha(const sf::Uint32 character) const;
    bool isNum(const sf::Uint32 character) const;

    void SetTitlePos(const uint16_t width, sf::Text *text);
    void SetTitlePos(const uint16_t width, sf::Text *title, sf::Text *text, float spaceBetweenInputs);

    void SetInputPos(const uint16_t width, sf::Text *input);

    void SetSpriteAndTextPos(const uint16_t width,
                             sf::Sprite *btnObj,
                             sf::Text *title,
                             sf::Text *btntext,
                             float spaceBetweenBtn);
    void SetSpriteAndTextPos(const uint16_t width,
                             sf::Sprite *btnObj,
                             sf::Sprite *btn,
                             sf::Text *btntext,
                             float spaceBetweenBtn);

    bool UpdateSpriteAndText(const bool firstMenuEntry,
                             const Alignment alignment,
                             const uint16_t width,
                             sf::Sprite *sprite,
                             sf::Text *prevText,
                             sf::Sprite *prevSprite,
                             sf::Text *text);

    void SetTextBeforeIcon(sf::Sprite &icon, sf::Text &text, const sf::Vector2f &prevPos);
    void SetTextBeforeIcon(const uint16_t x, const uint16_t y, sf::Sprite &icon, sf::Text &text);
    void SetTextBeforeIcon(const uint16_t width, sf::Text *title, sf::Sprite &icon, sf::Text &text);

    uint16_t CalculateAlignmentWindowWidth(const uint16_t width, const Alignment alignment) const;
    float CalculateSpaceBetweenMenu(const Alignment alignment) const;

    void PlayAnimation(sf::Sprite *sprite, sf::Clock &clock, sf::IntRect &anim0, sf::IntRect &anim1);

    void SetSFText(sf::Text *text,
                   const uint8_t size,
                   const vector<Font *> &fonts,
                   const uint8_t fontID,
                   const nlohmann::json_abi_v3_11_2::json &jsonDataLanguage,
                   const uint8_t languageID,
                   string_view language);

    void SetSFText(sf::Text *text, const uint8_t size, const vector<Font *> &fonts, const uint8_t fontID);

    template <typename T>
    void SetSFText(sf::Text *text, const uint8_t size, const vector<Font *> &fonts, const uint8_t fontID, const T value)
    {
        auto font = GetFont(fonts, fontID);

        text->setFont(*font);
        text->setCharacterSize(size);
        text->setString(format("{}", value));
    }

    void SetSFSprite(sf::Sprite *sprite, const sf::Texture *texture, const sf::IntRect &rectangle);
    void SetSFSprite(sf::Sprite *sprite,
                     const vector<Texture *> &textures,
                     const uint8_t textureID,
                     const sf::IntRect &rectangle,
                     float x,
                     float y);
    void SetSFSprite(sf::Sprite *sprite, const sf::Texture *texture, const sf::IntRect &rectangle, float x, float y);

    void SetSFSprite(sf::Sprite *sprite,
                     const vector<Texture *> &textures,
                     const uint8_t textureID,
                     const sf::IntRect &rectangle,
                     const sf::Vector2f &factors);

    const sf::Font *GetFont(const vector<Font *> &fonts, const uint8_t fontID) const;
    const sf::Texture *GetTexture(const vector<Texture *> &textures, const uint8_t textureID) const;

    AnimTextureCombined GetAnim(const vector<Anim *> &anim, const uint8_t animID) const;
    const sf::Texture *GetAnimTexture(const vector<Anim *> &anim,
                                      const vector<Texture *> &textures,
                                      const uint8_t animID) const;

    string GetLanguageText(const json &jsonData, const uint8_t languageID, string_view language) const;
    string_view GetItemName(const Game &game, const uint8_t itemID) const;
    string GetMessageFormat(const Game &game, const uint16_t messageFormatID);

    bool CheckMenuState(const vector<MenuState> &menuState, const MenuState currentState) const;

    bool CheckTextClicked(const sf::Vector2f &mousePos, const sf::Vector2f &txtPos, const sf::Vector2f &txtLSize) const;
    bool CheckSpriteClicked(const sf::Vector2f &mousePos,
                            const sf::Vector2f &btnPos,
                            const sf::Vector2f &btnLSize,
                            const sf::Vector2f &btnScale) const;
    bool CheckCreatureIsNearSurfacePos(const sf::Vector2f &creaturePos,
                                       const sf::Vector2f &surfacePos,
                                       float tileSize) const;
    bool CheckCreatureIsNearItemPos(const sf::Vector2f &creaturePos,
                                    const sf::Vector2f &creatureSize,
                                    const sf::Vector2f &itemPos,
                                    const sf::Vector2f &itemSize) const;

    string GetAnsiString(const uint16_t key) const;
};
