#include <iostream>
#include <cmath>
using namespace std;


// 1 СПОСОБ — ПОЛНОЕ ВЕТВЛЕНИЕ (if / else)

double fullBranch(double x)
{
    double y;

    if (x < -14)
        y = x * fabs(x + 21);

    else if (x >= -14 && x < -5)
        y = pow(x, 2) * log(fabs(pow(x, 2) + 48));

    else if (x >= -5 && x < 0)
        y = sqrt(pow(x, 2) + 16) - (1.0 / 3) * x;

    else
        y = 2 + x / 3;

    return y;
}

// 2 СПОСОБ — КРАТКАЯ ФОРМА (логические выражения)

double shortBranch(double x)
{
    double y = 0;

    if (x < -14)
        y = x * fabs(x + 21);

    if (x >= -14 && x < -5)
        y = pow(x, 2) * log(fabs(pow(x, 2) + 48));

    if (x >= -5 && x < 0)
        y = sqrt(pow(x, 2) + 16) - (1.0 / 3) * x;

    if (x >= 0)
        y = 2 + x / 3;

    return y;
}

// 3 СПОСОБ — УСЛОВНАЯ ОПЕРАЦИЯ (тернарный оператор)

double ternary(double x)
{
    double y;

    y = (x < -14) ? x * fabs(x + 21) :
        (x < -5)  ? pow(x, 2) * log(fabs(pow(x, 2) + 48)) :
        (x < 0)   ? sqrt(pow(x, 2) + 16) - (1.0 / 3) * x : 2 + x / 3;

    return y;
}

// ГЛАВНАЯ ФУНКЦИЯ

int main()
{
    double x, y;
    int choice;

    cout << "Введите x: ";
    cin >> x;

    cout << "\nВыберите способ решения:\n";
    cout << "1 - Полное ветвление\n";
    cout << "2 - Краткая форма\n";
    cout << "3 - Условная операция\n";
    cout << "Ваш выбор: ";

    cin >> choice;

    switch (choice)
    {
        case 1:
            y = fullBranch(x);
            break;
        case 2:
            y = shortBranch(x);
            break;
        case 3:
            y = ternary(x);
            break;
        default:
            cout << "Неверный выбор!";
            return 0;
    }

    cout << "\nРезультат: y = " << y << endl;

    return 0;
}