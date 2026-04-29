#include <iostream>

using namespace std;


// Рекурсивная функция для получения перевернутого числа
// num - оставшаяся часть числа, result - новое числа
long long reverseNumber(long long num, long long result = 0) {
    // Базовый случай: все цифры обработаны
    if (num == 0) {
        return result;
    }
    // Рекурсивный шаг: сдвигаем результат на разряд влево (* 10) 
    // и прибавляем последнюю цифру исходного числа (num % 10)
    return reverseNumber(num / 10, result * 10 + num % 10);
}

int main() {
    
    long long n;
    cout << "Введите натуральное число: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Ошибка: число должно быть натуральным (больше 0)." << endl;
        return 1;
    }

    cout << "Число в обратном порядке: " << reverseNumber(n) << endl;

    return 0;
}