#pragma once
#include "nlohmann/json.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;

class Utilities
{

public:
    Utilities();
    ~Utilities() = default;

    void SetTitlePos(const uint16_t width, sf::Text *text);
    void SetTitlePos(const uint16_t width, sf::Text *title, sf::Text *text);
    void SetInputPos(const uint16_t width, sf::Text *input);
    void SetBtnAndTextPos(const uint16_t width, sf::Sprite *btnObj, sf::Text *title, sf::Text *btntext);
    void SetBtnAndTextPos(const uint16_t width, sf::Sprite *btnObj, sf::Sprite *btn, sf::Text *btntext);
    void SetTextBeforeIcon(sf::Sprite &icon, sf::Text &text, sf::Sprite &prevIcon);
    void SetTextBeforeIcon(const uint16_t x, const uint16_t y, sf::Sprite &icon, sf::Text &text);

private:
};
