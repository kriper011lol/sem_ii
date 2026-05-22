#include "core.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <unordered_set>
#include <QString>
#include <xlsxdocument.h>

#define MINIDOCX_NAMESPACE md
#include "minidocx/minidocx.hpp"

const double PI = std::acos(-1.0);
const double EPS = 1e-12;

// --- МАТЕМАТИЧЕСКИЕ ФУНКЦИИ ---

double complexModulus(const Complex& value) noexcept {
    return std::sqrt(value.real * value.real + value.imag * value.imag);
}

double argumentDegrees(const Complex& value) noexcept {
    return std::atan2(value.imag, value.real) * 180.0 / PI;
}

Complex addComplex(const Complex& left, const Complex& right) noexcept {
    return {left.real + right.real, left.imag + right.imag};
}

Complex subtractComplex(const Complex& left, const Complex& right) noexcept {
    return {left.real - right.real, left.imag - right.imag};
}

Complex multiplyComplex(const Complex& left, const Complex& right) noexcept {
    return {
        left.real * right.real - left.imag * right.imag,
        left.real * right.imag + left.imag * right.real
    };
}

std::optional<Complex> divideComplex(const Complex& left, const Complex& right) noexcept {
    double den = right.real * right.real + right.imag * right.imag;
    if (den < EPS) return std::nullopt;
    return Complex{
        (left.real * right.real + left.imag * right.imag) / den,
        (left.imag * right.real - left.real * right.imag) / den
    };
}

Complex conjugateComplex(const Complex& value) noexcept {
    return {value.real, -value.imag};
}

Complex fromPolar(double radius, double angleDegrees) noexcept {
    double angle = angleDegrees * PI / 180.0;
    return {radius * std::cos(angle), radius * std::sin(angle)};
}

// --- ФОРМАТИРОВАНИЕ ---

std::string formatDouble(double value, int precision) {
    if (std::abs(value) < EPS) value = 0.0;
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    std::string text = out.str();
    while (text.size() > 1 && text.back() == '0') text.pop_back();
    if (!text.empty() && text.back() == '.') text.pop_back();
    if (text == "-0") return "0";
    return text;
}

std::string complexToString(const Complex& value) {
    std::ostringstream out;
    out << formatDouble(value.real);
    out << (value.imag >= -EPS ? " + " : " - ");
    out << formatDouble(std::abs(value.imag)) << "i";
    return out.str();
}

std::string polarFormText(const Complex& value) {
    double r = complexModulus(value);
    double phi = argumentDegrees(value);
    return formatDouble(r) + " * (cos(" + formatDouble(phi) + "°) + i*sin(" + formatDouble(phi) + "°))";
}

// --- ПАРСЕР ВЫРАЖЕНИЙ ---

ExpressionParser::ExpressionParser(std::string_view source) noexcept : text(source) {}

void ExpressionParser::skipSpaces() noexcept {
    while (position < text.size() && std::isspace(static_cast<unsigned char>(text[position]))) position++;
}

void ExpressionParser::setError(std::string_view message) {
    if (error.empty()) error = message;
}

bool ExpressionParser::match(char expected) noexcept {
    skipSpaces();
    if (position < text.size() && text[position] == expected) {
        position++;
        return true;
    }
    return false;
}

bool ExpressionParser::matchAny(std::string_view symbols, char& found) noexcept {
    skipSpaces();
    if (position < text.size() && symbols.find(text[position]) != std::string_view::npos) {
        found = text[position++];
        return true;
    }
    return false;
}

ExpressionParser::ParseResult ExpressionParser::parse() noexcept {
    position = 0;
    error.clear();
    Complex val = parseExpression();
    skipSpaces();
    if (error.empty() && position < text.size()) {
        setError("Не удалось разобрать часть выражения");
    }
    if (!error.empty()) return {false, {}, error};
    return {true, val, ""};
}

Complex ExpressionParser::parseExpression() noexcept {
    Complex left = parseTerm();
    while (error.empty()) {
        char op = 0;
        if (!matchAny("+-", op)) break;
        Complex right = parseTerm();
        if (op == '+') left = addComplex(left, right);
        else left = subtractComplex(left, right);
    }
    return left;
}

Complex ExpressionParser::parseTerm() noexcept {
    Complex left = parseUnary();
    while (error.empty()) {
        char op = 0;
        if (!matchAny("*/", op)) break;
        Complex right = parseUnary();
        if (op == '*') left = multiplyComplex(left, right);
        else {
            auto res = divideComplex(left, right);
            if (!res) { setError("Деление на ноль"); return left; }
            left = *res;
        }
    }
    return left;
}

Complex ExpressionParser::parseUnary() noexcept {
    skipSpaces();
    if (match('+')) return parseUnary();
    if (match('-')) {
        Complex v = parseUnary();
        return {-v.real, -v.imag};
    }
    return parsePrimary();
}

Complex ExpressionParser::parsePrimary() noexcept {
    skipSpaces();
    if (position >= text.size()) { setError("Неожиданный конец"); return {}; }
    if (match('(')) {
        Complex v = parseExpression();
        if (!match(')'))
        {
            setError("Отсутствует закрывающая скобка ')'");
        }
        return v;
    }
    char c = text[position];
    if (c == 'i' || c == 'I') { position++; return {0.0, 1.0}; }
    if (std::isalpha(static_cast<unsigned char>(c)) || (unsigned char)c > 127) return parseFunction();
    return parseNumber();
}
Complex ExpressionParser::parseFunction() noexcept {
    std::string name(readIdentifier());
    if (name.empty()) return {};

    std::string originalName = name;

    // убрать цифры и лишние символы в конце
    while (!name.empty() && 
           !std::isalpha(static_cast<unsigned char>(name.back())) && 
           static_cast<unsigned char>(name.back()) <= 127) {
        name.pop_back();
    }

    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    const std::unordered_set<std::string> functions = {
        "conj", "сопр", "mod", "abs", "модуль", "arg", "арг", "polar", "полярная"
    };

    bool knownFunction = (functions.find(name) != functions.end());

    if (!knownFunction) {
        setError("Неизвестная функция");
        return {};
    }

    // Проверка открывающей скобки
    if (!match('(')) {
        setError("Отсутствует открывающая скобка '('");
        return {};
    }

    if (name == "polar" || name == "полярная") {

        Complex r = parseExpression();
        char sep = 0;
        if (!matchAny(";,", sep)) {
            setError("Нужен разделитель ';'");
            return {};
        }
        Complex phi = parseExpression();
        if (!match(')')) {
            setError("Отсутствует закрывающая скобка ')'");
        }
        return fromPolar(r.real, phi.real);
    }
    
    Complex v = parseExpression();
    if (!match(')')) {
        setError("Отсутствует закрывающая скобка ')'");
    }
    if (name == "conj" || name == "сопр") return conjugateComplex(v);
    if (name == "mod" || name == "abs" || name == "модуль") return {complexModulus(v), 0.0};
    if (name == "arg" || name == "арг") return {argumentDegrees(v), 0.0};
    
    return {};
}

Complex ExpressionParser::parseNumber() noexcept {
    skipSpaces();
    size_t start = position;
    bool hasDot = false;
    while (position < text.size()) {
        char c = text[position];
        if (std::isdigit(static_cast<unsigned char>(c))) position++;
        else if ((c == '.' || c == ',') && !hasDot) { hasDot = true; position++; }
        else break;
    }
    if (start == position) { setError("Нужно число"); return {}; }
    std::string s(text.substr(start, position - start));
    std::replace(s.begin(), s.end(), ',', '.');
    double val = std::stod(s);
    skipSpaces();
    if (position < text.size() && (text[position] == 'i' || text[position] == 'I')) {
        position++;
        return {0.0, val};
    }
    return {val, 0.0};
}

std::string_view ExpressionParser::readIdentifier() noexcept {
    skipSpaces();
    size_t start = position;
    while (position < text.size() && (std::isalnum(static_cast<unsigned char>(text[position])) || (unsigned char)text[position] > 127)) position++;
    return text.substr(start, position - start);
}

// --- УТИЛИТЫ ---

std::string nowString() {
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now);
    char buffer[32]{};
    std::strftime(buffer, 32, "%d.%m.%Y %H:%M:%S", &localTime);
    return buffer;
}

// --- ЭКСПОРТ ---

bool saveCSV(const std::vector<HistoryItem>& history, const std::string& filename) {
    std::ofstream f(filename);
    if (!f) return false;
    // Используем UTF-8 BOM для корректного открытия в Excel
    f << "\xEF\xBB\xBF";
    f << "ID;Дата и время;Выражение;Результат;Статус\n";
    for (const auto& item : history) {
        f << item.id << ";" << item.dateTime << ";" << item.expression << ";" << complexToString(item.result) << ";" << item.status << "\n";
    }
    return true;
}

bool saveJSON(const std::vector<HistoryItem>& history, const std::string& filename) {
    std::ofstream f(filename);
    if (!f) return false;
    f << "[\n";
    for (size_t i = 0; i < history.size(); ++i) {
        const auto& item = history[i];
        f << "  {\n";
        f << "    \"id\": " << item.id << ",\n";
        f << "    \"timestamp\": \"" << item.dateTime << "\",\n";
        f << "    \"query\": \"" << item.expression << "\",\n";
        f << "    \"result\": \"" << complexToString(item.result) << "\",\n";
        f << "    \"status\": \"" << item.status << "\"\n";
        f << "  }" << (i == history.size() - 1 ? "" : ",") << "\n";
    }
    f << "]";
    return true;
}

bool saveXLSX(const std::vector<HistoryItem>& history, const std::string& filename) {
    QXlsx::Document xlsx;

    xlsx.write(1, 1, "ID");
    xlsx.write(1, 2, "Дата");
    xlsx.write(1, 3, "Выражение");
    xlsx.write(1, 4, "Результат");
    xlsx.write(1, 5, "Статус");

    int row = 2;
    for (const auto& item : history) {
        xlsx.write(row, 1, item.id);
        xlsx.write(row, 2, QString::fromStdString(item.dateTime));
        xlsx.write(row, 3, QString::fromStdString(item.expression));
        xlsx.write(row, 4, QString::fromStdString(complexToString(item.result)));
        xlsx.write(row, 5, QString::fromStdString(item.status));
        row++;
    }

    return xlsx.saveAs(QString::fromStdString(filename));
}

bool saveTXT(const std::vector<HistoryItem>& history, const std::string& filename) {
    std::ofstream f(filename);
    if (!f) return false;
    f << "\xEF\xBB\xBF"; // UTF-8 BOM
    f << "ОТЧЕТ О ВЫЧИСЛЕНИЯХ\n";
    f << "Программа: Калькулятор комплексных чисел\n";
    f << "Студент: Нгуен Чонг Хай Хоанг (ДИНРБ-11/2)\n";
    f << "========================================================\n\n";
    
    for (const auto& item : history) {
        f << "Запись №" << item.id << "\n";
        f << "Время:     " << item.dateTime << "\n";
        f << "Выражение: " << item.expression << "\n";
        f << "Результат: " << complexToString(item.result) << "\n";
        f << "--------------------------------------------------------\n";
    }
    f << "\nВсего записей: " << history.size() << "\n";
    f << "Конец отчета.\n";
    return true;
}

bool saveDOCX(const std::vector<HistoryItem>& history, const std::string& filename) {
    md::Document doc;
    auto section = doc.addSection();

    auto p1 = section->addParagraph();
    p1->addRichText("Журнал вычислений");
    
    section->addParagraph(); // Empty line

    for (const auto& item : history) {
        auto p_id = section->addParagraph();
        p_id->addRichText("Запись #" + std::to_string(item.id));
        
        auto p_date = section->addParagraph();
        p_date->addRichText("Дата: " + item.dateTime);
        
        auto p_expr = section->addParagraph();
        p_expr->addRichText("Выражение: " + item.expression);
        
        auto p_res = section->addParagraph();
        p_res->addRichText("Результат: " + complexToString(item.result));
        
        section->addParagraph(); // Empty line between items
    }

    doc.saveAs(filename);
    return true;
}

const char* helpTheoryText() noexcept {
    return R"HELP_TEXT(
СПРАВКА ПО КОМПЛЕКСНЫМ ЧИСЛАМ
==============================

1. ЧТО ТАКОЕ КОМПЛЕКСНОЕ ЧИСЛО
------------------------------
Комплексное число — это расширение понятия вещественного числа. 
Оно состоит из действительной части (Re) и мнимой части (Im).
Такие числа позволяют решать уравнения, не имеющие вещественных
корней (например, x² = -1).

2. АЛГЕБРАИЧЕСКАЯ ФОРМА
-----------------------
Стандартная запись: z = a + bi

Где:
    a — действительная часть Re(z)
    b — мнимая часть Im(z)
    i — мнимая единица, для которой выполняется i² = -1

Пример: z = 3 + 4i
Здесь: Re(z) = 3, Im(z) = 4

3. ОСНОВНЫЕ ОПЕРАЦИИ
--------------------
Пусть заданы: z1 = a + bi  и  z2 = c + di

• Сложение:
  z1 + z2 = (a + c) + (b + d)i
  (Складываются отдельно действительные и мнимые части)

• Вычитание:
  z1 - z2 = (a - c) + (b - d)i

• Умножение:
  z1 * z2 = (ac - bd) + (ad + bc)i
  (Раскрытие скобок с учетом i² = -1)

• Деление:
  z1 / z2 = ((ac + bd) / (c² + d²)) + ((bc - ad) / (c² + d²))i
  (Умножение числителя и знаменателя на сопряженное число)

4. СОПРЯЖЕННОЕ ЧИСЛО, МОДУЛЬ И АРГУМЕНТ
---------------------------------------
• Сопряженное число:
  Для z = a + bi сопряженным является conj(z) = a - bi.
  Оно симметрично относительно действительной оси.

• Модуль (абсолютная величина):
  |z| = sqrt(a² + b²)
  Это расстояние от начала координат до числа на плоскости.

• Аргумент (угол):
  arg(z) — угол между положительным направлением оси Re и 
  радиус-вектором числа. Вычисляется как atan2(b, a).

5. ТРИГОНОМЕТРИЧЕСКАЯ ФОРМА
---------------------------
Любое ненулевое комплексное число можно представить как:
    z = |z| * (cos(φ) + i * sin(φ))

Где:
    |z| — модуль числа
    φ — аргумент (угол)

Эта форма удобна для умножения, деления и возведения в степень.

6. ПРИМЕРЫ ВВОДА В ПРОГРАММЕ
----------------------------
Программа понимает различные форматы записи:

• Алгебраическая форма:
  - "2 + 3i"
  - "5 - i" (единичный коэффициент мнимой части можно опускать)
  - "-4" (чисто действительное число)
  - "3i" (чисто мнимое число)

• Полярная форма (через ';'):
  - "5; 45" — модуль 5, угол 45 градусов
  - "1; 180" — соответствует числу -1

• Специальные функции:
  - "сопр(2+3i)" — получение сопряженного числа (2-3i)
  - "модуль(3-4i)" — вычисление модуля (5)
  - "арг(1+i)" — вычисление аргумента в градусах (45)

7. РАЗДЕЛИТЕЛЬ ';' (ПОЛЯРНЫЕ КООРДИНАТЫ)
----------------------------------------
Использование точки с запятой (например, "r; phi") позволяет
вводить числа напрямую через их модуль и угол в градусах. 
Программа автоматически переведет их в алгебраическую форму.
)HELP_TEXT";
}
