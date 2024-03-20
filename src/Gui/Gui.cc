#include "Gui.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Title::Title(const MenuState menuState, const sf::Text &text) : m_menuState(menuState), m_text(text)
{
}

Button::Button(const std::vector<MenuState> menuState,
               const BtnFunc btnfnc,
               const sf::Text text,
               const sf::Sprite sprite)
    : m_menuState(menuState), m_text(text), m_sprite(sprite), m_btnfnc(btnfnc)
{
}

sf::Text Title::GetText() const
{
    return this->m_text;
}

MenuState Title::GetMenuState() const
{
    return this->m_menuState;
}

sf::Text Button::GetText() const
{
    return this->m_text;
}

std::vector<MenuState> Button::GetMenuState() const
{
    return this->m_menuState;
}

sf::Sprite Button::GetSprite() const
{
    return this->m_sprite;
}

BtnFunc Button::GetBtnFnc() const
{
    return this->m_btnfnc;
}

void ProcessJSON(const json &j, std::vector<nlohmann::json_abi_v3_11_2::ordered_json> &vec)
{
    for (auto it = j.begin(); it != j.end(); ++it)
    {
        if (it->is_object())
            vec.push_back(*it);
    }
}

void Menu::SetTitlePos(const std::uint32_t width, sf::Text &text)
{
    auto textLSize = text.getLocalBounds().getSize();
    text.setPosition(sf::Vector2f((width / 2U) - (textLSize.x / 2U), 0.0F));
}

void Menu::SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Text &title, sf::Text &btntext)
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

void Menu::SetBtnAndTextPos(const std::uint32_t width, sf::Sprite &btnObj, sf::Sprite &btn, sf::Text &btntext)
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

void Menu::SetTextBeforeIcon(sf::Sprite &icon, sf::Text &text, sf::Sprite &prevIcon)
{
    auto prevIconPos = prevIcon.getGlobalBounds().getPosition();
    auto iconLSize = icon.getLocalBounds().getSize();
    icon.setPosition(prevIconPos.x, prevIconPos.y + iconLSize.y);

    iconLSize = icon.getLocalBounds().getSize();
    auto textLSize = text.getLocalBounds().getSize();
    auto iconPos = icon.getPosition();

    text.setPosition(sf::Vector2f(iconPos.x + (iconLSize.x + (iconLSize.x / 2)), iconPos.y));
}

void Menu::SetTextBeforeIcon(const std::uint16_t x, const std::uint16_t y, sf::Sprite &icon, sf::Text &text)
{
    icon.setPosition(x, y);

    auto iconLSize = icon.getLocalBounds().getSize();
    auto textLSize = text.getLocalBounds().getSize();
    auto iconPos = icon.getPosition();

    text.setPosition(sf::Vector2f(iconPos.x + (iconLSize.x + (iconLSize.x / 2)), iconPos.y));
}

void Menu::Init(const Game &game,
                std::vector<std::unique_ptr<Title>> &titles,
                std::vector<std::unique_ptr<Button>> &buttons,
                const std::vector<std::unique_ptr<Texture>> &textures,
                const std::vector<std::unique_ptr<Font>> &fonts)
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
                ProcessJSON(*it, menuTitles);

            if (it.key() == "Buttons")
                ProcessJSON(*it, menuButtons);
        }

        for (const auto &data : menuTitles)
        {
            sf::Text titleText;
            sf::Text btnText;
            sf::Sprite btn;

            bool firstButton = true;
            MenuState state = data["state"];
            std::uint8_t fontSize = data["fontSize"];
            std::string text = data["name"];
            std::uint8_t fontID = data["fontID"];
            sf::Font *font;

            for (const auto &data5 : fonts)
            {
                auto fonID = data5->GetID();
                if (fonID == fontID)
                {
                    font = data5->GetFont();
                }
            }

            titleText.setFont(*font);
            titleText.setCharacterSize(fontSize);
            titleText.setString(text);

            SetTitlePos(width, titleText);
            titles.push_back(std::make_unique<Title>(state, titleText));

            for (const auto &data1 : menuButtons)
            {
                bool addBtn = false;
                for (const auto &data2 : data1["state"])
                {
                    if (state == data2)
                    {
                        addBtn = true;
                        break;
                    }
                }

                if (addBtn)
                {
                    auto textureRect = sf::IntRect{data1["textureCoords"][0],
                                                   data1["textureCoords"][1],
                                                   data1["textureCoords"][2],
                                                   data1["textureCoords"][3]};
                    auto scale = sf::Vector2f{data1["scale"][0], data1["scale"][1]};
                    text = data1["name"];
                    fontSize = data1["fontSize"];
                    std::vector<MenuState> state = data1["state"];
                    BtnFunc btnFnc = data1["fnc"];
                    std::uint8_t textureID = data1["textureID"];
                    fontID = data1["fontID"];
                    sf::Texture *texture;

                    for (const auto &data3 : textures)
                    {
                        auto texID = data3->GetID();
                        if (texID == textureID)
                        {
                            texture = data3->GetTexture();
                        }
                    }

                    for (const auto &data4 : fonts)
                    {
                        auto fonID = data4->GetID();
                        if (fonID == fontID)
                        {
                            font = data4->GetFont();
                        }
                    }

                    btn.setTexture(*texture);
                    btn.setTextureRect(textureRect);
                    btn.setScale(scale);

                    btnText.setFont(*font);
                    btnText.setCharacterSize(fontSize);
                    btnText.setString(text);

                    if (firstButton)
                        SetBtnAndTextPos(width, btn, titleText, btnText);
                    else
                        SetBtnAndTextPos(width, btn, prevBtn, btnText);

                    prevBtn = btn;
                    firstButton = false;

                    buttons.push_back(std::make_unique<Button>(state, btnFnc, btnText, btn));
                }
            }
        }

        file.close();
    }
    else
    {
    }
}


void Menu::Draw(sf::RenderWindow &window,
                sf::View &view,
                const std::vector<std::unique_ptr<Title>> &titles,
                const std::vector<std::unique_ptr<Button>> &buttons,
                const MenuState state)
{
    window.setView(view);

    for (const auto &data : titles)
    {
        if (state == data->GetMenuState())
            window.draw(data->GetText());

        for (const auto &data1 : buttons)
        {
            bool showBtn = false;
            auto menuStates = data1->GetMenuState();
            for (const auto &data3 : menuStates)
            {
                if (state == data3)
                {
                    showBtn = true;
                    break;
                }
            }

            if (showBtn)
            {
                window.draw(data1->GetSprite());
                window.draw(data1->GetText());
            }
        }
    }
}

void Menu::Draw(sf::RenderWindow &window,
                sf::View &view,
                const std::vector<std::unique_ptr<Title>> &titles,
                const std::vector<std::unique_ptr<Button>> &buttons,
                const MenuState state,
                Player &player,
                const std::vector<std::unique_ptr<ItemCfg>> &itemCfg)
{
    window.setView(view);

    for (const auto &data : titles)
    {
        if (state == data->GetMenuState())
            window.draw(data->GetText());

        if (state == MenuState::Inventory)
            player.DrawInventoryItems(window, itemCfg);

        for (const auto &data1 : buttons)
        {
            bool showBtn = false;
            auto menuStates = data1->GetMenuState();
            for (const auto &data3 : menuStates)
            {
                if (state == data3)
                {
                    showBtn = true;
                    break;
                }
            }

            if (showBtn)
            {
                window.draw(data1->GetSprite());
                window.draw(data1->GetText());
            }
        }
    }
}

bool Menu::HandleBtnClicked(sf::RenderWindow &window,
                            const std::vector<std::unique_ptr<Button>> &buttons,
                            const MenuState state,
                            Game &game)
{
    // get the current mouse position in the window
    auto pixelPos = sf::Mouse::getPosition(window);
    // convert it to world coordinates
    auto worldPos = window.mapPixelToCoords(pixelPos);
    bool breakLoop = false;

    for (const auto &data : buttons)
    {
        auto btnPos = data->GetSprite().getPosition();
        auto btnLSize = data->GetSprite().getLocalBounds().getSize();
        auto btnScale = data->GetSprite().getScale();
        auto data1 = data->GetMenuState();

        bool clickBtn = false;
        for (const auto &data2 : data1)
        {
            if (state == data2)
            {
                clickBtn = true;
                break;
            }
        }

        if (worldPos.x > btnPos.x && worldPos.x < btnPos.x + (btnLSize.x * btnScale.x) && worldPos.y > btnPos.y &&
            worldPos.y < btnPos.y + (btnLSize.y * btnScale.y) && clickBtn)
        {
            switch (data->GetBtnFnc())
            {
            case BtnFunc::Play:
                game.SetPlaying(true);
                game.SetMenuState(MenuState::Playing);
                breakLoop = true;
                break;
            case BtnFunc::Resume:
                game.SetMenuState(MenuState::Playing);
                breakLoop = true;
                break;
            case BtnFunc::Quit:
                window.close();
                breakLoop = true;
                break;
            case BtnFunc::Options:
                game.SetMenuState(MenuState::Options);
                /*
                    TODO: ADD OPTIONS TO OPTION MENU
                */
                // game.SetWindowWidth(1920U);
                // game.SetWindowHeight(1080U);
                // windowWidth = game.GetWindowWidth();
                // windowHeight = game.GetWindowHeight();
                // window.setSize(sf::Vector2u(windowWidth, windowHeight));
                break;
            case BtnFunc::Back:
                if (game.GetPlaying())
                {
                    if (game.GetMenuState() == MenuState::Inventory)
                        game.SetMenuState(MenuState::Playing);
                    else
                        game.SetMenuState(MenuState::Pause);
                }
                else
                {
                    game.SetMenuState(MenuState::Main);
                }
                breakLoop = true;
                break;
            default:
                break;
            }
            break;
        }
    }
    return breakLoop;
}
