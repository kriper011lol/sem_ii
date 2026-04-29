#include <iostream>
#include <cmath> // Математические функции

using namespace std;

int main()
{
  cout << "Фамилия: Нгуен" << endl;
  cout << "Группа: ДИНРб-11" << endl;
  cout << "Вариант: 10" << endl;
  cout << "y = sqrt(a + sin(x)) / cbrt(4 + cos(x))" << endl
    << endl;

  int N;
  double a, x_min, x_max;

  cout << "Введите N: ";
  cin >> N;

  cout << "Введите a: ";
  cin >> a;

  cout << "Введите x_min: ";
  cin >> x_min;

  cout << "Введите x_max: ";
  cin >> x_max;

  double h = (x_max - x_min) / N;
  double x, y;

  // ===== while =====
  cout << endl
      << "Цикл while" << endl;
  cout << "x\t\ty" << endl;

  int i = 0;
  x = x_min;

  while (i <= N)
  {
    y = sqrt(a + sin(x)) / cbrt(4 + cos(x));
    cout << x << "\t" << y << endl;
    x += h;
    i++;
  }

  // ===== do while =====
  cout << endl
      << "Цикл do while" << endl;
  cout << "x\t\ty" << endl;

  i = 0;
  x = x_min;

  do
  {
    y = sqrt(a + sin(x)) / cbrt(4 + cos(x));
    cout << x << "\t" << y << endl;
    x += h;
    i++;
  } while (i <= N);

  // ===== for =====
  cout << endl
      << "Цикл for" << endl;
  cout << "x\t\ty" << endl;

  for (i = 0; i <= N; i++)
  {
    x = x_min + i * h;
    y = sqrt(a + sin(x)) / cbrt(4 + cos(x));
    cout << x << "\t" << y << endl;
  }

  return 0;
}