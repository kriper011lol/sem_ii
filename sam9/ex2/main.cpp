#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

bool isAsciiLetter(unsigned char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

// Подсчет букв в UTF-8 строке:
// русская буква занимает 2 байта, английская - 1 байт
int countLetters(const string& token) {
    int count = 0;

    for (size_t i = 0; i < token.size(); ) {
        unsigned char ch = static_cast<unsigned char>(token[i]);

        if (isAsciiLetter(ch)) {
            count++;
            i++;
        } else if ((ch == 0xD0 || ch == 0xD1) && i + 1 < token.size()) {
            unsigned char next = static_cast<unsigned char>(token[i + 1]);

            bool isRussian =
                (ch == 0xD0 && (next >= 0x90 && next <= 0xBF)) ||
                (ch == 0xD1 && (next >= 0x80 && next <= 0x8F));

            if (isRussian) {
                count++;
                i += 2;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }

    return count;
}

// Обработка одной строки: удаляем слова, в которых меньше 3 букв
string processLine(const string& line) {
    stringstream ss(line);
    string token;
    string result;
    bool firstWord = true;

    while (ss >> token) {
        int letterCount = countLetters(token);

        if (letterCount >= 3) {
            if (!firstWord) {
                result += ' ';
            }
            result += token;
            firstWord = false;
        }
    }

    return result;
}

int main() {
    ifstream fin("Input.txt");
    if (!fin) {
        cout << "Ошибка: не удалось открыть файл Input.txt" << endl;
        return 1;
    }

    ofstream fout("Output.txt");
    if (!fout) {
        cout << "Ошибка: не удалось создать файл Output.txt" << endl;
        return 1;
    }

    string line;

    cout << "Результат обработки текста:\n";

    while (getline(fin, line)) {
        string processed = processLine(line);

        cout << processed << endl;
        fout << processed << endl;
    }

    fin.close();
    fout.close();

    return 0;
}
