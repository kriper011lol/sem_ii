#include "mainwindow.h"

#include <QApplication>
#include <QDialog>
#include <QEvent>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QLayout>
#include <QStyle>
#include <QStyleFactory>
#include <QToolTip>
#include <QCursor>
#include <QPlainTextEdit>

namespace {
// Простой FlowLayout для переноса кнопок на новую строку
class FlowLayout : public QLayout {
public:
    explicit FlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1)
        : QLayout(parent), m_hSpace(hSpacing), m_vSpacing(vSpacing) {
        setContentsMargins(margin, margin, margin, margin);
    }
    explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1)
        : m_hSpace(hSpacing), m_vSpacing(vSpacing) {
        setContentsMargins(margin, margin, margin, margin);
    }
    ~FlowLayout() {
        QLayoutItem *item;
        while ((item = takeAt(0))) delete item;
    }
    void addItem(QLayoutItem *item) override { m_itemList.append(item); }
    int horizontalSpacing() const { return m_hSpace >= 0 ? m_hSpace : spacing(); }
    int verticalSpacing() const { return m_vSpacing >= 0 ? m_vSpacing : spacing(); }
    Qt::Orientations expandingDirections() const override { return { }; }
    bool hasHeightForWidth() const override { return true; }
    int heightForWidth(int width) const override { return doLayout(QRect(0, 0, width, 0), true); }
    int count() const override { return m_itemList.size(); }
    QLayoutItem *itemAt(int index) const override { return m_itemList.value(index); }
    QSize minimumSize() const override {
        QSize size;
        for (auto* item : m_itemList) size = size.expandedTo(item->minimumSize());
        const QMargins margins = contentsMargins();
        size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
        return size;
    }
    void setGeometry(const QRect &rect) override {
        QLayout::setGeometry(rect);
        doLayout(rect, false);
    }
    QSize sizeHint() const override { return minimumSize(); }
    QLayoutItem *takeAt(int index) override {
        if (index >= 0 && index < m_itemList.size()) return m_itemList.takeAt(index);
        return nullptr;
    }
private:
    int doLayout(const QRect &rect, bool testOnly) const {
        int left, top, right, bottom;
        getContentsMargins(&left, &top, &right, &bottom);
        QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
        int x = effectiveRect.x();
        int y = effectiveRect.y();
        int lineHeight = 0;

        for (auto* item : m_itemList) {
            QWidget *wid = item->widget();
            int spaceX = horizontalSpacing();
            if (spaceX == -1) spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
            int spaceY = verticalSpacing();
            if (spaceY == -1) spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
            
            int nextX = x + item->sizeHint().width() + spaceX;
            if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
                x = effectiveRect.x();
                y = y + lineHeight + spaceY;
                nextX = x + item->sizeHint().width() + spaceX;
                lineHeight = 0;
            }
            if (!testOnly) item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
            x = nextX;
            lineHeight = qMax(lineHeight, item->sizeHint().height());
        }
        return y + lineHeight - rect.y() + bottom;
    }
    QList<QLayoutItem *> m_itemList;
    int m_hSpace, m_vSpacing;
};

constexpr const char* University = "Астраханский Государственный Технический Университет";
constexpr const char* Group = "ДИНРБ-11/2";
constexpr const char* Student = "Нгуен Чонг Хай Хоанг";
constexpr int DESIGN_WIDTH = 1200;
constexpr int DESIGN_HEIGHT = 920;

QString toUiText(const std::string& text) {
    return QString::fromUtf8(text.c_str());
}

std::string toCoreText(const QString& text) {
    const QByteArray utf8 = text.toUtf8();
    return std::string(utf8.constData(), static_cast<size_t>(utf8.size()));
}
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Курсовая работа - Калькулятор комплексных чисел - Нгуен Чонг Хай Хоанг");
    setMinimumSize(840, 640);
    resize(1200, DESIGN_HEIGHT);
    setupUi();
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    central->setObjectName("centralWidget");
    setCentralWidget(central);

    m_rootLayout = new QVBoxLayout(central);
    m_rootLayout->setContentsMargins(0, 0, 0, 0);
    m_rootLayout->setSpacing(0);

    // Хедер
    headerFrame = new QFrame();
    headerFrame->setObjectName("headerFrame");
    m_rootLayout->addWidget(headerFrame);

    m_headerLayout = new QHBoxLayout(headerFrame);
    
    auto* titleBlock = new QVBoxLayout();
    titleBlock->setSpacing(0);
    headerTitle = new QLabel("Калькулятор комплексных чисел");
    headerTitle->setObjectName("headerTitle");
    headerTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    headerSubtitle = new QLabel("Алгебраическая форма, тригонометрическая форма и журнал вычислений.");
    headerSubtitle->setObjectName("headerSubtitle");
    titleBlock->addWidget(headerTitle);
    titleBlock->addWidget(headerSubtitle);
    titleBlock->addStretch();

    auto* rightBlock = new QVBoxLayout();
    rightBlock->setSpacing(4);
    univLabel = new QLabel(University);
    univLabel->setObjectName("studentInfo");
    univLabel->setAlignment(Qt::AlignRight);
    univLabel->setWordWrap(false);

    infoLabel = new QLabel(QString("Группа: %1  •  ФИО: %2").arg(Group, Student));
    infoLabel->setObjectName("studentInfo");
    infoLabel->setAlignment(Qt::AlignRight);
    infoLabel->setWordWrap(false);
    
    auto* headerButtons = new QHBoxLayout();
    headerButtons->setSpacing(10);
    headerButtons->addStretch();
    helpButton = createCommandButton("Справка", "headerButton");
    m_themeButton = createCommandButton("Светлая тема", "headerButton");
    headerButtons->addWidget(helpButton);
    headerButtons->addWidget(m_themeButton);

    rightBlock->addWidget(univLabel);
    rightBlock->addWidget(infoLabel);
    rightBlock->addLayout(headerButtons);
    rightBlock->addStretch();

    m_headerLayout->addLayout(titleBlock, 1);
    m_headerLayout->addLayout(rightBlock);

    // Акцентная полоса
    accentBar = new QWidget();
    accentBar->setObjectName("accentBar");
    auto* accentLayout = new QHBoxLayout(accentBar);
    accentLayout->setContentsMargins(0, 0, 0, 0);
    accentLayout->setSpacing(0);
    warmLine = new QFrame();
    warmLine->setObjectName("warmLine");
    tealLine = new QFrame();
    tealLine->setObjectName("tealLine");
    accentLayout->addWidget(warmLine);
    accentLayout->addWidget(tealLine, 1);
    m_rootLayout->addWidget(accentBar);

    // Контент
    m_contentWidget = new QWidget();
    m_contentWidget->setObjectName("contentWidget");
    m_contentLayout = new QGridLayout(m_contentWidget);
    m_contentLayout->setColumnStretch(0, 60);
    m_contentLayout->setColumnStretch(1, 40);

    // 1. Рабочая строка
    m_queryCard = new QFrame();
    m_queryCard->setObjectName("queryCard");
    auto* queryGrid = new QGridLayout(m_queryCard);

    queryTitleLbl = new QLabel("Рабочая строка");
    queryTitleLbl->setObjectName("queryTitle");
    
    m_queryEdit = new QLineEdit("(2+3i)/(4-5i)");
    m_queryEdit->setObjectName("queryEdit");
    m_queryEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    btnCalculate = createCommandButton("Вычислить", "btnCalculate");
    btnCalculate->setProperty("hoverHint", "Команда: Вычислить\n\nРазобрать выражение\nВычислить результат\nДобавить запись в историю");

    btnClear = createCommandButton("Очистить", "btnClear");
    btnClear->setProperty("hoverHint", "Команда: Очистить\n\nУдалить выражение\nОчистить результаты");

    sampleContainer = new QWidget();
    auto* flowSamples = new FlowLayout(sampleContainer, 12, 8, 8);
    struct SampleInfo { QString text; QString hint; };
    const QList<SampleInfo> sampleList = {
        {"2 + 3i", "Декартова форма: 2 + 3i\na = 2 (действительная часть)\nb = 3 (мнимая часть)"},
        {"(2+3i)/(4-5i)", "Операция деления:\n(2+3i)/(4-5i)\n\nВыполняет деление z₁ / z₂\nz₂ не должен быть равен нулю"},
        {"сопр(2+3i)", "Сопряженное число:\nсопр(z)=a−bi\n\nМеняет знак мнимой части"},
        {"полярная(5;45)", "Полярная форма:\nr = 5 (модуль)\n45° (угол в градусах)\nРазделитель — \";\""}
    };
    for (const auto& s : sampleList) {
        auto* b = createCommandButton(s.text, "btnSample");
        b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed); 
        b->setProperty("hoverHint", s.hint);
        connect(b, &QPushButton::clicked, this, &MainWindow::onSampleClicked);
        flowSamples->addWidget(b);
        sampleButtons.append(b);
    }

    queryGrid->addWidget(queryTitleLbl, 0, 0, 1, 2);
    queryGrid->addWidget(m_queryEdit, 1, 0);
    queryGrid->addWidget(btnCalculate, 1, 1);
    queryGrid->addWidget(sampleContainer, 2, 0);
    queryGrid->addWidget(btnClear, 2, 1); 
    queryGrid->setColumnStretch(0, 1);
    m_contentLayout->addWidget(m_queryCard, 0, 0, 1, 2);

    // 2. Карточки результатов
    m_interpretationLbl = createValueLabel();
    m_resultLbl = createValueLabel();
    m_polarLbl = createValueLabel();
    m_detailsLbl = createValueLabel();
    m_interpretationLbl->setText("Введите выражение и нажмите Enter.");

    cardInterpretation = createStyledCard("Интерпретация", m_interpretationLbl);
    cardResult = createStyledCard("Результат", m_resultLbl);
    cardTrig = createStyledCard("Тригонометрическая форма", m_polarLbl);
    cardDetails = createStyledCard("Подробные сведения", m_detailsLbl);

    m_contentLayout->addWidget(cardInterpretation, 1, 0);
    m_contentLayout->addWidget(cardResult, 2, 0);
    m_contentLayout->addWidget(cardTrig, 3, 0);
    m_contentLayout->addWidget(cardDetails, 4, 0);

    // 3. История вычислений
    m_historyText = new QTextEdit();
    m_historyText->setObjectName("historyDisplay");
    m_historyText->setReadOnly(true);
    m_historyText->document()->setDocumentMargin(0);
    m_historyText->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_historyText->setText("История пока пуста.\nРезультаты появятся здесь после вычислений.");
    cardHistory = createStyledCard("История вычислений", m_historyText);
    m_contentLayout->addWidget(cardHistory, 1, 1, 3, 1);

    // 4. Экспорт
    exportPanel = new QWidget();
    auto* exportGrid = new QGridLayout(exportPanel);
    exportGrid->setContentsMargins(12, 12, 12, 12);
    exportGrid->setSpacing(8);
    
    csvBtn = createCommandButton("CSV");
    jsonBtn = createCommandButton("JSON");
    excelBtn = createCommandButton("Excel");
    txtBtn = createCommandButton("TXT");
    wordBtn = createCommandButton("Word");
    allBtn = createCommandButton("Экспортировать всё", "btnExportAll");

    exportButtons = {csvBtn, jsonBtn, excelBtn, txtBtn, wordBtn};

    exportGrid->addWidget(csvBtn, 0, 0, 1, 2);
    exportGrid->addWidget(jsonBtn, 0, 2, 1, 2);
    exportGrid->addWidget(excelBtn, 0, 4, 1, 2);
    exportGrid->addWidget(txtBtn, 1, 0, 1, 3);
    exportGrid->addWidget(wordBtn, 1, 3, 1, 3);
    exportGrid->addWidget(allBtn, 2, 0, 1, 6);
    
    cardExport = createStyledCard("Экспорт данных", exportPanel);
    m_contentLayout->addWidget(cardExport, 4, 1);

    m_rootLayout->addWidget(m_contentWidget, 1);

    // Сигналы
    connect(helpButton, &QPushButton::clicked, this, &MainWindow::onHelpRequested);
    connect(m_themeButton, &QPushButton::clicked, this, &MainWindow::onThemeToggled);
    connect(btnCalculate, &QPushButton::clicked, this, &MainWindow::onCalculateTriggered);
    connect(m_queryEdit, &QLineEdit::returnPressed, this, &MainWindow::onCalculateTriggered);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearRequested);

    for (QPushButton* b : {csvBtn, jsonBtn, excelBtn, txtBtn, wordBtn, allBtn}) {
        connect(b, &QPushButton::clicked, this, &MainWindow::onExportClicked);
    }

    QResizeEvent re(size(), size());
    resizeEvent(&re);
}

QPushButton* MainWindow::createCommandButton(const QString& text, const QString& objectName) {
    auto* button = new QPushButton(text);
    button->setCursor(Qt::PointingHandCursor);
    if (!objectName.isEmpty()) button->setObjectName(objectName);
    button->installEventFilter(this);
    return button;
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (auto* button = qobject_cast<QPushButton*>(watched)) {
        if (event->type() == QEvent::Enter) {
            const QString hint = button->property("hoverHint").toString();
            if (!hint.isEmpty()) {
                QToolTip::showText(QCursor::pos(), hint, button);
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    
    double scaleX = static_cast<double>(width()) / DESIGN_WIDTH;
    double scaleY = static_cast<double>(height()) / DESIGN_HEIGHT;
    
    // Взвешенное масштабирование для шрифтов и отступов
    double scale = (scaleX + scaleY) / 2.0;
    if (scale < 0.75) scale = 0.75; 
    if (scale > 1.5) scale = 1.5;

    applyStyleSheet(scale);
    updateLayout(scale);
}

void MainWindow::updateLayout(double scale) {
    if (headerFrame) headerFrame->setFixedHeight(qRound(120 * scale));
    if (m_headerLayout) m_headerLayout->setContentsMargins(qRound(64 * scale), qRound(20 * scale), qRound(64 * scale), qRound(10 * scale));
    
    if (headerTitle) {
        QFont f = headerTitle->font();
        f.setBold(true);
        f.setWeight(QFont::Black);
        int px = qRound(32 * scale);
        if (px < 24) px = 24;
        f.setPixelSize(px);
        headerTitle->setFont(f);
        headerTitle->setStyleSheet(QString("font-size: %1px; font-weight: 900; margin: 0px; padding: 0px;").arg(px));
    }

    if (accentBar) accentBar->setFixedHeight(qRound(4 * scale));
    if (warmLine) warmLine->setFixedWidth(qRound(350 * scale));
    
    if (m_contentLayout) {
        m_contentLayout->setContentsMargins(qRound(64 * scale), qRound(16 * scale), qRound(64 * scale), qRound(16 * scale));
        m_contentLayout->setHorizontalSpacing(qRound(24 * scale));
        m_contentLayout->setVerticalSpacing(qRound(16 * scale));
    }

    // Одинаковый размер заголовков карточек
    int titleH = qMax(22, qRound(26 * scale));
    auto titles = findChildren<QLabel*>("cardTitle");
    for (auto* t : titles) {
        t->setFixedHeight(titleH);
        t->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }

    auto setS = [&](QPushButton* b, int w, int h, bool isSample = false) {
        if (!b) return;
        int nh = qMax(28, qRound(h * scale));
        int nw = qMax(isSample ? 90 : 60, qRound(w * scale));
        if (isSample) {
            b->setFixedHeight(nh);
            b->setMinimumWidth(nw);
            b->setMaximumWidth(qRound(200 * scale));
            b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        } else {
            b->setMinimumSize(nw, nh);
            b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        }
    };

    setS(btnCalculate, 170, 44); 
    setS(btnClear, 170, 36);     
    setS(helpButton, 100, 36);
    setS(m_themeButton, 140, 36);
    
    for (auto* b : sampleButtons) {
        const QString t = b->text();
        int baseW = 120;
        if (t.contains("/")) baseW = 160;
        else if (t.contains("сопр")) baseW = 140;
        else if (t.contains("полярная")) baseW = 160;
        setS(b, baseW, 38, true);
    }

    for (auto* b : exportButtons) {
        if (!b) continue;
        int minW = qMax(40, qRound(65 * scale)); 
        if (b == txtBtn || b == wordBtn) minW = qMax(60, qRound(100 * scale));
        b->setMinimumSize(minW, qMax(24, qRound(34 * scale)));
        b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
    if (allBtn) {
        allBtn->setMinimumHeight(qMax(28, qRound(36 * scale)));
        allBtn->setMinimumWidth(qMax(100, qRound(150 * scale)));
        allBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    if (exportPanel && exportPanel->layout()) {
        auto* l = qobject_cast<QGridLayout*>(exportPanel->layout());
        if (l) {
            int m = qMax(4, qRound(12 * scale));
            int s = qMax(2, qRound(8 * scale));
            l->setContentsMargins(m, m, m, m);
            l->setSpacing(s);
        }
    }

    if (m_queryEdit) m_queryEdit->setMinimumHeight(qMax(32, qRound(42 * scale)));

    auto setCard = [&](QFrame* c, int h) {
        if (!c) return;
        c->setMinimumHeight(qMax(qRound(h * 0.65), qRound(h * scale)));
        auto* lay = c->layout();
        if (lay) {
            int hm = qMax(8, qRound(16 * scale));
            int vm = qMax(6, qRound(12 * scale));
            lay->setContentsMargins(hm, vm, hm, vm);
            lay->setSpacing(qMax(4, qRound(12 * scale)));
        }
    };
    setCard(cardInterpretation, 100);
    setCard(cardResult, 100);
    setCard(cardTrig, 130);
    setCard(cardDetails, 150);
    setCard(cardHistory, 350);
    setCard(cardExport, 180);

    if (univLabel) univLabel->setMinimumWidth(qRound(340 * scale));
    if (infoLabel) infoLabel->setMinimumWidth(qRound(340 * scale));
}

QFrame* MainWindow::createStyledCard(const QString& title, QWidget* content, int) {
    auto* card = new QFrame();
    card->setObjectName("cardFrame");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto* cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(16, 12, 16, 12);
    cardLayout->setSpacing(12);

    auto* accent = new QFrame();
    accent->setObjectName("cardAccentLine");
    accent->setFixedWidth(2);

    auto* inner = new QVBoxLayout();
    inner->setContentsMargins(0, 0, 0, 0);
    inner->setSpacing(8);

    auto* titleLabel = new QLabel(title);
    titleLabel->setObjectName("cardTitle");

    inner->addWidget(titleLabel);
    inner->addWidget(content, 1);

    cardLayout->addWidget(accent);
    cardLayout->addLayout(inner, 1);
    return card;
}

QLabel* MainWindow::createValueLabel() {
    auto* label = new QLabel("—");
    label->setObjectName("valueLabel");
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    return label;
}

void MainWindow::applyStyleSheet(double scale) {
    int fsTitle = 72; // Still calculated but not used in %2 for title if removed
    if (scale < 0.7) fsTitle = 48;
    if (scale < 0.5) fsTitle = 36;

    int fsSubtitle = qRound(13 * scale);
    if (fsSubtitle < 10) fsSubtitle = 10;

    int fsBase = qRound(11 * scale);
    if (fsBase < 10) fsBase = 10;
    
    int fsEdit = qRound(14 * scale);
    if (fsEdit < 10) fsEdit = 10;

    int fsCardTitle = qRound(12 * scale);
    if (fsCardTitle < 10) fsCardTitle = 10;

    const QString dark = QString(R"(
        #centralWidget, #contentWidget { background: #0C1018; }
        #centralWidget QLabel, #centralWidget QPushButton, #centralWidget QLineEdit, #centralWidget QFrame { 
            color: #EBF0F8; font-family: "Segoe UI", "Arial"; font-size: %1px; 
        }
        #headerFrame { background: #080B12; border: none; }
        #headerTitle { color: #FFFFFF; }
        #headerSubtitle, #studentInfo { color: #9AA6B8; font-size: %6px; }
        #accentBar { background: #2DD4BF; }
        #warmLine { background: #FBBF24; border: none; }
        #tealLine { background: #2DD4BF; border: none; }
        #queryCard, #cardFrame { background: #19202C; border: 1px solid #374256; border-radius: 8px; }
        #queryTitle, #cardTitle { color: #2DD4BF; font-size: %3px; font-weight: 700; text-transform: uppercase; }
        #queryEdit { background: #111827; border: 1px solid #374256; border-radius: 6px; color: #FFFFFF; font-size: %4px; padding: 0 12px; }
        #queryEdit:focus { border: 1px solid #2DD4BF; }
        
        QPushButton { 
            background: #1E293B; border: 1px solid #374256; border-radius: 6px; color: #FFFFFF; 
            font-weight: 600; padding: 5px 12px; font-size: %1px; 
        }
        QPushButton:hover { background: #334155; border: 1px solid #2DD4BF; }
        QPushButton:pressed { background: #0F172A; padding-top: 7px; }

        #btnCalculate, #btnExportAll { 
            background-color: #0F766E; color: #FFFFFF; font-weight: 700; border-radius: 8px; border: 1px solid #0F766E;
        }
        #btnCalculate:hover, #btnExportAll:hover { 
            background-color: #5EEAD4; color: #0F172A; border: 1px solid #5EEAD4;
        }
        #btnCalculate:pressed, #btnExportAll:pressed {
            background-color: #14B8A6; color: #0F172A; border: 1px solid #14B8A6;
            padding-top: 7px;
        }
        
        #btnSample { background: #1E293B; font-weight: 600; font-size: %5px; color: #EBF0F8; }
        #btnSample:hover { color: #2DD4BF; border: 1px solid #2DD4BF; }
        
        #headerButton { background: #1E293B; padding: 4px 12px; font-size: 11px; }
        #valueLabel, #historyDisplay { color: #EBF0F8; font-size: %1px; background: transparent; }
        QToolTip { background-color: #111827; color: #FFFFFF; border: 1px solid #374256; padding: 4px; font-size: 11px; }
    )").arg(fsBase).arg(fsTitle).arg(fsCardTitle).arg(fsEdit).arg(qMax(10, qRound(11*scale))).arg(fsSubtitle);

    const QString light = QString(R"(
        #centralWidget, #contentWidget { background: #F7F3EC; }
        #centralWidget QLabel, #centralWidget QPushButton, #centralWidget QLineEdit, #centralWidget QFrame { 
            color: #2F2A25; font-family: "Segoe UI", "Arial"; font-size: %1px; 
        }
        #headerFrame { background: #F5EFE5; border-bottom: 1px solid #D9CCBA; }
        #headerTitle { color: #2F2A25; }
        #headerSubtitle, #studentInfo { color: #7A6E61; font-size: %6px; }
        #accentBar { background: #2D8E87; }
        #warmLine { background: #C28B52; border: none; }
        #tealLine { background: #2D8E87; border: none; }
        #queryCard, #cardFrame { background: #FFFDF9; border: 1px solid #D9CCBA; border-radius: 12px; }
        #queryTitle, #cardTitle { color: #2D8E87; font-size: %3px; font-weight: 700; text-transform: uppercase; }
        #queryEdit { background: #F7F3EC; border: 1px solid #D9CCBA; border-radius: 6px; color: #2F2A25; font-size: %4px; padding: 0 12px; }
        #queryEdit:focus { border: 1px solid #2D8E87; background: #FFFDF9; }
        
        QPushButton { 
            background: #F5EFE5; border: 1px solid #D9CCBA; border-radius: 6px; 
            font-weight: 600; padding: 5px 12px; font-size: 11px; 
        }
        QPushButton:hover { background: #DCEBE6; border: 1px solid #2D8E87; }
        QPushButton:pressed { background: #23736E; color: white; padding-top: 7px; }

        #btnCalculate,
        #btnExportAll
        {
            background-color: #B8865A;
            color: #FFFDF8;
            border: 1px solid #B8865A;
            font-size: 14px;
            font-weight: 700;
            border-radius: 8px;
        }

        #btnCalculate:hover,
        #btnExportAll:hover
        {
            background-color: #A3734A;
            color: #FFFFFF;
            border: 1px solid #A3734A;
        }

        #btnCalculate:pressed,
        #btnExportAll:pressed
        {
            background-color: #8D613B;
            color: #FFFFFF;
            border: 1px solid #8D613B;
            padding-top: 7px;
        }

        #btnSample { background: #F7F3EC; font-weight: 600; font-size: %5px; color: #2F2A25; }
        #btnSample:hover { background: #DCEBE6; color: #2D8E87; border: 1px solid #2D8E87; }

        #headerButton { background: #F5EFE5; padding: 4px 12px; font-size: 11px; }
        #valueLabel, #historyDisplay { color: #2F2A25; font-size: %1px; background: transparent; }
        QToolTip { background-color: #FFFDF9; color: #2F2A25; border: 1px solid #D9CCBA; padding: 4px; font-size: 11px; }
    )").arg(fsBase).arg(fsTitle).arg(fsCardTitle).arg(fsEdit).arg(qMax(10, qRound(11*scale))).arg(fsSubtitle);

    setStyleSheet(m_lightTheme ? light : dark);
    if (m_themeButton) {
        m_themeButton->setText(m_lightTheme ? "Темная тема" : "Светлая тема");
    }
    updateHistoryDisplay();
}

void MainWindow::showMessage(const QString& title, const QString& text, QMessageBox::Icon icon) {
    QMessageBox msgBox(icon, title, text, QMessageBox::Ok, this);
    msgBox.setWindowModality(Qt::WindowModal);
    msgBox.setStyleSheet("background-color: palette(window); color: palette(window-text);");
    msgBox.setPalette(QApplication::style()->standardPalette());
    QStyle* winStyle = QStyleFactory::create("windows");
    if (winStyle) msgBox.setStyle(winStyle);
    msgBox.exec();
}

void MainWindow::onSampleClicked() {
    if (auto* button = qobject_cast<QPushButton*>(sender())) {
        m_queryEdit->setText(button->text());
        m_queryEdit->setFocus();
    }
}

void MainWindow::onClearRequested() {
    m_queryEdit->clear();
    m_interpretationLbl->setText("Введите выражение и нажмите Enter.");
    for (QLabel* label : {m_resultLbl, m_polarLbl, m_detailsLbl}) label->setText("—");
    m_queryEdit->setFocus();
}

void MainWindow::onCalculateTriggered() {
    const QString source = m_queryEdit->text().trimmed();
    if (source.isEmpty()) {
        showMessage("Пустой запрос", "Введите выражение, например: (2+3i)/(4-5i).");
        m_queryEdit->setFocus();
        return;
    }
    const std::string expression = toCoreText(source);
    ExpressionParser parser(expression);
    const auto result = parser.parse();
    if (!result.success) {
        showMessage("Ошибка в выражении", toUiText(result.message), QMessageBox::Critical);
        m_queryEdit->setFocus();
        return;
    }
    m_interpretationLbl->setText("Введено: " + source);
    m_resultLbl->setText(toUiText(complexToString(result.value)));
    m_polarLbl->setText(toUiText(polarFormText(result.value)));
    m_detailsLbl->setText(QString("Действительная часть: %1\nМнимая часть: %2\nМодуль: %3\nАргумент: %4°")
        .arg(toUiText(formatDouble(result.value.real)))
        .arg(toUiText(formatDouble(result.value.imag)))
        .arg(toUiText(formatDouble(complexModulus(result.value))))
        .arg(toUiText(formatDouble(argumentDegrees(result.value)))));
    m_history.push_back({static_cast<int>(m_history.size() + 1), nowString(), expression, result.value, "Выполнено"});
    updateHistoryDisplay();
}

void MainWindow::updateHistoryDisplay() {
    if (m_history.empty()) {
        m_historyText->setText("История пока пуста.\nРезультаты появятся здесь после вычислений.");
        return;
    }
    const QString textColor = m_lightTheme ? "#111827" : "#EBF0F8";
    const QString secondaryColor = m_lightTheme ? "#4B5563" : "#9AA6B8";
    const QString badgeBg = m_lightTheme ? "#E0F2FE" : "#164E4C";
    const QString badgeText = m_lightTheme ? "#0891B2" : "#5EEAD4";

    QString html = QString("<style>div { font-family: 'Segoe UI'; font-size: 11px; margin-bottom: 10px; color: %1; }</style>").arg(textColor);
    for (auto it = m_history.rbegin(); it != m_history.rend(); ++it) {
        const QString expression = toUiText(it->expression).toHtmlEscaped();
        const QString value = toUiText(complexToString(it->result)).toHtmlEscaped();
        html += QString(
            "<div>"
            "<span style='background:%1; color:%2; padding:1px 4px; border-radius:2px; font-weight:bold;'>#%3</span>"
            "<span style='color:%4;'> %5</span><br>"
            "<span>%6</span><br>"
            "<span style='color:%4;'>= %7</span>"
            "</div>"
        ).arg(badgeBg).arg(badgeText).arg(it->id).arg(secondaryColor).arg(toUiText(it->dateTime).toHtmlEscaped()).arg(expression).arg(value);
    }
    m_historyText->setHtml(html);
}

void MainWindow::onExportClicked() {
    if (m_history.empty()) {
        showMessage("Сохранение", "Журнал пуст. Сначала выполните хотя бы одно вычисление.");
        return;
    }
    auto* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    const QString label = button->text();
    if (label == "Экспортировать всё") {
        QString path = QFileDialog::getSaveFileName(this, "Сохранение всех форматов", "", "All Files (*)");
        if (path.isEmpty()) return;
        QString base = path;
        int lastDot = base.lastIndexOf('.');
        int lastSlash = qMax(base.lastIndexOf('/'), base.lastIndexOf('\\'));
        if (lastDot != -1 && lastDot > lastSlash) base = base.left(lastDot);
        saveCSV(m_history, toCoreText(base + ".csv"));
        saveJSON(m_history, toCoreText(base + ".json"));
        saveXLSX(m_history, toCoreText(base + ".xlsx"));
        saveTXT(m_history, toCoreText(base + ".txt"));
        saveDOCX(m_history, toCoreText(base + ".docx"));
        showMessage("Сохранение всех форматов", "Файлы успешно сохранены.");
        return;
    }
    QString defaultName, filter;
    if (label == "CSV") { defaultName = "result.csv"; filter = "CSV Files (*.csv)"; }
    else if (label == "JSON") { defaultName = "result.json"; filter = "JSON Files (*.json)"; }
    else if (label == "Excel") { defaultName = "result.xlsx"; filter = "Excel Files (*.xlsx)"; }
    else if (label == "TXT") { defaultName = "result.txt"; filter = "Text Files (*.txt)"; }
    else if (label == "Word") { defaultName = "result.docx"; filter = "Word Files (*.docx)"; }
    const QString path = QFileDialog::getSaveFileName(this, "Сохранение", defaultName, filter);
    if (path.isEmpty()) return;
    const std::string fileName = toCoreText(path);
    bool ok = false;
    if (label == "CSV") ok = saveCSV(m_history, fileName);
    else if (label == "JSON") ok = saveJSON(m_history, fileName);
    else if (label == "Excel") ok = saveXLSX(m_history, fileName);
    else if (label == "TXT") ok = saveTXT(m_history, fileName);
    else if (label == "Word") ok = saveDOCX(m_history, fileName);
    if (ok) showMessage("Сохранение", "Файл сохранен: " + path);
}

void MainWindow::showHelpDialog() {
    auto* dialog = new QDialog(this);
    dialog->setWindowTitle("Справка - теория комплексных чисел");
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMinimumSize(900, 700);
    dialog->resize(900, 700);
    auto* layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(0, 0, 0, 0);
    auto* text = new QPlainTextEdit(dialog);
    text->setReadOnly(true);
    text->setObjectName("helpText");
    text->setPlainText(QString::fromUtf8(helpTheoryText()));
    layout->addWidget(text);
    const QString darkHelp = R"(QDialog { background: #0C1018; } #helpText { background: #0C1018; border: none; color: #EBF0F8; font-family: "Consolas", "Segoe UI", monospace; font-size: 14pt; padding: 10px; } QScrollBar:vertical { background: #0C1018; width: 12px; } QScrollBar::handle:vertical { background: #374256; border-radius: 6px; margin: 2px; })";
    const QString lightHelp = R"(QDialog { background: #F2EEE6; } #helpText { background: #F2EEE6; border: none; color: #2D2823; font-family: "Consolas", "Segoe UI", monospace; font-size: 14pt; padding: 10px; })";
    dialog->setStyleSheet(m_lightTheme ? lightHelp : darkHelp);
    dialog->show();
}

void MainWindow::onHelpRequested() { showHelpDialog(); }
void MainWindow::onThemeToggled() { m_lightTheme = !m_lightTheme; QResizeEvent re(size(), size()); resizeEvent(&re); }
