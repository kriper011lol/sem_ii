#pragma once

#include <QMainWindow>
#include <QPointer>
#include <QMessageBox>
#include <vector>
#include <memory>
#include "core.h"

class QLineEdit;
class QLabel;
class QPushButton;
class QTextEdit;
class QFrame;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

/**
 * @brief Главное окно калькулятора с кастомным дизайном.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onCalculateTriggered();
    void onClearRequested();
    void onSampleClicked();
    void onExportClicked();
    void onHelpRequested();
    void onThemeToggled();

private:
    void setupUi();
    void applyStyleSheet(double scale = 1.0);
    void updateHistoryDisplay();
    void updateLayout(double scale);
    void showMessage(const QString& title, const QString& text, QMessageBox::Icon icon = QMessageBox::Information);
    
    // Вспомогательный метод для создания стилизованных карточек
    QFrame* createStyledCard(const QString& title, QWidget* content, int minimumHeight = 0);
    // Вспомогательный метод для создания ярлыков внутри карточек
    QLabel* createValueLabel();
    QPushButton* createCommandButton(const QString& text, const QString& objectName = QString());
    void showHelpDialog();

    std::vector<HistoryItem> m_history;
    bool m_lightTheme = false;

    // Layouts для масштабирования
    QVBoxLayout* m_rootLayout = nullptr;
    QHBoxLayout* m_headerLayout = nullptr;
    QGridLayout* m_contentLayout = nullptr;
    QHBoxLayout* m_sampleRowLayout = nullptr;

    // UI элементы (для масштабирования в updateLayout)
    QFrame* headerFrame = nullptr;
    QLabel* headerTitle = nullptr;
    QLabel* headerSubtitle = nullptr;
    QLabel* univLabel = nullptr;
    QLabel* infoLabel = nullptr;
    QPushButton* helpButton = nullptr;
    QWidget* accentBar = nullptr;
    QFrame* warmLine = nullptr;
    QFrame* tealLine = nullptr;
    QLabel* queryTitleLbl = nullptr;
    QPushButton* btnCalculate = nullptr;
    QPushButton* btnClear = nullptr;
    QWidget* sampleContainer = nullptr;
    QList<QPushButton*> sampleButtons;
    QList<QPushButton*> exportButtons;
    QFrame* cardInterpretation = nullptr;
    QFrame* cardResult = nullptr;
    QFrame* cardTrig = nullptr;
    QFrame* cardDetails = nullptr;
    QFrame* cardHistory = nullptr;
    QFrame* cardExport = nullptr;
    QWidget* exportPanel = nullptr;
    QPushButton* csvBtn = nullptr;
    QPushButton* jsonBtn = nullptr;
    QPushButton* excelBtn = nullptr;
    QPushButton* txtBtn = nullptr;
    QPushButton* wordBtn = nullptr;
    QPushButton* allBtn = nullptr;

    QLineEdit* m_queryEdit = nullptr;
    QPointer<QWidget> m_contentWidget;
    QPointer<QFrame> m_queryCard;
    QPushButton* m_themeButton = nullptr;
    QLabel* m_interpretationLbl = nullptr;
    QLabel* m_resultLbl = nullptr;
    QLabel* m_polarLbl = nullptr;
    QLabel* m_detailsLbl = nullptr;
    QTextEdit* m_historyText = nullptr;
};
