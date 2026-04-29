#include <iostream>
#include <cmath>
using namespace std;

// Функция вычисления факториала

double factorial(int n)
{
    double f = 1;

    for(int i = 1; i <= n; i++)
        f *= i;

    return f;
}


int main()
{
    double x;     // значение x
    double eps;   // точность
    double S = 1; // сумма ряда
    double term;  // текущий член ряда
    int n = 1;    // номер члена

    cout << "Введите x: ";
    cin >> x;

    cout << "Введите точность eps (0 < eps < 1): ";
    cin >> eps;

    // вычисление суммы ряда
    do
    {
        term = cos(n * x) / factorial(n);
        S += term;

        n++;

    } while (fabs(term) > eps);


    cout << "\nСумма ряда S = " << S << endl;
    cout << "Количество членов = " << n << endl;

    return 0;
}