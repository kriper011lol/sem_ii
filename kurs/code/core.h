#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <optional>

// Комплексное число: используем простые double, RVO обеспечит эффективность возврата
struct Complex {
    double real = 0.0;
    double imag = 0.0;
};

// Запись истории: инкапсулируем данные для логгирования
struct HistoryItem {
    int id = 0;
    std::string dateTime;
    std::string expression;
    Complex result;
    std::string status;
};

// Математические функции: передача по const& предотвращает копирование
double complexModulus(const Complex& value) noexcept;
double argumentDegrees(const Complex& value) noexcept;

Complex addComplex(const Complex& left, const Complex& right) noexcept;
Complex subtractComplex(const Complex& left, const Complex& right) noexcept;
Complex multiplyComplex(const Complex& left, const Complex& right) noexcept;
std::optional<Complex> divideComplex(const Complex& left, const Complex& right) noexcept;
Complex conjugateComplex(const Complex& value) noexcept;
Complex fromPolar(double radius, double angleDegrees) noexcept;

// Форматирование: возвращаем std::string (NRVO)
std::string complexToString(const Complex& value);
std::string polarFormText(const Complex& value);
std::string formatDouble(double value, int precision = 8);

/**
 * @brief Оптимизированный парсер выражений.
 * Использует std::string_view для исключения аллокаций при разборе строки.
 */
class ExpressionParser {
public:
    explicit ExpressionParser(std::string_view source) noexcept;
    
    // Возвращает пару {результат, сообщение об ошибке}
    struct ParseResult {
        bool success;
        Complex value;
        std::string message;
    };
    
    ParseResult parse() noexcept;

private:
    std::string_view text;
    size_t position = 0;
    std::string error;

    void skipSpaces() noexcept;
    void setError(std::string_view message);
    bool match(char expected) noexcept;
    bool matchAny(std::string_view symbols, char& found) noexcept;

    Complex parseExpression() noexcept;
    Complex parseTerm() noexcept;
    Complex parseUnary() noexcept;
    Complex parsePrimary() noexcept;
    Complex parseFunction() noexcept;
    Complex parseNumber() noexcept;
    std::string_view readIdentifier() noexcept;
};

// Утилиты времени
std::string nowString();

// Функции экспорта: использование const& для тяжелых векторов
bool saveCSV(const std::vector<HistoryItem>& history, const std::string& filename);
bool saveJSON(const std::vector<HistoryItem>& history, const std::string& filename);
bool saveXLSX(const std::vector<HistoryItem>& history, const std::string& filename);
bool saveTXT(const std::vector<HistoryItem>& history, const std::string& filename);
bool saveDOCX(const std::vector<HistoryItem>& history, const std::string& filename);

// Справочная информация
const char* helpTheoryText() noexcept;
