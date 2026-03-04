#pragma once
#include <string>
#include <vector>
#include "Item.h"
#include "LootBox.h"

using namespace std;

class Player {
private:
    int m_hp;
    int m_x;
    int m_y;
    string m_name;
    string m_status;
    vector<Item> m_items;

public:
    Player(string name, int x, int y, int hp, string status, vector<Item> items = {});
    Player(const Player& other);
    ~Player();

    string RandomName();

    Player& operator=(const Player& other);
    Player operator/(const Player& other) const;
    Player operator+(const Player& other) const;
    Player operator-(const Player& other) const;
    Player& operator-=(const Player& other);

    void ChangeX(int value);
    void ChangeY(int value);
    void AddItem(const Item& item);
    void DeleteItem(string itemName);
    void ChangeStatus(string newStatus);
    void IncreaseHP(int amount);
    void DecreaseHP(int amount);
    void OpenLootBox(const LootBox& box);
    void DisplayItems() const;
    void DisplayAll() const;

    string GetName() const;
    int GetX() const;
    int GetY() const;
    int GetHP() const;
};
