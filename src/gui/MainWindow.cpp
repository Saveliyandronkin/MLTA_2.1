#include "MainWindow.h"
#include "TableInputWidget.h"
#include "ResultsWidget.h"
#include "../core/FunctionSet.h"
#include "../core/CompletenessChecker.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QTextBrowser>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createMenuBar();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Верхняя панель с настройками
    QHBoxLayout* topPanel = new QHBoxLayout();
    topPanel->addWidget(new QLabel("Количество функций (1-5):"));
    m_functionCountSpin = new QSpinBox();
    m_functionCountSpin->setRange(1, 5);
    m_functionCountSpin->setValue(1);
    topPanel->addWidget(m_functionCountSpin);
    topPanel->addStretch();
    
    // Добавляем информацию об авторе в интерфейс
    QLabel* authorLabel = new QLabel("Андронкин Савелий Алексеевич (группа 2012)");
    authorLabel->setStyleSheet("color: gray; font-size: 10px;");
    topPanel->addWidget(authorLabel);
    
    mainLayout->addLayout(topPanel);
    
    // Tab-виджет для ввода таблиц
    m_tabWidget = new QTabWidget();
    mainLayout->addWidget(m_tabWidget);
    
    // Кнопка проверки
    m_checkButton = new QPushButton("Проверить полноту и базис");
    m_checkButton->setStyleSheet("font-weight: bold; padding: 5px;");
    mainLayout->addWidget(m_checkButton);
    
    // Виджет результатов
    m_resultsWidget = new ResultsWidget();
    mainLayout->addWidget(m_resultsWidget);
    
    setCentralWidget(centralWidget);
    
    // Подключаем сигналы
    connect(m_functionCountSpin, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &MainWindow::onFunctionCountChanged);
    connect(m_checkButton, &QPushButton::clicked, 
            this, &MainWindow::onCheckClicked);
    
    // Создаем начальный набор вкладок
    onFunctionCountChanged(1);
    
    setWindowTitle("Проверка полноты системы булевых функций");
    setMinimumSize(900, 700);
}

void MainWindow::createMenuBar() {
    m_menuBar = new QMenuBar(this);
    
    // Создаем меню "Справка"
    m_helpMenu = new QMenu("Справка", m_menuBar);
    
    // Добавляем пункты меню
    m_helpAction = new QAction("Помощь", this);
    m_helpAction->setShortcut(QKeySequence::HelpContents);
    connect(m_helpAction, &QAction::triggered, this, &MainWindow::onHelpClicked);
    
    m_aboutAction = new QAction("О программе", this);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAboutClicked);
    
    // Добавляем действия в меню
    m_helpMenu->addAction(m_helpAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_aboutAction);
    
    // Добавляем меню в строку меню
    m_menuBar->addMenu(m_helpMenu);
    
    setMenuBar(m_menuBar);
}

void MainWindow::onFunctionCountChanged(int count) {
    // Очищаем текущие вкладки
    m_tabWidget->clear();
    m_tableWidgets.clear();
    
    // Создаем новые вкладки для каждой функции
    for (int i = 0; i < count; i++) {
        TableInputWidget* tableWidget = new TableInputWidget(i + 1, this);
        m_tabWidget->addTab(tableWidget, QString("Функция %1").arg(i + 1));
        m_tableWidgets.push_back(tableWidget);
    }
}

void MainWindow::onCheckClicked() {
    if (!validateAllTables()) {
        QMessageBox::warning(this, "Ошибка ввода", 
                            "Пожалуйста, заполните все таблицы полностью "
                            "(только 0 и 1 во всех строках)");
        return;
    }
    
    // Собираем все таблицы
    std::vector<TruthTable> functions;
    collectFunctions(functions);
    
    // Создаем набор функций
    FunctionSet functionSet;
    try {
        for (const auto& func : functions) {
            functionSet.addFunction(func);
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
        return;
    }
    
    // Проверяем полноту
    CompletenessChecker checker;
    CheckResult result = checker.checkSet(functionSet);
    
    // Отображаем результаты
    m_resultsWidget->displayResults(result);
}

bool MainWindow::validateAllTables() {
    for (auto* widget : m_tableWidgets) {
        if (!widget->isValid()) {
            return false;
        }
    }
    return true;
}

void MainWindow::collectFunctions(std::vector<TruthTable>& functions) {
    functions.clear();
    for (auto* widget : m_tableWidgets) {
        functions.push_back(widget->getTruthTable());
    }
}

void MainWindow::onHelpClicked() {
    showHelpDialog();
}

void MainWindow::onAboutClicked() {
    showAboutDialog();
}

void MainWindow::showHelpDialog() {
    QDialog* helpDialog = new QDialog(this);
    helpDialog->setWindowTitle("Помощь");
    helpDialog->setMinimumSize(600, 500);
    helpDialog->setModal(true);
    
    QVBoxLayout* layout = new QVBoxLayout(helpDialog);
    
    QTextBrowser* helpText = new QTextBrowser();
    helpText->setOpenExternalLinks(true);
    
    QString helpContent = 
        "<h2>Программа проверки полноты системы булевых функций</h2>"
        "<br>"
        
        "<h3>Назначение программы</h3>"
        "<p>Программа предназначена для проверки, является ли заданный набор булевых функций "
        "функционально полным (базисом) согласно теореме Поста.</p>"
        "<br>"
        
        "<h3>Теоретическое обоснование</h3>"
        "<p>Согласно теореме Поста, система булевых функций является функционально полной тогда и только тогда, "
        "когда для каждого из пяти классов Поста (T0, T1, S, M, L) в системе найдется функция, "
        "не принадлежащая этому классу.</p>"
        "<p>Классы Поста:</p>"
        "<ul>"
        "<li><b>T0</b> - функции, сохраняющие 0 (f(0,...,0)=0)</li>"
        "<li><b>T1</b> - функции, сохраняющие 1 (f(1,...,1)=1)</li>"
        "<li><b>S</b> - самодвойственные функции (f(~x) = ~f(x))</li>"
        "<li><b>M</b> - монотонные функции (a ≤ b ⇒ f(a) ≤ f(b))</li>"
        "<li><b>L</b> - линейные функции (представимые полиномом Жегалкина 1-й степени)</li>"
        "</ul>"
        "<br>"
        
        "<h3>Как пользоваться программой</h3>"
        "<ol>"
        "<li>Выберите количество функций (от 1 до 5) в верхней панели</li>"
        "<li>Для каждой функции укажите количество переменных (от 1 до 7)</li>"
        "<li>Заполните таблицу истинности: введите 0 или 1 для каждого набора переменных</li>"
        "<li>Убедитесь, что все ячейки заполнены (статус станет зеленым)</li>"
        "<li>Нажмите кнопку \"Проверить полноту и базис\"</li>"
        "<li>Ознакомьтесь с результатами: принадлежность функций классам Поста и вывод о полноте</li>"
        "</ol>"
        "<br>"
        
        "<h3>Примеры заполнения</h3>"
        "<p><b>Функция И (конъюнкция) от 2 переменных:</b></p>"
        "<pre>"
        "(0,0) → 0"
        "(0,1) → 0"
        "(1,0) → 0"
        "(1,1) → 1"
        "</pre>"
        
        "<p><b>Функция ИЛИ (дизъюнкция) от 2 переменных:</b></p>"
        "<pre>"
        "(0,0) → 0"
        "(0,1) → 1"
        "(1,0) → 1"
        "(1,1) → 1"
        "</pre>"
        
        "<p><b>Функция НЕ (отрицание) от 1 переменной:</b></p>"
        "<pre>"
        "(0) → 1"
        "(1) → 0"
        "</pre>"
        "<br>"
        
        "<h3>Интерпретация результатов</h3>"
        "<ul>"
        "<li><b>✔ ДА</b> - функция принадлежит данному классу Поста</li>"
        "<li><b>✗ НЕТ</b> - функция НЕ принадлежит данному классу Поста</li>"
        "<li><b>Набор полный</b> - система функций образует полную систему</li>"
        "<li><b>Базис</b> - полная и минимальная система (удаление любой функции нарушает полноту)</li>"
        "</ul>"
        "<br>"
        
        "<h3>Примечания</h3>"
        "<ul>"
        "<li>Количество строк в таблице = 2^n, где n - количество переменных</li>"
        "<li>Порядок наборов переменных - стандартный двоичный (от 0 до 2^n-1)</li>"
        "<li>Программа автоматически проверяет корректность ввода</li>"
        "</ul>";
    
    helpText->setHtml(helpContent);
    layout->addWidget(helpText);
    
    QPushButton* closeButton = new QPushButton("Закрыть");
    connect(closeButton, &QPushButton::clicked, helpDialog, &QDialog::accept);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);
    
    helpDialog->exec();
}

void MainWindow::showAboutDialog() {
    QDialog* aboutDialog = new QDialog(this);
    aboutDialog->setWindowTitle("О программе");
    aboutDialog->setMinimumSize(400, 300);
    aboutDialog->setModal(true);
    
    QVBoxLayout* layout = new QVBoxLayout(aboutDialog);
    
    QLabel* titleLabel = new QLabel("Проверка полноты системы булевых функций");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    layout->addSpacing(20);
    
    QLabel* authorLabel = new QLabel("Автор: Андронкин Савелий Алексеевич");
    authorLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(authorLabel);
    
    QLabel* versionLabel = new QLabel("Версия: 1.0");
    versionLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(versionLabel);
    
    QLabel* yearLabel = new QLabel("Группа: 2012");
    yearLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(yearLabel);
    
    layout->addSpacing(20);
    
    QLabel* descLabel = new QLabel(
        "Программа разработана в рамках дисциплины\n"
        "\"Математическая логика и теория алгоритмов\"\n\n"
        "Реализует проверку функциональной полноты\n"
        "систем булевых функций на основе теоремы Поста."
    );
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setStyleSheet("color: gray;");
    layout->addWidget(descLabel);
    
    layout->addSpacing(20);
    
    QPushButton* closeButton = new QPushButton("Закрыть");
    connect(closeButton, &QPushButton::clicked, aboutDialog, &QDialog::accept);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);
    
    aboutDialog->exec();
}
