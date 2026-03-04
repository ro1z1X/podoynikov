#include "Player.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace std;


static void OutputInventory(const vector<Item>& inv) {
    if (inv.empty()) {
        cout << "пусто";
    }
    else {
        for (size_t i = 0; i < inv.size(); i++) {
            cout << inv[i].GetName();
            if (i + 1 != inv.size()) cout << ", ";
        }
    }
    cout << endl;
}

static vector<Item> CombineVectorsUnique(const vector<Item>& v1, const vector<Item>& v2) {
    vector<Item> result;

    for (const auto& item : v1) {
        auto it = find_if(result.begin(), result.end(),
            [&](const Item& i) { return i.GetName() == item.GetName(); });

        if (it == result.end())
            result.push_back(item);
    }

    for (const auto& item : v2) {
        auto it = find_if(result.begin(), result.end(),
            [&](const Item& i) { return i.GetName() == item.GetName(); });

        if (it == result.end())
            result.push_back(item);
    }

    return result;
}

Player::Player(string name, int x, int y, int hp, string status, vector<Item> items)
    : m_hp(hp), m_x(x), m_y(y), m_name(name), m_status(status), m_items(items)
{
    if (name.empty())
        throw invalid_argument("Имя не может быть пустым");

    if (hp < 0)
        throw invalid_argument("HP должен быть положительным");
}

Player::Player(const Player& source)
    : m_hp(source.m_hp),
    m_x(source.m_x),
    m_y(source.m_y),
    m_name(source.m_name),
    m_status(source.m_status),
    m_items(source.m_items)
{
    cout << "Копия создана" << endl;
}

Player::~Player() {
    if (!m_items.empty()) {
        m_items.clear();
        cout << "Инвентарь очищен" << endl;
    }
    cout << "Деструктор" << endl;
}

string Player::RandomName() {
    return "Player_" + to_string(rand() % 100000);
}

Player& Player::operator=(const Player& rhs) {
    if (this == &rhs)
        return *this;

    m_hp = rhs.m_hp;
    m_x = rhs.m_x;
    m_y = rhs.m_y;
    m_name = rhs.m_name;
    m_status = rhs.m_status;
    m_items = rhs.m_items;

    cout << "Оператор =" << endl;
    return *this;
}

Player Player::operator+(const Player& rhs) const {
    Player result = *this;

    result.m_name = result.RandomName();
    result.m_x = (m_x + rhs.m_x) / 2;
    result.m_y = (m_y + rhs.m_y) / 2;

    result.m_items = CombineVectorsUnique(m_items, rhs.m_items);

    return result;
}

Player& Player::operator-=(const Player& rhs) {
    if (rhs.m_items.empty() || m_items.empty())
        return *this;

    const Item& target = rhs.m_items[rand() % rhs.m_items.size()];

    auto it = find_if(m_items.begin(), m_items.end(),
        [&](const Item& i) { return i.GetName() == target.GetName(); });

    if (it != m_items.end()) {
        m_items.erase(it);
    }

    return *this;
}

Player Player::operator-(const Player& rhs) const {
    Player result = *this;
    result -= rhs;
    result.m_name = result.RandomName();
    return result;
}

Player Player::operator/(const Player& rhs) const {
    Player result = *this;

    result.m_name = result.RandomName();
    result.m_x = (abs(m_x) + 1) / (abs(rhs.m_x) + 1);
    result.m_y = (abs(m_y) + 1) / (abs(rhs.m_y) + 1);

    size_t middle1 = m_items.size() / 2;
    size_t middle2 = rhs.m_items.size() / 2;

    vector<Item> firstPart(m_items.begin(), m_items.begin() + middle1);
    vector<Item> secondPart(rhs.m_items.begin() + middle2, rhs.m_items.end());

    result.m_items.clear();
    result.m_items.insert(result.m_items.end(), firstPart.begin(), firstPart.end());
    result.m_items.insert(result.m_items.end(), secondPart.begin(), secondPart.end());

    return result;
}

void Player::ChangeX(int value) {
    m_x = value;
}

void Player::ChangeY(int value) {
    m_y = value;
}

void Player::AddItem(const Item& item) {
    m_items.push_back(item);
    cout << "Добавлено: " << item.GetName() << " для " << m_name << endl;
}

void Player::DeleteItem(string itemName) {
    auto it = find_if(m_items.begin(), m_items.end(),
        [&](const Item& i) { return i.GetName() == itemName; });

    if (it == m_items.end()) {
        cout << "Нет предмета: " << itemName << " у " << m_name << endl;
        return;
    }

    cout << "Удалено: " << it->GetName() << " у " << m_name << endl;
    m_items.erase(it);
}

void Player::ChangeStatus(string newStatus) {
    m_status = newStatus;
}

void Player::IncreaseHP(int amount) {
    m_hp += amount;
    if (m_hp > 100)
        m_hp = 100;
}

void Player::DecreaseHP(int amount) {
    m_hp -= amount;
    if (m_hp < 0)
        m_hp = 0;
}

void Player::OpenLootBox(const LootBox& box) {
    Item newItem = box.Open();

    cout << m_name << " открыл лутбокс и получил: "
        << newItem.GetName()
        << " [" << newItem.GetRarity() << "]" << endl;

    m_items.push_back(newItem);
}

void Player::DisplayItems() const {
    cout << "Предметы " << m_name << ":" << endl;

    if (m_items.empty()) {
        cout << "пусто" << endl;
    }
    else {
        for (const auto& item : m_items) {
            cout << "- "
                << item.GetName()
                << " (" << item.GetRarity() << ")"
                << endl;
        }
    }
}

void Player::DisplayAll() const {
    cout << "Имя: " << m_name
        << " | Здоровье: " << m_hp
        << " | Позиция: (" << m_x << ", " << m_y << ")"
        << " | Статус: " << m_status
        << " | Инвентарь: ";

    OutputInventory(m_items);
}

string Player::GetName() const { return m_name; }
int Player::GetX() const { return m_x; }
int Player::GetY() const { return m_y; }
int Player::GetHP() const { return m_hp; }
