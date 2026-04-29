#include <iostream>
using namespace std;

// Структура для хранения дроби
struct Fraction {
    long long num;   // числитель
    long long den;   // знаменатель
};


//----------------------------------------------------
// Функция нахождения НОД (алгоритм Евклида)
//----------------------------------------------------
long long gcd(long long a, long long b)
{
    while (b != 0)
    {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}


//----------------------------------------------------
// Функция сокращения дроби
//----------------------------------------------------
Fraction reduce(Fraction f)
{
    long long g = gcd(abs(f.num), abs(f.den));

    f.num /= g;
    f.den /= g;

    return f;
}


//----------------------------------------------------
// Функция сложения двух дробей
//----------------------------------------------------
Fraction add(Fraction a, Fraction b)
{
    Fraction r;

    r.num = a.num * b.den + b.num * a.den;
    r.den = a.den * b.den;

    return reduce(r);
}


//----------------------------------------------------
// Функция деления 1 на дробь
// 1/(a/b) = b/a
//----------------------------------------------------
Fraction inverse(Fraction f)
{
    Fraction r;

    r.num = f.den;
    r.den = f.num;

    return r;
}


//----------------------------------------------------
// Функция вычисления n-го элемента последовательности
//----------------------------------------------------
Fraction findElement(int n)
{
    // первые два элемента последовательности
    Fraction a1 = {1, 2};
    Fraction a2 = {1, 3};

    if (n == 1) return a1;
    if (n == 2) return a2;

    Fraction prev = a1;
    Fraction curr = a2;
    Fraction next;

    // вычисляем элементы начиная с 3
    for (int i = 3; i <= n; i++)
    {
        // (a_n + 1)
        Fraction one = {1,1};
        Fraction temp1 = add(curr, one);

        // 1/(a_n + 1)
        Fraction part1 = inverse(temp1);

        // 1/(a_{n-1})
        Fraction part2 = inverse(prev);

        // сумма
        next = add(part1, part2);

        // сдвигаем элементы
        prev = curr;
        curr = next;
    }

    return curr;
}


//----------------------------------------------------
// Главная функция
//----------------------------------------------------
int main()
{
    int n;

    cout << "Введите номер элемента n: ";
    cin >> n;

    if (n <= 0)
    {
        cout << "Ошибка: n должно быть больше 0\n";
        return 0;
    }

    Fraction result = findElement(n);

    cout << "a_" << n << " = " << result.num << "/" << result.den << endl;

    return 0;
}