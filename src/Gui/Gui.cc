#include "Gui.h"

#include <iostream>


Text::Text(const GameState gameState, const BtnFunc btnfnc, const sf::Text text, const sf::Sprite sprite)
    : m_gameState(gameState), m_text(text), m_sprite(sprite), m_btnfnc(btnfnc)
{
}

Button::Button(const GameState gameState, const BtnFunc btnfnc, const sf::Text text, const sf::Sprite sprite)
    : Text(gameState, btnfnc, text, sprite)
{
}

sf::Text Text::GetText() const
{
    return this->m_text;
}

GameState Text::GetGameState() const
{
    return this->m_gameState;
}

sf::Sprite Text::GetSprite() const
{
    return this->m_sprite;
}

BtnFunc Text::GetBtnFnc() const
{
    return this->m_btnfnc;
}


void SetTitlePos(const std::uint32_t width, sf::Text &text)
{
    text.setPosition(sf::Vector2f((width / 2U) - (text.getLocalBounds().getSize().x / 2U), 0.0F));
}

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Text &title, sf::Text &btntext)
{
    btnObj.setPosition(
        sf::Vector2f((width / 2U) - ((btnObj.getLocalBounds().getSize().x * btnObj.getScale().x) / 2U),
                     (title.getGlobalBounds().getPosition().y + title.getLocalBounds().getSize().y) + 50.0F));

    btntext.setPosition(
        sf::Vector2f(btnObj.getGlobalBounds().getPosition().x +
                         ((btnObj.getGlobalBounds().getSize().x / 2U) - (btntext.getLocalBounds().getSize().x / 2U)),
                     btnObj.getGlobalBounds().getPosition().y +
                         ((btnObj.getGlobalBounds().getSize().y / 2U) - (btntext.getLocalBounds().getSize().y / 2U))));
}

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Sprite &button, sf::Text &btntext)
{
    btnObj.setPosition(
        sf::Vector2f((width / 2U) - ((btnObj.getLocalBounds().getSize().x * btnObj.getScale().x) / 2U),
                     (button.getGlobalBounds().getPosition().y + button.getLocalBounds().getSize().y) + 50.0F));

    btntext.setPosition(
        sf::Vector2f(btnObj.getGlobalBounds().getPosition().x +
                         ((btnObj.getGlobalBounds().getSize().x / 2U) - (btntext.getLocalBounds().getSize().x / 2U)),
                     btnObj.getGlobalBounds().getPosition().y +
                         ((btnObj.getGlobalBounds().getSize().y / 2U) - (btntext.getLocalBounds().getSize().y / 2U))));
}


void InitMenus(const std::uint32_t width,
               const sf::Font &font,
               const sf::Texture &texture,
               std::vector<std::unique_ptr<Text>> &menus,
               const std::vector<TextCfg> &menuTitles,
               const std::vector<ButtonCfg> &menuButtons)
{


    sf::Sprite prevBtn;
    for (const auto &data : menuTitles)
    {
        sf::Text titleText;
        sf::Text btnText;
        sf::Sprite btn;

        bool firstButton = true;
        auto gameState = data.guiCfg.gameState;
        titleText.setFont(font);
        titleText.setCharacterSize(data.fontSize);
        titleText.setString(data.text);

        SetTitlePos(width, titleText);
        menus.push_back(std::make_unique<Text>(data.guiCfg.gameState, BtnFunc::Nothing, titleText, btn));

        for (const auto &data1 : menuButtons)
        {
            if (gameState == data1.textCfg.guiCfg.gameState)
            {
                btn.setTexture(texture);
                btn.setTextureRect(data1.textureRect);
                btn.setScale(data1.scale);

                btnText.setFont(font);
                btnText.setCharacterSize(data1.textCfg.fontSize);
                btnText.setString(data1.textCfg.text);

                if (firstButton)
                {
                    SetBtnAndTextPos(width, btn, titleText, btnText);
                }
                else
                {
                    SetBtnAndTextPos(width, btn, prevBtn, btnText);
                }

                prevBtn = btn;
                firstButton = false;

                menus.push_back(std::make_unique<Button>(data1.textCfg.guiCfg.gameState, data1.btnFnc, btnText, btn));
            }
        }
    }
}
