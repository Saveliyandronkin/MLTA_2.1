#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include "../core/CompletenessChecker.h"

class ResultsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ResultsWidget(QWidget *parent = nullptr);
    
    void displayResults(const CheckResult& result);
    void clear();
    
private:
    void setupUI();
    QString formatFunctionInfo(const FunctionPostInfo& info, int index) const;
    
    QTextEdit* m_textEdit;
};

#endif // RESULTSWIDGET_H
