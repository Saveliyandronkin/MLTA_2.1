#ifndef TABLEINPUTWIDGET_H
#define TABLEINPUTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QSpinBox>
#include "../core/TruthTable.h"

class TableInputWidget : public QWidget {
    Q_OBJECT

public:
    explicit TableInputWidget(int functionIndex, QWidget *parent = nullptr);
    
    TruthTable getTruthTable() const;
    bool isValid() const;
    
private slots:
    void onVarCountChanged(int count);
    void onCellChanged(int row, int col);
    
private:
    void rebuildTable();
    bool validateValue(const QString& value) const;
    
    int m_functionIndex;
    QSpinBox* m_varCountSpin;
    QTableWidget* m_table;
    QLabel* m_statusLabel;
    bool m_isValid;
};

#endif // TABLEINPUTWIDGET_H
