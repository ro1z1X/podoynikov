#pragma once
#include <vector>
#include <string>
#include "Item.h"

using namespace std;

class LootBox {
private:
    string m_name;
    vector<Item> m_possibleItems;

public:
    LootBox(string name = "");
    LootBox(string name, const vector<Item>& items);
    LootBox(const LootBox& other);

    Item Open() const;
};
