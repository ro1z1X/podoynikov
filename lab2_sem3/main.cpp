#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <string>
#include <locale>
#include <algorithm>
using namespace std;

const int CVET_BELIY = 15;
const int CVET_GLASNAYA = 12;
const int CVET_SOGLASNAYA = 9;
const int CVET_CIFRA = 10;
const int CVET_ZNAK = 14;

char toLowerRu(char c) {
    static string GU = "АЕЁИОУЫЭЮЯБВГДЖЗЙКЛМНПРСТФХЦЧШЩЪЬ";
    static string GL = "аеёиоуыэюябвгджзйклмнпрстфхцчшщъь";
    size_t p = GU.find(c);
    if (p != string::npos) return GL[p];
    return c;
}

bool isGlasnaya(char c) {
    static string V = "аеёиоуыэюя";
    return V.find(toLowerRu(c)) != string::npos;
}

bool isSoglasnaya(char c) {
    static string S = "бвгджзйклмнпрстфхцчшщъь";
    return S.find(toLowerRu(c)) != string::npos;
}

bool isLatin(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int kategoria(char c) {
    if (c == '@') return 3;
    if (isGlasnaya(c)) return 0;
    if (isSoglasnaya(c)) return 1;
    if (isdigit((unsigned char)c)) return 2;
    return 3;
}

class Massiv {
private:
    vector<vector<string>> stroki;

public:
    Massiv() : stroki(4) {}

    class Stroka {
    private:
        vector<string>& r;
    public:
        Stroka(vector<string>& row) : r(row) {}
        string& operator[](int j) { return r[j]; }
        size_t size() { return r.size(); }
    };

    Stroka operator[](int i) { return Stroka(stroki[i]); }

    void dobavit(int i, const string& item) {
        if (i < 0 || i >= (int)stroki.size()) return;
        for (size_t k = 0; k < stroki[i].size(); k++)
            if (stroki[i][k] == item) return;
        stroki[i].push_back(item);
    }

    bool est(int i, const string& item) {
        for (size_t k = 0; k < stroki[i].size(); k++)
            if (stroki[i][k] == item) return true;
        return false;
    }

    void delete_(int i, int j) {
        if (i < 0 || i >= (int)stroki.size()) return;
        if (j < 0 || j >= (int)stroki[i].size()) return;
        stroki[i].erase(stroki[i].begin() + j);
    }

    void delete_(const string& item) {
        for (size_t i = 0; i < stroki.size(); i++) {
            for (size_t j = 0; j < stroki[i].size(); j++) {
                if (stroki[i][j] == item) {
                    stroki[i].erase(stroki[i].begin() + j);
                    return;
                }
            }
        }
    }

    void add_endline(int k, const string& item) {
        if (k < 0 || k >= (int)stroki.size()) return;
        stroki[k].push_back(item);
    }

    Massiv operator+(Massiv& other) {
        Massiv result = *this;
        for (size_t i = 0; i < result.stroki.size(); i++)
            for (size_t j = 0; j < other.stroki[i].size(); j++)
                result.dobavit((int)i, other.stroki[i][j]);
        return result;
    }

    Massiv& operator++() {
        for (size_t i = 0; i < stroki.size(); i++)
            for (size_t j = 0; j < stroki[i].size(); j++)
                stroki[i][j] += "+1A";
        return *this;
    }

    void pokazat() {
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        int cveta[4] = { CVET_GLASNAYA, CVET_SOGLASNAYA, CVET_CIFRA, CVET_ZNAK };
        for (int i = 0; i < 4; i++) {
            SetConsoleTextAttribute(hCon, cveta[i]);
            for (size_t j = 0; j < stroki[i].size(); j++)
                cout << stroki[i][j] << " ";
            cout << endl;
        }
        SetConsoleTextAttribute(hCon, CVET_BELIY);
    }

    void sortirovat() {
        for (size_t i = 0; i < stroki.size(); i++)
            sort(stroki[i].begin(), stroki[i].end());
    }
};

string vCp1251(const string& s) {
    string t = s;

    if (t.size() >= 3 && (unsigned char)t[0] == 0xEF && (unsigned char)t[1] == 0xBB && (unsigned char)t[2] == 0xBF)
        t = t.substr(3);
    if (t.empty()) return t;

    int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, t.c_str(), -1, NULL, 0);
    if (n == 0) return t;

    wstring w(n, 0);
    MultiByteToWideChar(CP_UTF8, 0, t.c_str(), -1, &w[0], n);

    int m = WideCharToMultiByte(1251, 0, w.c_str(), -1, NULL, 0, NULL, NULL);
    if (m <= 1) return t;
    string r(m, 0);
    WideCharToMultiByte(1251, 0, w.c_str(), -1, &r[0], m, NULL, NULL);
    r.resize(m - 1);
    return r;
}

Massiv zagruzitIzFaila(const string& imyaFaila) {
    Massiv m;
    ifstream fin(imyaFaila);
    if (!fin.is_open()) {
        cout << "Не удалось открыть файл " << imyaFaila << endl;
        return m;
    }

    string str = "";
    string line;
    while (getline(fin, line))
        str += line;

    fin.close();

    str = vCp1251(str);

    if (str.length() > 50)
        cout << "Внимание: исходный текст длиннее 50 символов" << endl;

    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        if (c == '@') continue;
        if (isLatin(c)) continue;

        int k = kategoria(c);
        char cc = c;
        if (k == 0 || k == 1) cc = toLowerRu(c);

        string element(1, cc);
        m.dobavit(k, element);
    }

    return m;
}

void pokazatMenu() {
    cout << endl;
    cout << "========== МЕНЮ ==========" << endl;
    cout << "1 - Показать оба массива (A и B)" << endl;
    cout << "2 - Обращение по индексам A[i][j]" << endl;
    cout << "3 - Добавить элемент в конец строки (add_endline)" << endl;
    cout << "4 - Удалить элемент по индексам (delete(i, j))" << endl;
    cout << "5 - Удалить элемент по значению (delete(item))" << endl;
    cout << "6 - Сортировка внутри строк" << endl;
    cout << "7 - Оператор + (A + B)" << endl;
    cout << "8 - Оператор ++ (добавить \"+1A\" к каждому элементу)" << endl;
    cout << "0 - Выход" << endl;
    cout << "Выберите пункт: ";
}

int vvestiChislo() {
    int x;
    if (!(cin >> x)) {
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }
    cin.ignore(10000, '\n');
    return x;
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Massiv A = zagruzitIzFaila("input.txt");
    Massiv B = zagruzitIzFaila("input2.txt");

    while (true) {
        pokazatMenu();
        int vybor = vvestiChislo();
        cout << endl;

        if (vybor == 0) {
            break;
        }
        else if (vybor == 1) {
            cout << "Массив A:" << endl;
            A.pokazat();
            cout << endl << "Массив B:" << endl;
            B.pokazat();
        }
        else if (vybor == 2) {
            cout << "Номер строки i (0-3): ";
            int i = vvestiChislo();
            cout << "Номер элемента j: ";
            int j = vvestiChislo();

            if (i < 0 || i > 3 || j < 0 || j >= (int)A[i].size()) {
                cout << "Нет такого элемента" << endl;
            }
            else {
                cout << "A[" << i << "][" << j << "] = " << A[i][j] << endl;
                cout << "Новое значение (пустая строка - не менять): ";
                string novoe;
                getline(cin, novoe);
                if (!novoe.empty()) {
                    A[i][j] = novoe;
                    cout << "Значение изменено" << endl;
                    A.pokazat();
                }
            }
        }
        else if (vybor == 3) {
            cout << "Номер строки k (0-3): ";
            int k = vvestiChislo();
            cout << "Элемент для добавления: ";
            string item;
            getline(cin, item);

            if (k < 0 || k > 3 || item.empty()) {
                cout << "Неверный номер строки или пустой элемент" << endl;
            }
            else {
                A.add_endline(k, item);
                cout << "Массив A после add_endline:" << endl;
                A.pokazat();
            }
        }
        else if (vybor == 4) {
            cout << "Номер строки i (0-3): ";
            int i = vvestiChislo();
            cout << "Номер элемента j: ";
            int j = vvestiChislo();

            if (i < 0 || i > 3 || j < 0 || j >= (int)A[i].size()) {
                cout << "Нет такого элемента" << endl;
            }
            else {
                A.delete_(i, j);
                cout << "Массив A после delete(" << i << ", " << j << "):" << endl;
                A.pokazat();
            }
        }
        else if (vybor == 5) {
            cout << "Значение элемента для удаления: ";
            string item;
            getline(cin, item);
            A.delete_(item);
            cout << "Массив A после delete(\"" << item << "\"):" << endl;
            A.pokazat();
        }
        else if (vybor == 6) {
            A.sortirovat();
            cout << "Массив A после сортировки внутри строк:" << endl;
            A.pokazat();
        }
        else if (vybor == 7) {
            cout << "Массив A:" << endl;
            A.pokazat();
            cout << endl << "Массив B:" << endl;
            B.pokazat();

            Massiv summa = A + B;
            cout << endl << "Результат A + B (без повторов):" << endl;
            summa.pokazat();
        }
        else if (vybor == 8) {
            ++A;
            cout << "Массив A после ++A:" << endl;
            A.pokazat();
        }
        else {
            cout << "Нет такого пункта меню" << endl;
        }
    }

    return 0;
}
