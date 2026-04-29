#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    string s;
    
    // Ввод с проверкой
    do {
        cout << "Введите строку, заканчивающуюся точкой:\n";
        getline(cin, s);

        if (s.empty() || s.back() != '.') {
            cout << "Ошибка: строка должна заканчиваться точкой.\n\n";
        }

    } while (s.empty() || s.back() != '.');

    // Убираем точку перед подсчётом
    s.pop_back();

    int count = 0;
    bool inWord = false;

    for (char ch : s) {
        if (isspace(static_cast<unsigned char>(ch))) {
            inWord = false;
        } else {
            if (!inWord) {
                count++;
                inWord = true;
            }
        }
    }

    cout << "Количество слов в строке: " << count << endl;

    return 0;
}