#include "Player.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace std;

static void OutputInventory(const vector<string>& inv) {
    if (inv.empty()) {
        cout << "пусто";
    }
    else {
        for (size_t c = 0; c < inv.size(); c++) {
            cout << inv[c];
            if (c + 1 != inv.size()) cout << ", ";
        }
    }
    cout << endl;
}

static vector<string> CombineVectorsUnique(const vector<string>& v1, const vector<string>& v2) {
    vector<string> result;
    result.reserve(v1.size() + v2.size());

    for (const auto& item : v1) {
        if (find(result.begin(), result.end(), item) == result.end()) {
            result.push_back(item);
        }
    }
    for (const auto& item : v2) {
        if (find(result.begin(), result.end(), item) == result.end()) {
            result.push_back(item);
        }
    }
    return result;
}

Player::Player(string name, int x, int y, int hp, string status, vector<string> items)
    : m_hp(hp), m_x(x), m_y(y), m_name(name), m_status(status), m_items(items) {
    if (name.length() == 0) throw invalid_argument("Имя не может быть пустым");
    if (hp < 0) throw invalid_argument("HP должен быть положительным");
}

Player::Player(const Player& source)
    : m_hp(source.m_hp), m_x(source.m_x), m_y(source.m_y),
    m_name(source.m_name), m_status(source.m_status), m_items(source.m_items) {
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
    if (this == &rhs) return *this;
    m_hp = rhs.m_hp;
    m_x = rhs.m_x;
    m_y = rhs.m_y;
    m_name = rhs.m_name;
    m_status = rhs.m_status;
    m_items = rhs.m_items;
    cout << "Оператор =" << endl;
    return *this;
}

Player Player::operator/(const Player& rhs) const {
    Player result = *this;
    result.m_name = result.RandomName();
    result.m_x = (abs(result.m_x) + 1) / (abs(m_x) + 1);
    result.m_y = (abs(result.m_y) + 1) / (abs(m_y) + 1);

    size_t middle1 = result.m_items.size() / 2;
    size_t middle2 = rhs.m_items.size() / 2;

    vector<string> firstPart(result.m_items.begin(), result.m_items.begin() + middle1);
    vector<string> secondPart(rhs.m_items.begin() + middle2, rhs.m_items.end());

    result.m_items.clear();
    result.m_items.insert(result.m_items.end(), firstPart.begin(), firstPart.end());
    result.m_items.insert(result.m_items.end(), secondPart.begin(), secondPart.end());
    return result;
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
    if (rhs.m_items.empty()) return *this;

    string target = rhs.m_items[rand() % rhs.m_items.size()];
    auto it = find(m_items.begin(), m_items.end(), target);
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

void Player::ChangeX(int value) {
    m_x = value;
}

void Player::ChangeY(int value) {
    m_y = value;
}

void Player::AddItem(string item) {
    m_items.push_back(item);
    cout << "Добавлено: " << item << " для " << m_name << endl;
}

void Player::DeleteItem(string item) {
    auto pos = find(m_items.begin(), m_items.end(), item);
    if (pos == m_items.end()) {
        cout << "Нет предмета: " << item << " у " << m_name << endl;
        return;
    }
    m_items.erase(pos);
    cout << "Удалено: " << item << " у " << m_name << endl;
}

void Player::ChangeStatus(string newStatus) {
    m_status = newStatus;
}

void Player::IncreaseHP(int amount) {
    m_hp += amount;
    if (m_hp > 100) m_hp = 100;
}

void Player::DecreaseHP(int amount) {
    m_hp -= amount;
    if (m_hp < 0) m_hp = 0;
}

void Player::DisplayItems() const {
    cout << "Предметы " << m_name << ":" << endl;
    if (m_items.empty()) {
        cout << "пусто" << endl;
    }
    else {
        for (const auto& element : m_items) {
            cout << "- " << element << endl;
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
