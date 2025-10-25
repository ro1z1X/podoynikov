/*
#include <iostream>
#include <string>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    const int size = 6;
    int arr[size];

    cout << "Введите 6 элементов: ";
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }

    bool duplicate = false;
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] == arr[i + 1]) {
            duplicate = true;
            break;
        }
    }

    if (duplicate) {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    int a = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = a;
                }
            }
        }
    }

    cout << "Вывод: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
*/

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    const int row = 3;
    const int col = 4;
    int matr[row][col];

    cout << "Введите элементы матрицы " << row << "x" << col << ":\n";
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cin >> matr[i][j];
        }
    }

    cout << "\nИсход матрица:\n";
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cout << matr[i][j] << "\t";
        }
        cout << endl;
    }

    int msum = -1;
    int mRind = 0;

    for (int i = 0; i < row; i++) {
        int csum = 0;
        for (int j = 0; j < col; j++) {
            csum += abs(matr[i][j]);
        }

        if (csum > msum) {
            msum = csum;
            mRind = i;
        }
    }

    cout << "Строка с наиб суммой модулей: " << mRind + 1 << endl;
    cout << "Сумма модулей: " << msum << endl;

    for (int j = 0; j < col; j++) {
        matr[mRind][j] = 0;
    }

    cout << "\nИтог матрица:\n";
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cout << matr[i][j] << "\t";
        }
        cout << endl;
    }

    return 0;
}
