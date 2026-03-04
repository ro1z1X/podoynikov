#include <iostream>
#include <ctime>
#include "Player.h"
#include "Item.h"

using namespace std;

int main()
{
    srand((unsigned)time(0));
    setlocale(LC_ALL, "ru_RU.UTF-8");

    try
    {
        vector<Item> items1 = {
            Item("knife", "common", "Обычный нож"),
            Item("bandage", "common", "Бинт"),
            Item("pistol", "rare", "Пистолет"),
            Item("flashlight", "common", "Фонарик"),
        };

        vector<Item> items2 = {
            Item("shotgun", "rare", "Дробовик"),
            Item("bullets", "common", "Патроны"),
            Item("medkit", "rare", "Аптечка"),
            Item("grenade", "epic", "Граната"),
            Item("compass", "common", "Компас"),
            Item("bullets", "common", "Патроны")
        };

        Player first("Дмитрий", 5, 12, 85, "Осматривает руины", items1);
        Player second("Алексей", 23, 44, 100, "Чинит генератор", items2);
        Player third = first + second;

        first -= second;

        first.DisplayAll();
        second.DisplayAll();
        third.DisplayAll();
    }
    catch (const invalid_argument& err)
    {
        cout << "Ошибка: " << err.what() << endl;
    }
    catch (...)
    {
        cout << "Неизвестная ошибка" << endl;
    }

    return 0;
}
