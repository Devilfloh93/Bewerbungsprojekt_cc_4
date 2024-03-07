#include "Item.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Item::Item(const std::uint16_t ID, const std::string name) : m_ID(ID), m_name(name)
{
}

std::uint16_t Item::GetID() const
{
    return this->m_ID;
}

std::string Item::GetName() const
{
    return this->m_name;
}

void InitItems(std::vector<std::unique_ptr<Item>> &items)
{
    std::ifstream file("./data/items.json");

    std::vector<nlohmann::json_abi_v3_11_2::ordered_json> jsonItems;

    if (file.is_open())
    {
        auto jsonData = nlohmann::ordered_json::parse(file);

        for (const auto &data : jsonData)
        {
            items.push_back(std::make_unique<Item>(data["id"], data["name"]));
        }
    }
}
