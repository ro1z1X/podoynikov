#include "LootBox.h"
#include <cstdlib>

LootBox::LootBox(string name)
    : m_name(name) {
}

LootBox::LootBox(string name, const vector<Item>& items)
    : m_name(name), m_possibleItems(items) {
}

LootBox::LootBox(const LootBox& other)
    : m_name(other.m_name), m_possibleItems(other.m_possibleItems) {
}

Item LootBox::Open() const {
    if (m_possibleItems.empty()) {
        return Item("Пусто", "None", "Лутбокс пуст");
    }

    int index = rand() % m_possibleItems.size();
    return m_possibleItems[index];
}
