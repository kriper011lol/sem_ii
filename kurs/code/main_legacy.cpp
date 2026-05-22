/*
 * Университет: Астраханский Государственный Технический Университет
 * Группа: ДИНРб-11/2
 * ФИО: Нгуен Чонг Хай Хоанг
 *
 * Краткая карта программы:
 * 1. Сначала объявляются структуры данных, идентификаторы кнопок и глобальные переменные окна.
 * 2. Затем идут функции для работы с WinAPI/GDI: кисти, перья, шрифты, цвета и темы.
 * 3. После этого описана математика комплексных чисел и парсер выражений.
 * 4. Далее находятся функции сохранения истории в CSV, JSON и XLSX.
 * 5. В конце файла создаётся интерфейс, рисуется дизайн и обрабатываются события окна.
 */

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <commdlg.h>

using namespace std;

// Число pi нужно для перевода градусов в радианы и обратно.
const double PI = acos(-1.0);
// Маленькая погрешность для сравнения double, потому что вещественные числа неточны.
const double EPS = 1e-12;
// Базовый размер макета. Все координаты интерфейса ниже заданы относительно этой области.
const int DESIGN_WIDTH = 1200;
const int DESIGN_HEIGHT = 870;
// Минимальный размер окна: ниже интерфейс станет слишком мелким для нормальной работы.
const int MIN_WINDOW_WIDTH = 840;
const int MIN_WINDOW_HEIGHT = 640;

// Комплексное число хранится как две части: действительная и мнимая.
struct Complex {
    // Действительная часть: например, у 2 + 3i это 2.
    double real = 0.0;
    // Мнимая часть: например, у 2 + 3i это 3.
    double imag = 0.0;
};

// Одна строка истории вычислений, которая потом рисуется в интерфейсе и экспортируется.
struct HistoryItem {
    // Порядковый номер записи в истории.
    int id = 0;
    // Дата и время вычисления.
    wstring dateTime;
    // Исходное выражение, которое ввёл пользователь.
    wstring expression;
    // Посчитанный результат.
    Complex result;
    // Текстовый статус операции, сейчас используется "Выполнено".
    wstring status;
};

// Числовые ID элементов управления. По этим ID обработчик понимает, какую кнопку нажали.
enum ControlId {
    // Поле ввода выражения.
    ID_QUERY = 1001,
    // Основные команды.
    ID_CALCULATE,
    ID_CLEAR,
    // Кнопки с готовыми примерами.
    ID_SAMPLE_SIMPLE,
    ID_SAMPLE_DIVISION,
    ID_SAMPLE_CONJUGATE,
    ID_SAMPLE_POLAR,
    // Кнопки экспорта.
    ID_SAVE_CSV,
    ID_SAVE_JSON,
    ID_SAVE_XLSX,
    ID_SAVE_TXT,
    ID_SAVE_DOCX,
    ID_SAVE_ALL,
    // Переключатель ночной/светлой темы.
    ID_TOGGLE_THEME,
    // Окно с теорией и инструкциями.
    ID_HELP
};

// Дескриптор экземпляра программы нужен WinAPI при создании окон.
HINSTANCE gInstance = nullptr;
// Дескриптор главного окна приложения.
HWND gMainWindow = nullptr;
// Дескриптор окна справки. Если он уже открыт, повторно не создаём.
HWND gHelpWindow = nullptr;
// Многострочное поле внутри окна справки.
HWND gHelpEdit = nullptr;

// Вызывает стандартное диалоговое окно Windows для выбора пути сохранения файла.
wstring promptSaveFileName(const wstring& filter, const wstring& defaultExt) {
    wchar_t fileName[MAX_PATH] = L"result";
    
    // Динамически загружаем функцию из comdlg32.dll, чтобы избежать ошибок линковки (ld error).
    static HMODULE comdlg = LoadLibraryW(L"comdlg32.dll");
    if (!comdlg) return L"";

    typedef BOOL (WINAPI *GetSaveFileNameFunc)(LPOPENFILENAMEW);
    static GetSaveFileNameFunc pGetSaveFileName = (GetSaveFileNameFunc)GetProcAddress(comdlg, "GetSaveFileNameW");
    if (!pGetSaveFileName) return L"";

    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = gMainWindow;
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = defaultExt.c_str();

    if (pGetSaveFileName(&ofn)) {
        return fileName;
    }
    return L"";
}
// Дескриптор поля ввода выражения.
HWND gQueryEdit = nullptr;
// Дескриптор кнопки переключения темы.
HWND gThemeButton = nullptr;
// Старая оконная процедура поля ввода: нужна, чтобы после своей обработки вернуть управление EDIT.
WNDPROC gQueryEditProcedure = nullptr;
// ID выбранного примера, чтобы выбранная кнопка подсвечивалась.
int gSelectedSampleId = 0;
// ID кнопки под курсором, чтобы рисовать заметное наведение.
int gHoveredButtonId = 0;
// Текущие масштабы интерфейса по горизонтали и вертикали.
double gLayoutScaleX = 1.0;
double gLayoutScaleY = 1.0;
// Флаг компактного режима для узких окон.
bool gIsCompactMode = false;
// Эффективная высота макета.
int gEffectiveDesignHeight = 870;
// Смещения для центрирования.
int gLayoutOffsetX = 0;
int gLayoutOffsetY = 0;
// Запоминаем реальные размеры шрифтов контролов, чтобы не пересоздавать их лишний раз.
int gInputFontHeight = 22;
int gButtonFontHeight = 16;

bool gShowTooltip = false;
wstring gTooltipText = L"";
RECT gTooltipRect{0, 0, 0, 0}; // Координаты отрисовки плашки

// Текстовые блоки результата. Они не являются отдельными Label-контролами: мы рисуем их вручную.
wstring gInputText = L"Введите выражение и нажмите Enter.";
wstring gResultText = L"";
wstring gPolarText = L"";
wstring gDetailsText = L"";

// Шрифты интерфейса. Создаются один раз при старте и удаляются при выходе.
HFONT gTitleFont = nullptr;
HFONT gSubtitleFont = nullptr;
HFONT gTextFont = nullptr;
HFONT gInputFont = nullptr;
HFONT gCardTitleFont = nullptr;
HFONT gResultFont = nullptr;
HFONT gButtonFont = nullptr;
HFONT gMetaFont = nullptr;

// Кисти для заливки фона, карточек, поля ввода и акцентных элементов.
HBRUSH gPageBrush = nullptr;
HBRUSH gWhiteBrush = nullptr;
HBRUSH gEditBrush = nullptr;
HBRUSH gAccentBrush = nullptr;

// Вектор хранит всю историю текущей сессии.
vector<HistoryItem> gHistory;

// Данные студента вынесены в константы, чтобы они были в одном месте.
const wchar_t* APP_TITLE = L"Калькулятор комплексных чисел";
const wchar_t* APP_UNIVERSITY = L"Астраханский Государственный Технический Университет";
const wchar_t* APP_GROUP = L"ДИНРб-11/2";
const wchar_t* APP_STUDENT = L"Нгуен Чонг Хай Хоанг";

// Текст справки.
const wchar_t* helpTheoryText() {
    return L"СПРАВКА ПО КОМПЛЕКСНЫМ ЧИСЕЛ\r\n"
          L"=======================================\r\n\r\n"
          L"1. НАЗНАЧЕНИЕ ПРОГРАММЫ\r\n"
          L"----------------------------------------------------------------------\r\n"
          L"Калькулятор выполняет вычисления с комплексными\r\n"
          L"числами, выводит результат в алгебраической и\r\n"
          L"тригонометрической формах, ведет журнал истории\r\n"
          L"и экспортирует данные в файлы разных форматов.\r\n\r\n"
          L"2. ПОНЯТИЕ КОМПЛЕКСНОГО ЧИСЛА\r\n"
          L"----------------------------------------------------------------------\r\n"
          L"Комплексное число записывается в декартовой форме:\r\n"
          L"    z = a + bi\r\n\r\n"
          L"Где:\r\n"
          L"    a - действительная часть Re(z)\r\n"
          L"    b - мнимая часть Im(z)\r\n"
          L"    i - мнимая единица (i^2 = -1)\r\n\r\n"
          L"Пример ввода: 2 + 3i\r\n"
          L"Здесь: Re(z) = 2, Im(z) = 3\r\n\r\n"
          L"3. АРИФМЕТИЧЕСКИЕ ОПЕРАЦИИ\r\n"
          L"----------------------------------------------------------------------\r\n"
          L"Пусть заданы: z1 = a + bi  и  z2 = c + di\r\n\r\n"
          L"• Сложение:\r\n"
          L"  z1 + z2 = (a + c) + (b + d)i\r\n\r\n"
          L"• Вычитание:\r\n"
          L"  z1 - z2 = (a - c) + (b - d)i\r\n\r\n"
          L"• Умножение:\r\n"
          L"  z1 * z2 = (ac - bd) + (ad + bc)i\r\n\r\n"
          L"• Деление:\r\n"
          L"  z1 / z2 = ((a*c + b*d) + (b*c - a*d)i) / (c^2 + d^2)\r\n"
          L"  Условие: знаменатель (c^2 + d^2) не равен 0.\r\n\r\n"
          L"4. СОПРЯЖЕННОЕ ЧИСЛО\r\n"
          L"----------------------------------------------------------------------\r\n"
          L"Для числа z = a + bi сопряженным является:\r\n"
          L"    conj(z) = a - bi\r\n\r\n"
          L"Ввод в программе: сопр(2+3i)\r\n"
          L"Результат: 2 - 3i\r\n\r\n"
          L"5. МОДУЛЬ ЧИСЛА\r\n"
          L"----------------------------------------------------------------------\r\n"
          L"Модуль |z| — это расстояние от начала координат до\r\n"
          L"точки на комплексной плоскости:\r\n"
          L"    |z| = sqrt(a^2 + b^2)\r\n\r\n"
          L"Ввод в программе: модуль(3+4i)\r\n"
          L"Результат: 5\r\n\r\n"
          L"6. АРГУМЕНТ ЧИСЛА\r\n"
          L"----------------------------------------------------------------------\r\n"
          L"Аргумент arg(z) — угол между положительным\r\n"
          L"направлением действительной оси и вектором точки:\r\n"
          L"    arg(z) = atan2(b, a) (выводится в градусах)\r\n\r\n"
          L"Ввод в программе: арг(1+i)\r\n"
          L"Результат: 45\r\n\r\n"
          L"7. ТРИГОНОМЕТРИЧЕСКАЯ ФОРМА\r\n"
          L"----------------------------------------------------------------------\r\n"
          L"Запись через модуль r = |z| и аргумент phi = arg(z):\r\n"
          L"    z = r * (cos(phi) + i * sin(phi))\r\n\r\n"
          L"Ввод в программе: полярная(5;45)\r\n"
          L"Важно: используется точка с запятой (;), так как\r\n"
          L"запятая может быть десятичным разделителем.";
}

// Короткая подсказка для области ввода при наведении на кнопку-пример.
wstring hoverInstructionText(int id) {
    switch (id) {
...
void drawLayout(HDC dc) {
    RECT deviceClient{};
    GetClientRect(gMainWindow, &deviceClient);
    FillRect(dc, &deviceClient, gPageBrush);

    HBRUSH headerBrush = makeSolidBrush(COLOR_HEADER);
    RECT headerRect{0, 0, deviceClient.right, 118};
    FillRect(dc, &headerRect, headerBrush);
    deleteGdiObject(headerBrush);

    auto drawHeaderText = [&](const wstring& t, HFONT f, COLORREF c, RECT r, UINT fmt) {
        drawText(dc, t, f, c, r, fmt);
    };

    drawHeaderText(APP_TITLE, gTitleFont, COLOR_HEADER_TEXT, {64, 24, 740, 64}, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
    drawHeaderText(L"Алгебраическая форма, тригонометрическая форма и журнал вычислений.", gSubtitleFont, COLOR_HEADER_MUTED, {66, 68, 740, 92}, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
    
    drawHeaderText(APP_UNIVERSITY, gMetaFont, COLOR_HEADER_MUTED, {740, 24, 1136, 44}, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
    wstring studentInfo = wstring(L"Группа: ") + APP_GROUP + L"  •  ФИО: " + APP_STUDENT;
    drawHeaderText(studentInfo, gMetaFont, COLOR_HEADER_MUTED, {740, 44, 1136, 64}, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
}
...