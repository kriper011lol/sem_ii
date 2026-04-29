#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

int main() {
    srand(time(0));


    // Задание 1

    int n = 10 + rand() % 21;
    int choice;

    cout << "Длина последовательности: " << n << endl;
    cout << "1 - случайные числа\n2 - ввод с клавиатуры\nВаш выбор: ";
    if (!(cin >> choice)) {
        choice = 1;
    }

    ofstream fout("Input.txt");

    for (int i = 0; i < n; i++) {
        int x;

        if (choice == 2) {
            cout << "Введите " << (i + 1) << "-е число: ";
            cin >> x;
        } else {
            x = rand() % 199 - 99;
        }

        fout << x << " ";
    }

    fout.close();


    // Вывод всей последовательности

    ifstream show("Input.txt");
    int x;

    cout << "\nИсходная последовательность:\n";
    while (show >> x) {
        cout << x << " ";
    }
    cout << endl;

    show.close();


    // Задание 2

    ifstream fin("Input.txt");

    int prev, curr;
    int countPairs = 0;

    cout << "\nПары с разностью 2:\n";

    if (fin >> prev) {
        while (fin >> curr) {
            if (abs(curr - prev) == 2) {
                cout << "(" << prev << ", " << curr << ")\n";
                countPairs++;
            }
            prev = curr;
        }
    }

    fin.close();

    cout << "Количество пар: " << countPairs << endl;


    // Задание 3

    fin.open("Input.txt");
    ofstream fout2("Output.txt");

    int oddCount = 0;

    cout << "\nКаждое второе нечётное число:\n";

    while (fin >> x) {
        if (x % 2 != 0) {
            oddCount++;

            if (oddCount % 2 == 0) {
                cout << x << " ";
                fout2 << x << " ";
            }
        }
    }

    cout << endl;

    fin.close();
    fout2.close();


    // Задание 4

    ifstream fin2("Output.txt");

    int maxValue, pos = 1, index = 1;

    if (!(fin2 >> maxValue)) {
        cout << "Файл Output.txt пуст: в исходной последовательности меньше двух нечётных чисел.\n";
        return 0;
    }

    while (fin2 >> x) {
        index++;

        if (x > maxValue) {
            maxValue = x;
            pos = index;
        }
    }

    fin2.close();

    cout << "\nМаксимальное число в Output.txt: " << maxValue << endl;
    cout << "Его номер в Output.txt: " << pos << endl;

    return 0;
}
