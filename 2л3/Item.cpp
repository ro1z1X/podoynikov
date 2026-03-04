#include "Item.h"

Item::Item(string name, string rarity, string description)
    : m_name(name), m_rarity(rarity), m_description(description) {
}

string Item::GetName() const { return m_name; }
string Item::GetRarity() const { return m_rarity; }
string Item::GetDescription() const { return m_description; }
