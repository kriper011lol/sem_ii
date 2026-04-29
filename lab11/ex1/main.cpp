#include <iostream>
#include <cmath>

using namespace std;

// Структура для хранения дроби
struct Fraction {
    int numerator;     // числитель
    int denominator;   // знаменатель
};


//-------------------------------------------------------------
// Функция ввода дроби с клавиатуры и проверки корректности
//-------------------------------------------------------------
Fraction inputFraction() {
    Fraction f;

    cout << "Введите числитель: ";
    cin >> f.numerator;

    // Проверяем знаменатель (он не должен быть равен 0)
    do {
        cout << "Введите знаменатель (не 0): ";
        cin >> f.denominator;

        if (f.denominator == 0) {
            cout << "Ошибка! Знаменатель не может быть равен 0.\n";
        }

    } while (f.denominator == 0);

    return f;
}


//-------------------------------------------------------------
// Функция нахождения НОД (наибольшего общего делителя)
// Используется алгоритм Евклида
//-------------------------------------------------------------
int gcd(int a, int b) {

    a = abs(a);
    b = abs(b);

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}


//-------------------------------------------------------------
// Функция сокращения дроби
//-------------------------------------------------------------
Fraction reduceFraction(Fraction f) {

    int divisor = gcd(f.numerator, f.denominator);

    f.numerator /= divisor;
    f.denominator /= divisor;

    // Нормализуем знак (знаменатель должен быть положительным)
    if (f.denominator < 0) {
        f.denominator *= -1;
        f.numerator *= -1;
    }

    return f;
}


//-------------------------------------------------------------
// Функция сложения двух дробей
//-------------------------------------------------------------
Fraction sumFractions(Fraction a, Fraction b) {

    Fraction result;

    // формула сложения дробей
    result.numerator = a.numerator * b.denominator + b.numerator * a.denominator;

    result.denominator = a.denominator * b.denominator;

    // сокращаем результат
    result = reduceFraction(result);

    return result;
}


//-------------------------------------------------------------
// Функция подсчета количества цифр в числе
//-------------------------------------------------------------
int countDigits(int number) {

    number = abs(number);

    if (number == 0)
        return 1;

    int count = 0;

    while (number > 0) {
        number /= 10;
        count++;
    }

    return count;
}


//-------------------------------------------------------------
// Функция вывода дроби на экран
//-------------------------------------------------------------
void printFraction(Fraction f) {

    int numDigits = countDigits(f.numerator);
    int denDigits = countDigits(f.denominator);

    int width = max(numDigits, denDigits) + 2; // длина черты

    // вывод числителя
    cout.width(width);
    cout << f.numerator << endl;

    // линия дроби
    for (int i = 0; i < width; i++)
        cout << "-";
    cout << endl;

    // вывод знаменателя
    cout.width(width);
    cout << f.denominator << endl;
}


//-------------------------------------------------------------
// Главная функция программы
//-------------------------------------------------------------
int main() {

    int n;

    cout << "Сколько раз выполнить сложение дробей? ";
    cin >> n;

    for (int i = 0; i < n; i++) {

        cout << "\nДробь 1:\n";
        Fraction f1 = inputFraction();

        cout << "Дробь 2:\n";
        Fraction f2 = inputFraction();

        // считаем сумму
        Fraction result = sumFractions(f1, f2);

        cout << "\nРезультат:\n";
        printFraction(result);
    }

    return 0;
}