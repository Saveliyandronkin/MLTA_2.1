#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <memory>
#include <vector>

#include "../core/TruthTable.h"

class TableInputWidget;
class ResultsWidget;
class FunctionSet;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFunctionCountChanged(int count);
    void onCheckClicked();
    void onHelpClicked();
    void onAboutClicked();

private:
    void setupUI();
    void createMenuBar();
    void collectFunctions(std::vector<TruthTable>& functions);
    bool validateAllTables();
    void showHelpDialog();
    void showAboutDialog();

    QTabWidget* m_tabWidget;
    QSpinBox* m_functionCountSpin;
    QPushButton* m_checkButton;
    ResultsWidget* m_resultsWidget;
    std::vector<TableInputWidget*> m_tableWidgets;
    
    // Элементы меню
    QMenuBar* m_menuBar;
    QMenu* m_helpMenu;
    QAction* m_helpAction;
    QAction* m_aboutAction;
};

#endif // MAINWINDOW_H
