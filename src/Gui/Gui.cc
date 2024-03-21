#include "Gui.h"


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
    return m_text;
}

MenuState Title::GetMenuState() const
{
    return m_menuState;
}

sf::Text Button::GetText() const
{
    return m_text;
}

std::vector<MenuState> Button::GetMenuState() const
{
    return m_menuState;
}

sf::Sprite Button::GetSprite() const
{
    return m_sprite;
}

BtnFunc Button::GetBtnFnc() const
{
    return m_btnfnc;
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
