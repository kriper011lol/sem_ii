#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>   

using namespace std;

// Максимальный размер для фиксированного массива
const int MAX_SIZE = 100;

// Ввод размера с проверкой

int inputSize(int maxSize) {
    int n;
    cout << "Введите размер массива (1 - " << maxSize << "): ";
    cin >> n;

    while (n < 1 || n > maxSize) {
        cout << "Ошибка! Введите корректный размер: ";
        cin >> n;
    }

    return n;
}


// Заполнение массива с клавиатуры

void fillKeyboard(int* arr, int n) {
    cout << "Введите элементы массива:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
}


// Заполнение массива случайными числами

void fillRandom(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 50 - 25;   // числа от -25 до 24
    }
}

// Вывод массива

void printArray(int* arr, int n) {
    cout << "Массив: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Проверка: простое ли число

bool isPrime(int x) {
    if (x <= 1) return false; // 0, 1 и отрицательные числа не простые

    for (int i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            return false;
        }
    }

    return true;
}


// Обработка по варианту 10:
// 1) найти номер элемента минимального простого числа
// 2) найти сумму модулей элементов после первого нуля

void process(int* arr, int n) {
    // 1. Поиск минимального простого числа
    int minPrimeValue = 0;   // значение минимального простого числа
    int minPrimeIndex = -1;  // индекс этого числа

    for (int i = 0; i < n; i++) {
        if (isPrime(arr[i])) {
            if (minPrimeIndex == -1 || arr[i] < minPrimeValue) {
                minPrimeValue = arr[i];
                minPrimeIndex = i;
            }
        }
    }

    if (minPrimeIndex == -1) {
        cout << "Простых чисел в массиве нет." << endl;
    } else {
        cout << "Номер элемента минимального простого числа: "
          << minPrimeIndex + 1 << endl;
        cout << "Само минимальное простое число: " << minPrimeValue << endl;
    }

    // 2. Сумма модулей элементов после первого нуля
    int zeroIndex = -1;

    for (int i = 0; i < n; i++) {
        if (arr[i] == 0) {
            zeroIndex = i;
            break;
        }
    }

    if (zeroIndex == -1) {
        cout << "Ноль не найден." << endl;
    } else {
        int sumAbs = 0;

        for (int i = zeroIndex + 1; i < n; i++) {
            sumAbs += abs(arr[i]);
        }

        cout << "Сумма модулей элементов после первого нуля: " << sumAbs << endl;
    }
}

// Фиксированный массив

void mainArrayS() {
    int arr[MAX_SIZE];
    int n = inputSize(MAX_SIZE);

    int choice;
    cout << "1 - ввод с клавиатуры\n";
    cout << "2 - случайные числа\n";
    cout << "Выбор: ";
    cin >> choice;

    if (choice == 1) {
        fillKeyboard(arr, n);
    } else {
        fillRandom(arr, n);
    }

    printArray(arr, n);
    process(arr, n);
}

// Динамический массив

void mainArrayD() {
    int n = inputSize(1000);

    int* arr = new int[n];

    int choice;
    cout << "1 - ввод с клавиатуры\n";
    cout << "2 - случайные числа\n";
    cout << "Выбор: ";
    cin >> choice;

    if (choice == 1) {
        fillKeyboard(arr, n);
    } else {
        fillRandom(arr, n);
    }

    printArray(arr, n);
    process(arr, n);

    delete[] arr;
}

// vector

void mainVector() {
    int n = inputSize(1000);

    vector<int> arr(n);

    int choice;
    cout << "1 - ввод с клавиатуры\n";
    cout << "2 - случайные числа\n";
    cout << "Выбор: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Введите элементы массива:\n";
        for (int i = 0; i < n; i++) {
            cin >> arr[i];
        }
    } else {
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 50 - 25;
        }
    }

    cout << "Массив: ";
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;

    process(arr.data(), n);
}


// Главная функция

int main() {
    srand(time(0));

    int mode;

    do {
        cout << "\nВыберите тип массива:\n";
        cout << "1 - фиксированный\n";
        cout << "2 - динамический\n";
        cout << "3 - vector\n";
        cout << "0 - выход\n";
        cout << "Ваш выбор: ";
        cin >> mode;

        switch (mode) {
            case 1:
                mainArrayS();
                break;
            case 2:
                mainArrayD();
                break;
            case 3:
                mainVector();
                break;
            case 0:
                cout << "Выход из программы." << endl;
                break;
            default:
                cout << "Неверный выбор." << endl;
        }

    } while (mode != 0);

    return 0;
}