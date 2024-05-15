#pragma once
#include "Anim.h"
#include "Font.h"
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

class Utilities
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

    void SetTextBeforeIcon(sf::Sprite &icon, sf::Text &text, const sf::Vector2f &prevPos);
    void SetTextBeforeIcon(const uint16_t x, const uint16_t y, sf::Sprite &icon, sf::Text &text);
    void SetTextBeforeIcon(const uint16_t width, sf::Text *title, sf::Sprite &icon, sf::Text &text);

    uint16_t CalculateAlignmentWindowWidth(const uint16_t width, const Alignment alignment);
    float CalculateSpaceBetweenMenu(const Alignment alignment);

    void PlayAnimation(sf::Sprite *sprite, sf::Clock &clock, sf::IntRect &anim0, sf::IntRect &anim1);

    template <typename T>
    void SetSFText(sf::Text *text, const sf::Font *font, const uint8_t size, const T value)
    {
        text->setFont(*font);
        text->setCharacterSize(size);
        text->setString(format("{}", value));
    }
    void SetSFText(sf::Text *text, const sf::Font *font, const uint8_t size);

    void SetSFSprite(sf::Sprite *sprite, const sf::Texture *texture, const sf::IntRect &rectangle);
    void SetSFSprite(sf::Sprite *sprite, const sf::Texture *texture, const sf::IntRect &rectangle, float x, float y);
    void SetSFSprite(sf::Sprite *sprite,
                     const sf::Texture *texture,
                     const sf::IntRect &rectangle,
                     const sf::Vector2f &factors);

    const sf::Font *GetFont(const vector<Font *> &fonts, const uint8_t fontID) const;
    const sf::Texture *GetTexture(const vector<Texture *> &textures, const uint8_t textureID) const;

    AnimTextureCombined GetAnim(const vector<Anim *> &anim, const uint8_t animID);
    const sf::Texture *GetAnimTexture(const vector<Anim *> &anim,
                                      const vector<Texture *> &textures,
                                      const uint8_t animID) const;

    string GetLanguageText(const json &jsonData, const uint8_t languageID, string_view language);

    bool CheckMenuState(const vector<MenuState> &menuState, const MenuState currentState);

    bool CheckTextClicked(const sf::Vector2f &mousePos, const sf::Vector2f &txtPos, const sf::Vector2f &txtLSize);
    bool CheckSpriteClicked(const sf::Vector2f &mousePos,
                            const sf::Vector2f &btnPos,
                            const sf::Vector2f &btnLSize,
                            const sf::Vector2f &btnScale);
};
