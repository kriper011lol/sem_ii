#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main()
{
    double a, b, c, d, dx, dy;
    double x, y, z;

    cout << "Введите a b: ";
    cin >> a >> b;

    cout << "Введите c d: ";
    cin >> c >> d;

    cout << "Введите dx dy: ";
    cin >> dx >> dy;

    cout << fixed << setprecision(2);

    cout << endl;
    cout << "Z(x,y) = arccos(x*y / sqrt(1 - x*y))" << endl << endl;

    // верхняя строка
    cout << setw(6) << "Y\\X" << " |";

    for (x = a; x <= b + 0.0001; x += dx)
        cout << setw(7) << x;

    cout << endl;

    // линия
    cout << "---------------------------------------------------------------------------------------" << endl;

    // таблица
    for (y = c; y <= d + 0.0001; y += dy)
    {
        cout << setw(6) << y << " |";

        for (x = a; x <= b + 0.0001; x += dx)
        {
            double t = 1 - x * y;

            if (t <= 0)
            {
                cout << setw(7) << "*";
            }
            else
            {
                double value = (x * y) / sqrt(t);

                if (value < -1 || value > 1)
                    cout << setw(7) << "*";
                else
                {
                    z = acos(value);
                    cout << setw(7) << z;
                }
            }
        }

        cout << endl;
    }

    return 0;
}