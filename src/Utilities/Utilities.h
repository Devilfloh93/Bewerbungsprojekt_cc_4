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

    void SetTitlePos(const uint16_t width, sf::Text *text);
    void SetTitlePos(const uint16_t width, sf::Text *title, sf::Text *text);
    void SetInputPos(const uint16_t width, sf::Text *input);
    void SetBtnAndTextPos(const uint16_t width, sf::Sprite *btnObj, sf::Text *title, sf::Text *btntext);
    void SetBtnAndTextPos(const uint16_t width, sf::Sprite *btnObj, sf::Sprite *btn, sf::Text *btntext);
    void SetTextBeforeIcon(sf::Sprite &icon, sf::Text &text, const sf::Vector2f &prevPos);
    void SetTextBeforeIcon(const uint16_t x, const uint16_t y, sf::Sprite &icon, sf::Text &text);
    void SetTextBeforeIcon(const uint16_t width, sf::Text *title, sf::Sprite &icon, sf::Text &text);

    void PlayAnimation(sf::Sprite *sprite, sf::Clock &clock, sf::IntRect &anim0, sf::IntRect &anim1);

    void SetSFText(sf::Text *text, const sf::Font *font, const uint8_t size, const string value);
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
};
