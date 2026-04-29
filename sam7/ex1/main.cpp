#include <iostream>
#include <cmath> 

using namespace std;


// Функция принимает x, точность eps и ссылку на счетчик итераций
double calculateSeriesSum(double x, double eps, int& iterations) {
    double sum = 0.0;
    int n = 1;
    
    // Вычисляем первый член ряда (T_1)
    double term = -2.0 * x / 9.0; 
    
    iterations = 0;

    
    // Цикл работает, пока по модулю член ряда больше или равен заданной точности.
    while (fabs(term) >= eps) {
        sum += term;        // прибавляем текущий член к сумме
        iterations++;       // увеличиваем счетчик итераций
        
        // Вычисляем следующий член ряда через рекуррентное соотношение
        term *= -2.0 * x / (3.0 * (n + 2)); 
        n++;                // переходим к следующему номеру
    }

    return sum;
}

int main() {
    
    double x, eps;
    cout << "Введите значение x: ";
    cin >> x;
    cout << "Введите точность (например, 0.01): ";
    cin >> eps;

    int iterations = 0;
    double result = calculateSeriesSum(x, eps, iterations);

    cout << "\nРезультат вычислений:\n";
    cout << "Сумма ряда: " << result << "\n";
    cout << "Количество итераций: " << iterations << "\n";

    return 0;
}