#include <iostream>
#include <ctime>
#include "Player.h"

using namespace std;

int main() {
    srand((unsigned)time(0));
    setlocale(LC_ALL, "ru_RU.UTF-8");

    try {
        vector<string> items1 = { "knife", "bandage", "vodka", "flashlight", "knife" };
        vector<string> items2 = { "shotgun", "bullets", "medkit", "grenade", "compass", "bullets" };

        Player first("Дмитрий", 5, 12, 85, "Осматривает руины", items1);
        Player second("Алексей", 23, 44, 100, "Чинит генератор", items2);

        Player third = first + second;
        first -= second;

        first.DisplayAll();
        second.DisplayAll();
        third.DisplayAll();
    }
    catch (const invalid_argument& err) {
        cout << "Ошибка: " << err.what() << endl;
    }
    catch (...) {
        cout << "Неизвестная ошибка" << endl;
    }

    return 0;
}
