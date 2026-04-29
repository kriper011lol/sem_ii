#include <iostream> 
#include <fstream>    // библиотека для работы с файлами
#include <cstdlib>    // библиотека для rand()
#include <ctime>      // библиотека для srand()

using namespace std;


/*
Функция-предикат
Проверяет допустимость длины последовательности.
Возвращает true если длина допустима.
Возвращает false если нет.
*/
bool CheckLength(int n)
{
    if (n > 1)   // последовательность должна содержать минимум 2 элемента
        return true;
    else
        return false;
}


/*
Перегруженная функция №1
Заполнение файла числами с клавиатуры.

Параметры:
n - длина последовательности
Name - имя файла

Возвращает:
1 - файл успешно создан
0 - файл не создан
*/
int CreateFile(int n, char Name[])
{
    ofstream fout(Name);   // создаём поток записи в файл

    if (!fout)             // проверяем открылся ли файл
        return 0;

    cout << "Введите " << n << " чисел:\n";

    for (int i = 0; i < n; i++)   // цикл ввода чисел
    {
        int x;
        cin >> x;                 // ввод числа
        fout << x << " ";         // запись числа в файл
    }

    fout.close();                 // закрываем файл

    return 1;
}


/*
Перегруженная функция №2
Заполнение файла случайными числами.

Параметры:
Name - имя файла
n - длина последовательности
a,b - диапазон случайных чисел
*/
int CreateFile(char Name[], int n, int a, int b)
{
    ofstream fout(Name);  // создаём файл

    if (!fout)            // проверяем открылся ли файл
        return 0;

    for (int i = 0; i < n; i++)   // цикл генерации чисел
    {
        int x = a + rand() % (b - a + 1); // случайное число из диапазона
        fout << x << " ";                 // запись числа в файл
    }

    fout.close();   // закрываем файл

    return 1;
}


/*
Функция вывода текстового файла на экран.
Используется символьный массив для имени файла.

Пример вызова:
Print_f("Input.txt")
*/
void Print_f(char Name[])
{
    ifstream fin(Name);   // открываем файл для чтения

    int x;

    cout << "\nСодержимое файла:\n";

    while (fin >> x)      // читаем числа пока файл не закончится
    {
        cout << x << " "; // вывод числа на экран
    }

    cout << endl;

    fin.close();          // закрываем файл
}


/*
Функция выполнения индивидуального задания.

Читает данные из текстового файла
Записывает произведения соседних элементов в бинарный файл.

Параметры:
входной поток fin
выходной поток fout

Возвращает:
количество элементов записанных в бинарный файл
*/
int CreateBinary(ifstream &fin, ofstream &fout)
{
    int prev;   // предыдущий элемент
    int curr;   // текущий элемент

    if (!(fin >> prev))   // если файл пуст
        return 0;

    int count = 0;        // количество записанных элементов

    while (fin >> curr)   // читаем следующий элемент
    {
        int product = prev * curr;   // произведение соседних элементов

        fout.write((char*)&product, sizeof(product));
        // запись числа в бинарный файл

        prev = curr;      // текущий становится предыдущим

        count++;          // увеличиваем счётчик
    }

    return count;
}


/*
Функция вывода бинарного файла на экран.
Параметр — символьный массив имени файла.
*/
void PrintBinary(char Name[])
{
    ifstream fin(Name, ios::binary);   // открываем бинарный файл

    int x;

    cout << "\nСодержимое бинарного файла:\n";

    while (fin.read((char*)&x, sizeof(x)))  // читаем бинарные данные
    {
        cout << x << " ";                   // выводим число
    }

    cout << endl;

    fin.close();   // закрываем файл
}



int main()
{
    srand(time(0));  // инициализация генератора случайных чисел

    int n;

    cout << "Введите длину последовательности: ";
    cin >> n;

    // проверка допустимости длины
    if (!CheckLength(n))
    {
        cout << "Ошибка: длина должна быть больше 1\n";
        return 0;
    }

    char InputName[] = "Input.txt";    // имя текстового файла
    char OutputName[] = "Output.bin";  // имя бинарного файла


    cout << "\n1 - ввод с клавиатуры\n";
    cout << "2 - случайные числа\n";

    int choice;
    cin >> choice;


    int result;   // результат создания файла


    if (choice == 1)
        result = CreateFile(n, InputName);        // ввод с клавиатуры
    else
        result = CreateFile(InputName, n, -10, 10); // случайные числа


    if (!result)
    {
        cout << "Ошибка создания файла\n";
        return 0;
    }


    // Задание 2 — вывод файла
    Print_f(InputName);


    // Задание 3 — создание бинарного файла
    ifstream fin(InputName);                 // открываем текстовый файл
    ofstream fout(OutputName, ios::binary);  // создаём бинарный файл

    int count = CreateBinary(fin, fout);     // вызываем функцию

    fin.close();
    fout.close();


    if (count == 0)
    {
        cout << "Бинарный файл не создан\n";
        return 0;
    }


    // Задание 4 — вывод бинарного файла
    PrintBinary(OutputName);


    return 0;
}