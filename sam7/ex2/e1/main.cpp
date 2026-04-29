#include <iostream>
#include <string>

using namespace std;


// Рекурсивная функция перевода из p-ичной системы в 10-ичную
// str - строка с числом, p - основание, index - текущий символ, result - аккумулятор
long long pToDecimal(const std::string& str, int p, int index = 0, long long result = 0) {
    // Базовый случай: дошли до конца строки
    if (index == str.length()) {
        return result;
    }
    
    // Преобразуем символ в цифру
    int digit = str[index] - '0';
    
    // Проверка на корректность цифры для данной системы счисления
    if (digit < 0 || digit >= p) {
        cout << "\nОшибка: цифра '" << digit << "' недопустима для " << p << "-ичной системы!" << endl;
        return -1; 
    }
    
    // Рекурсивный шаг
    return pToDecimal(str, p, index + 1, result * p + digit);
}

int main() {

    
    string s;
    int p;
    
    cout << "Введите основание системы счисления p (2 <= p <= 9): ";
    cin >> p;
    
    if (p < 2 || p > 9) {
        cout << "Основание должно быть от 2 до 9." << endl;
        return 1;
    }

    cout << "Введите число в " << p << "-ичной системе: ";
    cin >> s;

    long long decimalValue = pToDecimal(s, p);
    
    if (decimalValue != -1) {
        cout << "Число в десятичной системе: " << decimalValue << endl;
    }

    return 0;
}