#include <iostream>

using namespace std;

int main()
{
    int month;          // номер месяца
    int days;           // количество дней в месяце
    int firstDay;       // день недели первого числа

    cout << "Введите номер месяца (1-12): ";
    cin >> month;

    // определяем количество дней в месяце
    switch(month)
    {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            days = 31;
            break;

        case 4: case 6: case 9: case 11:
            days = 30;
            break;

        case 2:
            days = 28; // без учета високосного года
            break;

        default:
            cout << "Неверный номер месяца!";
            return 0;
    }

    cout << "Введите день недели 1-го числа (1-Пн ... 7-Вс): ";
    cin >> firstDay;

    cout << "\nВыходные дни месяца:\n";

    // перебираем все дни месяца
    for(int day = 1; day <= days; day++)
    {
        int weekday = (firstDay + day - 2) % 7 + 1;

        // если суббота или воскресенье
        if(weekday == 6 || weekday == 7)
        {
            cout << day << " ";
        }
    }

    cout << endl;

    return 0;
}