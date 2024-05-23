#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>

using namespace std;

enum class ItemFnc
{
    Nothing = 0,
    Food,
    Water,
    Health
};

class ItemCfg final
{
public:
    ItemCfg(const uint8_t textureID,
            const sf::IntRect textureData,
            const uint8_t ID,
            const string_view name,
            const uint8_t maxDrop,
            const uint8_t fontID,
            const ItemFnc usableFnc,
            const float usableValue);
    ~ItemCfg() = default;

    sf::IntRect GetTextureData() const;

    uint8_t GetID() const;
    uint8_t GetTextureID() const;
    uint8_t GetMaxDrop() const;
    uint8_t GetFontID() const;
    string_view GetName() const;
    ItemFnc GetUsableFnc() const;
    float GetUsableValue() const;

private:
    sf::IntRect m_textureData;
    uint8_t m_ID;
    uint8_t m_textureID;
    string m_name;
    uint8_t m_maxDrop;
    uint8_t m_fontID;
    ItemFnc m_usableFnc;
    float m_usableValue;
};
