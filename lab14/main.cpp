#include <iostream>
#include <string>
#include <fstream> //для работы с файлами
#include <vector> //для работы с векторами (динамическими массивами)
#include <clocale> //для рабоаты с локализацией
#include <Windows.h> //для доступа к функциям Windows API

using namespace std;

//Задание 2: Функция для форматирования строки (удаление лишних пробелов)
string formatString(const string& input) {
    string temp = ""; //временная строка для очистки от лишних пробелов
    bool lastWasSpace = false; //был ли предыдущий символ пробелом

//шаг1: убираем двойные пробелы и пробелы в начале

    for (char c : input) {
        if (c == ' ') {
            if (temp.empty()) continue; //пропускаем пробелы в самом начале строки
            lastWasSpace = true; //запоминаем, что был пробел
        } else {
            if (lastWasSpace) {
                temp += ' '; //добавляем один пробел перед словом
                lastWasSpace = false;
            }
            temp += c;
        }
    }
    
//если строка заканчивается пробелом - удаляем его

    if (!temp.empty() && temp.back() == ' ') {
        temp.pop_back();
    }

    string finalStr = ""; //итоговая строка для обработки знаков препинания
//шаг 2: расставляем пробелы вокруг знаков препинания

    for (size_t i = 0; i < temp.length(); ++i) {
        char c = temp[i];
        //проверяем, является ли символ знаком препинания
        bool isPunctuation = (c == '.' || c == ',' || c == ':' || c == ';' || c == '!' || c == '?');
//удаляем пробел перед знаком препинания
        if (c == ' ' && i + 1 < temp.length()) {
            char nextC = temp[i + 1];
            if (nextC == '.' || nextC == ',' || nextC == ':' || nextC == ';' || nextC == '!' || nextC == '?') {
                continue;
            }
        }

        finalStr += c; //записываем символ
//добавляем пробел после знака препинания (если его там нет и это не конец строки)
        if (isPunctuation && i + 1 < temp.length() && temp[i + 1] != ' ') {
            finalStr += ' ';
        }
    }

    return finalStr;
}
//Задание 4: чтение из Input.txt, форматирование и запись в Output.txt

void processFile(const string& inFileName, const string& outFileName) {
    ifstream inFile(inFileName);
    ofstream outFile(outFileName);

    if (!inFile.is_open()) {
        cout << "Ошибка: Не удалось открыть файл " << inFileName << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        string formattedLine = formatString(line);
        outFile << formattedLine << endl;
    }

    inFile.close();
    outFile.close();
    cout << "Файл успешно обработан. Результат записан в " << outFileName << endl;
}
//задание 5: поиск слов по фрагменту в файле

bool findWordsInFile(const string& fileName, const string& targetWord) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Ошибка: Не удалось открыть файл " << fileName << endl;
        return false;
    }

    string word;
    bool found = false;

    cout << "Слова, содержащие \"" << targetWord << "\":" << endl;

    while (file >> word) {
        if (word.find(targetWord) != string::npos) {
            string cleanWord = "";
            //очищаем найденное слово от прилипших знаков препинания
            for (char c : word) {
                if (c != '.' && c != ',' && c != ';' && c != ':' && c != '!' && c != '?') {
                    cleanWord += c;
                }
            }
            cout << "- " << cleanWord << endl;
            found = true;
        }
    }

    file.close();

    if (!found) {
        cout << "Совпадений не найдено." << endl;
    }

    return found;
}
//задание 6

void processMyVariant(const string& fileName) {
    
}

string wideToUtf8(const wstring& text) {
    if (text.empty()) {
        return "";
    }

    // Сначала узнаем, сколько байт нужно для UTF-8 строки.
    int size = WideCharToMultiByte(
        CP_UTF8,
        0,
        text.c_str(),
        static_cast<int>(text.size()),
        nullptr,
        0,
        nullptr,
        nullptr
    );

    // Создаем строку нужного размера и заполняем ее данными в UTF-8.
    string result(size, '\0');
    WideCharToMultiByte(
        CP_UTF8,
        0,
        text.c_str(),
        static_cast<int>(text.size()),
        result.data(),
        size,
        nullptr,
        nullptr
    );

    return result;
}

string readConsoleLineUtf8() {
    // Получаем "ручку" стандартного ввода консоли Windows.
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    if (hInput == INVALID_HANDLE_VALUE) {
        // Запасной вариант: если это не консоль, читаем как обычную строку.
        string fallback;
        getline(cin >> ws, fallback);
        return fallback;
    }

    // Будем читать строку как wide-символы, чтобы русский текст не ломался.
    wstring wideLine;
    wchar_t buffer[256];
    DWORD charsRead = 0;

    while (true) {
        // ReadConsoleW читает именно Unicode-символы из консоли.
        if (!ReadConsoleW(hInput, buffer, 255, &charsRead, nullptr) || charsRead == 0) {
            break;
        }

        buffer[charsRead] = L'\0';
        wideLine += buffer;

        if (wideLine.find(L'\n') != wstring::npos) {
            break;
        }
    }

    // Убираем символы конца строки, которые приходят после Enter.
    while (!wideLine.empty() && (wideLine.back() == L'\r' || wideLine.back() == L'\n')) {
        wideLine.pop_back();
    }

    // Переводим строку в UTF-8, чтобы дальше работать с обычным string.
    return wideToUtf8(wideLine);
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    cout << "--- Задание 1 ---" << endl;
    cout << "Введите любую строку с лишними пробелами и ошибками пунктуации:" << endl;

    string userInput = readConsoleLineUtf8();

    cout << "\nПолучена строка: " << userInput << endl;
    cout << "Количество символов в строке: " << userInput.length() << endl;

    cout << "\n--- Задание 2 ---" << endl;
    string formattedInput = formatString(userInput);
    cout << "Отформатированная строка: " << formattedInput << endl;
    cout << "Количество символов после форматирования: " << formattedInput.length() << endl;

    cout << "\n--- Задание 3 ---" << endl;
    ofstream createInput("Input.txt");
    if (createInput.is_open()) {
        createInput << "Это тестовая   строка ,в которой много  ошибок." << endl;
        createInput << "Пробелы перед запятой  ,   и нет пробела после точки.Вот так!" << endl;
        createInput << "   Слово волк , невольно  заставляет вспомнить символы  ." << endl;
        createInput.close();
        cout << "Файл Input.txt успешно создан с тестовыми данными." << endl;
    }

    cout << "\n--- Задание 4 ---" << endl;
    processFile("Input.txt", "Output.txt");

    cout << "\n--- Задание 5 ---" << endl;
    cout << "Введите фрагмент слова для поиска в файле (например, 'вол'): ";

    // Читаем поисковый фрагмент тем же способом, чтобы русский ввод
    string searchWord = readConsoleLineUtf8();

    if (searchWord.empty()) {
        cout << "Ошибка: введена пустая строка для поиска." << endl;
    } else {
        findWordsInFile("Output.txt", searchWord);
    }

    cout << "\n--- Задание 6 ---" << endl;
    processMyVariant("Output.txt");

    cout << "\nПрограмма завершена." << endl;
    return 0;
}
