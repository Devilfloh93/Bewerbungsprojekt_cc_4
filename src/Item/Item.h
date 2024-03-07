#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class Item
{

public:
    Item(const std::uint16_t ID, const std::string name);
    ~Item() = default;

    std::uint16_t GetID() const;
    std::string GetName() const;


private:
    std::uint16_t m_ID;
    std::string m_name;
};

void InitItems(std::vector<std::unique_ptr<Item>> &items);
