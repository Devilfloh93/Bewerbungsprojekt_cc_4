#include "Gui.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

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
    auto textLSize = text.getLocalBounds().getSize();
    text.setPosition(sf::Vector2f((width / 2U) - (textLSize.x / 2U), 0.0F));
}

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Text &title, sf::Text &btntext)
{
    auto btnObjLSize = btnObj.getLocalBounds().getSize();
    auto btnObjScale = btnObj.getScale();
    auto titleLSize = title.getLocalBounds().getSize();
    auto titlePos = title.getGlobalBounds().getPosition();
    auto btnTextLSize = btntext.getLocalBounds().getSize();

    btnObj.setPosition(
        sf::Vector2f((width / 2U) - ((btnObjLSize.x * btnObjScale.x) / 2U), (titlePos.y + titleLSize.y) + 50.0F));

    auto btnObjPos = btnObj.getGlobalBounds().getPosition();
    auto btnObjSize = btnObj.getGlobalBounds().getSize();

    btntext.setPosition(sf::Vector2f(btnObjPos.x + ((btnObjSize.x / 2U) - (btnTextLSize.x / 2U)),
                                     btnObjPos.y + ((btnObjSize.y / 2U) - (btnTextLSize.y / 2U))));
}

void SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Sprite &btn, sf::Text &btntext)
{
    auto btnObjLSize = btnObj.getLocalBounds().getSize();
    auto btnObjScale = btnObj.getScale();
    auto btnLSize = btn.getLocalBounds().getSize();
    auto btnPos = btn.getGlobalBounds().getPosition();
    auto btnTextLSize = btntext.getLocalBounds().getSize();

    btnObj.setPosition(
        sf::Vector2f((width / 2U) - ((btnObjLSize.x * btnObjScale.x) / 2U), (btnPos.y + btnLSize.y) + 50.0F));

    auto btnObjPos = btnObj.getGlobalBounds().getPosition();
    auto btnObjSize = btnObj.getGlobalBounds().getSize();

    btntext.setPosition(sf::Vector2f(btnObjPos.x + ((btnObjSize.x / 2U) - (btnTextLSize.x / 2U)),
                                     btnObjPos.y + ((btnObjSize.y / 2U) - (btnTextLSize.y / 2U))));
}

void ProcessJSON(const json &j, std::vector<nlohmann::json_abi_v3_11_2::ordered_json> &vec)
{
    for (auto it = j.begin(); it != j.end(); ++it)
    {
        if (it->is_object())
        {
            vec.push_back(*it);
        }
    }
}

void InitMenus(const Game &game,
               const sf::Font &font,
               const sf::Texture &texture,
               std::vector<std::unique_ptr<Text>> &menus)
{
    sf::Sprite prevBtn;
    auto width = game.GetWindowWidth();

    std::ifstream file("./data/menus.json");

    std::vector<nlohmann::json_abi_v3_11_2::ordered_json> menuTitles;
    std::vector<nlohmann::json_abi_v3_11_2::ordered_json> menuButtons;

    if (file.is_open())
    {
        auto jsonData = nlohmann::ordered_json::parse(file);

        for (auto it = jsonData.begin(); it != jsonData.end(); ++it)
        {
            if (it.key() == "Titles")
            {
                ProcessJSON(*it, menuTitles);
            }

            if (it.key() == "Buttons")
            {
                ProcessJSON(*it, menuButtons);
            }
        }

        for (const auto &data : menuTitles)
        {
            sf::Text titleText;
            sf::Text btnText;
            sf::Sprite btn;

            bool firstButton = true;
            auto gameState = data["state"];
            titleText.setFont(font);
            titleText.setCharacterSize(data["fontSize"]);
            titleText.setString(static_cast<std::string>(data["name"]));

            SetTitlePos(width, titleText);
            menus.push_back(std::make_unique<Text>(data["state"], BtnFunc::Nothing, titleText, btn));

            for (const auto &data1 : menuButtons)
            {
                if (gameState == data1["state"])
                {
                    btn.setTexture(texture);
                    btn.setTextureRect({data1["textureCoords"][0],
                                        data1["textureCoords"][1],
                                        data1["textureCoords"][2],
                                        data1["textureCoords"][3]});
                    btn.setScale({data1["scale"][0], data1["scale"][1]});

                    btnText.setFont(font);
                    btnText.setCharacterSize(data1["fontSize"]);
                    btnText.setString(static_cast<std::string>(data1["name"]));

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

                    menus.push_back(std::make_unique<Button>(data1["state"], data1["fnc"], btnText, btn));
                }
            }
        }

        file.close();
    }
    else
    {
    }
}

void DrawMenu(sf::RenderWindow &window, sf::View &view, std::vector<std::unique_ptr<Text>> &menus, GameState state)
{
    window.setView(view);

    for (const auto &data : menus)
    {
        auto gameState = data->GetGameState();
        if (gameState == state)
        {
            window.draw(data->GetSprite());
            window.draw(data->GetText());
        }
    }
}
