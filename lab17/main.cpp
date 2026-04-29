#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
#include <ctime>
#include <cstdlib>
#include <clocale>

using namespace std;

// Максимальный размер для режима с фиксированным массивом
const int MAX_SIZE = 100;

// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ВВОДА
// Чтение целого числа в заданном диапазоне
int readIntInRange(const string& prompt, int minValue, int maxValue) {
    int x;

    while (true) {
        cout << prompt;

        if (cin >> x && x >= minValue && x <= maxValue) {
            return x;
        }

        cout << "Ошибка: нужно ввести целое число от " << minValue << " до " << maxValue << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Чтение любого целого числа без ограничения по диапазону
int readAnyInt(const string& prompt) {
    int x;

    while (true) {
        cout << prompt;

        if (cin >> x) {
            return x;
        }

        cout << "Ошибка: нужно ввести целое число.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Вывод текста одновременно на экран и в файл
template <typename T>
void printBoth(const T& text, ostream& fileOut) {
    cout << text;
    fileOut << text;
}

// ФУНКЦИИ ДЛЯ ФИКСИРОВАННОГО МАССИВА
// Ввод матрицы с клавиатуры
void fillMatrixKeyboard(int a[][MAX_SIZE], int rows, int cols) {
    cout << "Введите элементы матрицы по одному.\n";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            string prompt = "Элемент [" + to_string(i + 1) + "][" + to_string(j + 1) + "]: ";
            a[i][j] = readAnyInt(prompt);
        }
    }
}

// Заполнение матрицы случайными числами
void fillMatrixRandom(int a[][MAX_SIZE], int rows, int cols, int minVal, int maxVal) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            a[i][j] = minVal + rand() % (maxVal - minVal + 1);
        }
    }
}

// Ввод матрицы из файла.
// Если чисел в файле меньше, чем нужно, недостающие элементы заполняются нулями.
bool fillMatrixFromFile(int a[][MAX_SIZE], int rows, int cols, const string& filename, bool& wasShort) {
    ifstream fin(filename);

    wasShort = false;

    if (!fin.is_open()) {
        return false;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!(fin >> a[i][j])) {
                a[i][j] = 0;
                wasShort = true;
            }
        }
    }

    return true;
}

// Вывод матрицы
void printMatrix(int a[][MAX_SIZE], int rows, int cols, ostream& out) {
    out << "Матрица " << rows << " x " << cols << ":\n";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            out << setw(8) << a[i][j];
        }
        out << '\n';
    }

    out << '\n';
}

// Поиск максимума каждого столбца и количества таких максимумов
void findColumnMaxAndCount(int a[][MAX_SIZE], int rows, int cols, int maxValues[], int counts[]) {
    for (int j = 0; j < cols; ++j) {
        maxValues[j] = a[0][j];
        counts[j] = 1;

        for (int i = 1; i < rows; ++i) {
            if (a[i][j] > maxValues[j]) {
                maxValues[j] = a[i][j];
                counts[j] = 1;
            } else if (a[i][j] == maxValues[j]) {
                counts[j]++;
            }
        }
    }
}

// Сумма положительных чисел в заданной строке
long long sumPositiveInRow(int a[][MAX_SIZE], int cols, int rowIndex) {
    long long sum = 0;

    for (int j = 0; j < cols; ++j) {
        if (a[rowIndex][j] > 0) {
            sum += a[rowIndex][j];
        }
    }

    return sum;
}

// ФУНКЦИИ ДЛЯ ДИНАМИЧЕСКОГО МАССИВА
// Выделение памяти под динамическую матрицу
void allocateMatrix(int**& a, int rows, int cols) {
    a = nullptr;
    int allocatedRows = 0;

    try {
        a = new int*[rows];

        for (int i = 0; i < rows; ++i) {
            a[i] = new int[cols]{}; // {} — сразу заполняем нулями
            allocatedRows++;
        }
    } catch (...) {
        if (a != nullptr) {
            for (int i = 0; i < allocatedRows; ++i) {
                delete[] a[i];
            }
            delete[] a;
            a = nullptr;
        }
        throw;
    }
}

// Освобождение памяти
void freeMatrix(int** a, int rows) {
    if (a == nullptr) {
        return;
    }

    for (int i = 0; i < rows; ++i) {
        delete[] a[i];
    }
    delete[] a;
}

// Ввод матрицы с клавиатуры
void fillMatrixKeyboard(int** a, int rows, int cols) {
    cout << "Введите элементы матрицы по одному.\n";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            string prompt = "Элемент [" + to_string(i + 1) + "][" + to_string(j + 1) + "]: ";
            a[i][j] = readAnyInt(prompt);
        }
    }
}

// Заполнение матрицы случайными числами
void fillMatrixRandom(int** a, int rows, int cols, int minVal, int maxVal) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            a[i][j] = minVal + rand() % (maxVal - minVal + 1);
        }
    }
}

// Ввод матрицы из файла.
// Если чисел в файле меньше, чем нужно, недостающие элементы заполняются нулями.
bool fillMatrixFromFile(int** a, int rows, int cols, const string& filename, bool& wasShort) {
    ifstream fin(filename);

    wasShort = false;

    if (!fin.is_open()) {
        return false;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!(fin >> a[i][j])) {
                a[i][j] = 0;
                wasShort = true;
            }
        }
    }

    return true;
}

// Вывод матрицы
void printMatrix(int** a, int rows, int cols, ostream& out) {
    out << "Матрица " << rows << " x " << cols << ":\n";

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            out << setw(8) << a[i][j];
        }
        out << '\n';
    }

    out << '\n';
}

// Поиск максимума каждого столбца и количества таких максимумов
void findColumnMaxAndCount(int** a, int rows, int cols, int maxValues[], int counts[]) {
    for (int j = 0; j < cols; ++j) {
        maxValues[j] = a[0][j];
        counts[j] = 1;

        for (int i = 1; i < rows; ++i) {
            if (a[i][j] > maxValues[j]) {
                maxValues[j] = a[i][j];
                counts[j] = 1;
            } else if (a[i][j] == maxValues[j]) {
                counts[j]++;
            }
        }
    }
}

// Сумма положительных чисел в заданной строке
long long sumPositiveInRow(int** a, int cols, int rowIndex) {
    long long sum = 0;

    for (int j = 0; j < cols; ++j) {
        if (a[rowIndex][j] > 0) {
            sum += a[rowIndex][j];
        }
    }

    return sum;
}

// ВЫВОД РЕЗУЛЬТАТОВ
// Вывод максимума и количества максимумов по каждому столбцу
void printColumnResults(const int maxValues[], const int counts[], int cols, ostream& out) {
    out << "Результат по столбцам:\n";

    for (int j = 0; j < cols; ++j) {
        out << "Столбец " << j + 1
            << ": максимум = " << maxValues[j]
            << ", количество таких максимумов = " << counts[j] << '\n';
    }

    out << '\n';
}

// РЕЖИМ С ФИКСИРОВАННЫМ МАССИВОМ

void runFixedMode(ostream& report) {
    printBoth("Режим: массив фиксированного размера\n", report);


    // Ввод размеров матрицы с проверкой
    int rows = readIntInRange("Введите количество строк (1..100): ", 1, MAX_SIZE);
    int cols = readIntInRange("Введите количество столбцов (1..100): ", 1, MAX_SIZE);

    // Сам массив фиксированного размера
    int a[MAX_SIZE][MAX_SIZE];

    // Выбор способа заполнения
    printBoth("Выберите способ заполнения матрицы:\n", report);
    printBoth("1 - с клавиатуры\n", report);
    printBoth("2 - случайными числами\n", report);
    printBoth("3 - из файла\n", report);

    int fillChoice = readIntInRange("Ваш выбор: ", 1, 3);

    if (fillChoice == 1) {
        printBoth("\nЗаполнение матрицы с клавиатуры.\n", report);
        fillMatrixKeyboard(a, rows, cols);
    } else if (fillChoice == 2) {
        printBoth("\nЗаполнение матрицы случайными числами.\n", report);

        int minVal, maxVal;

        while (true) {
            minVal = readAnyInt("Введите минимальное случайное число: ");
            maxVal = readAnyInt("Введите максимальное случайное число: ");

            if (minVal <= maxVal) {
                break;
            }

            cout << "Ошибка: минимальное число не должно быть больше максимального.\n";
        }

        fillMatrixRandom(a, rows, cols, minVal, maxVal);
    } else {
        printBoth("\nЗаполнение матрицы из файла.\n", report);

        while (true) {
            string filename;
            cout << "Введите имя файла: ";
            cin >> filename;

            bool wasShort = false;

            if (fillMatrixFromFile(a, rows, cols, filename, wasShort)) {
                if (wasShort) {
                    printBoth("В файле оказалось меньше чисел, чем нужно.\n"
                              "Недостающие элементы заполнены нулями.\n", report);
                }
                break;
            }

            cout << "Файл не удалось открыть. Попробуйте ещё раз.\n";
        }
    }

    // Вывод исходной матрицы
    printBoth("\nИсходная матрица:\n", report);
    printMatrix(a, rows, cols, cout);
    printMatrix(a, rows, cols, report);

    // Вариант 10, пункт 1:
    // для каждого столбца ищем максимум и количество его вхождений
    int maxValues[MAX_SIZE];
    int counts[MAX_SIZE];

    findColumnMaxAndCount(a, rows, cols, maxValues, counts);

    printBoth("Максимумы по столбцам и количество таких максимумов:\n", report);
    printColumnResults(maxValues, counts, cols, cout);
    printColumnResults(maxValues, counts, cols, report);

    // Вариант 10, пункт 2:
    // сумма положительных чисел в заданной строке
    int rowNumber = readIntInRange("Введите номер строки для поиска суммы положительных чисел: ", 1, rows);

    long long sum = sumPositiveInRow(a, cols, rowNumber - 1);

    cout << "Сумма положительных чисел в строке " << rowNumber << ": " << sum << '\n';
    report << "Сумма положительных чисел в строке " << rowNumber << ": " << sum << '\n';
    report << '\n';
}

// РЕЖИМ С ДИНАМИЧЕСКИМ МАССИВОМ
void runDynamicMode(ostream& report) {
    printBoth("Режим: динамический массив\n", report);


    // Ввод размеров матрицы
    int rows = readIntInRange("Введите количество строк (больше 0): ", 1, numeric_limits<int>::max());
    int cols = readIntInRange("Введите количество столбцов (больше 0): ", 1, numeric_limits<int>::max());

    // Выделение памяти под динамическую матрицу
    int** a = nullptr;

    try {
        allocateMatrix(a, rows, cols);
    } catch (const bad_alloc&) {
        cout << "Ошибка: не удалось выделить память под матрицу.\n";
        report << "Ошибка: не удалось выделить память под матрицу.\n";
        return;
    }

    // Выбор способа заполнения
    printBoth("Выберите способ заполнения матрицы:\n", report);
    printBoth("1 - с клавиатуры\n", report);
    printBoth("2 - случайными числами\n", report);
    printBoth("3 - из файла\n", report);

    int fillChoice = readIntInRange("Ваш выбор: ", 1, 3);

    if (fillChoice == 1) {
        printBoth("\nЗаполнение матрицы с клавиатуры.\n", report);
        fillMatrixKeyboard(a, rows, cols);
    } else if (fillChoice == 2) {
        printBoth("\nЗаполнение матрицы случайными числами.\n", report);

        int minVal, maxVal;

        while (true) {
            minVal = readAnyInt("Введите минимальное случайное число: ");
            maxVal = readAnyInt("Введите максимальное случайное число: ");

            if (minVal <= maxVal) {
                break;
            }

            cout << "Ошибка: минимальное число не должно быть больше максимального.\n";
        }

        fillMatrixRandom(a, rows, cols, minVal, maxVal);
    } else {
        printBoth("\nЗаполнение матрицы из файла.\n", report);

        while (true) {
            string filename;
            cout << "Введите имя файла: ";
            cin >> filename;

            bool wasShort = false;

            if (fillMatrixFromFile(a, rows, cols, filename, wasShort)) {
                if (wasShort) {
                    printBoth("В файле оказалось меньше чисел, чем нужно.\n"
                              "Недостающие элементы заполнены нулями.\n", report);
                }
                break;
            }

            cout << "Файл не удалось открыть. Попробуйте ещё раз.\n";
        }
    }

    // Вывод исходной матрицы
    printBoth("\nИсходная матрица:\n", report);
    printMatrix(a, rows, cols, cout);
    printMatrix(a, rows, cols, report);

    // Вариант 10, пункт 1
    int* maxValues = nullptr;
    int* counts = nullptr;

    try {
        maxValues = new int[cols];
        counts = new int[cols];
    } catch (const bad_alloc&) {
        cout << "Ошибка: не удалось выделить память под вспомогательные массивы.\n";
        report << "Ошибка: не удалось выделить память под вспомогательные массивы.\n";
        freeMatrix(a, rows);
        return;
    }

    findColumnMaxAndCount(a, rows, cols, maxValues, counts);

    printBoth("Максимумы по столбцам и количество таких максимумов:\n", report);
    printColumnResults(maxValues, counts, cols, cout);
    printColumnResults(maxValues, counts, cols, report);

    // Вариант 10, пункт 2
    int rowNumber = readIntInRange("Введите номер строки для поиска суммы положительных чисел: ", 1, rows);

    long long sum = sumPositiveInRow(a, cols, rowNumber - 1);

    cout << "Сумма положительных чисел в строке " << rowNumber << ": " << sum << '\n';
    report << "Сумма положительных чисел в строке " << rowNumber << ": " << sum << '\n';
    report << '\n';

    // Освобождаем память
    delete[] maxValues;
    delete[] counts;
    freeMatrix(a, rows);
}

int main() {
    // Чтобы русский текст корректнее отображался в некоторых средах
    setlocale(LC_ALL, "");

    // Случайные числа будут разными при каждом запуске
    srand(static_cast<unsigned>(time(nullptr)));

    ofstream report("result.txt");

    if (!report.is_open()) {
        cout << "Ошибка: не удалось открыть файл result.txt для записи.\n";
        return 1;
    }

    printBoth("Лабораторная работа по обработке матриц.\n", report);
    printBoth("Вариант 10.\n\n", report);

    printBoth("1 - массив фиксированного размера\n", report);
    printBoth("2 - динамический массив\n\n", report);

    int mode = readIntInRange("Выберите режим: ", 1, 2);

    if (mode == 1) {
        runFixedMode(report);
    } else {
        runDynamicMode(report);
    }

    printBoth("Работа программы завершена.\n", report);
    printBoth("Результаты записаны в файл result.txt\n", report);

    return 0;
}