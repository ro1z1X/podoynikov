#pragma once
#include <string>

using namespace std;

struct Item {
private:
    string m_name;
    string m_rarity;
    string m_description;

public:
    Item(string name = "", string rarity = "", string description = "");
    string GetName() const;
    string GetRarity() const;
    string GetDescription() const;
};
