#include "TableInputWidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

TableInputWidget::TableInputWidget(int functionIndex, QWidget *parent)
    : QWidget(parent)
    , m_functionIndex(functionIndex)
    , m_isValid(false)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Выбор количества переменных
    QHBoxLayout* varLayout = new QHBoxLayout();
    varLayout->addWidget(new QLabel("Количество переменных (1-7):"));
    m_varCountSpin = new QSpinBox();
    m_varCountSpin->setRange(1, 7);
    m_varCountSpin->setValue(2);
    varLayout->addWidget(m_varCountSpin);
    varLayout->addStretch();
    layout->addLayout(varLayout);
    
    // Таблица истинности
    m_table = new QTableWidget();
    m_table->setEditTriggers(QAbstractItemView::DoubleClicked | 
                            QAbstractItemView::EditKeyPressed);
    layout->addWidget(m_table);
    
    // Статус
    m_statusLabel = new QLabel("Введите значения (0 или 1)");
    m_statusLabel->setStyleSheet("color: gray;");
    layout->addWidget(m_statusLabel);
    
    // Подключаем сигналы
    connect(m_varCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &TableInputWidget::onVarCountChanged);
    connect(m_table, &QTableWidget::cellChanged,
            this, &TableInputWidget::onCellChanged);
    
    // Инициализация
    rebuildTable();
}

void TableInputWidget::onVarCountChanged(int count) {
    rebuildTable();
}

void TableInputWidget::rebuildTable() {
    int varCount = m_varCountSpin->value();
    int rowCount = 1 << varCount;
    
    m_table->clear();
    m_table->setRowCount(rowCount);
    m_table->setColumnCount(1);
    
    // Заголовки столбцов
    QStringList horizontalHeaders;
    horizontalHeaders << "f(" + QString::number(m_functionIndex) + ")";
    m_table->setHorizontalHeaderLabels(horizontalHeaders);
    
    // Заполняем номера строк и возможные наборы переменных
    QStringList verticalHeaders;
    for (int i = 0; i < rowCount; i++) {
        // Генерируем набор переменных для строки
        QString setStr = "(";
        for (int bit = varCount - 1; bit >= 0; bit--) {
            setStr += QString::number((i >> bit) & 1);
            if (bit > 0) setStr += ",";
        }
        setStr += ")";
        verticalHeaders << setStr;
        
        // Создаем пустую ячейку
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText("");
        m_table->setItem(i, 0, item);
    }
    m_table->setVerticalHeaderLabels(verticalHeaders);
    
    // Настройка размеров
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->resizeColumnsToContents();
    
    m_isValid = false;
    m_statusLabel->setText("Введите значения (0 или 1)");
    m_statusLabel->setStyleSheet("color: gray;");
}

void TableInputWidget::onCellChanged(int row, int col) {
    QTableWidgetItem* item = m_table->item(row, col);
    if (!item) return;
    
    QString value = item->text().trimmed();
    
    if (value.isEmpty()) {
        m_isValid = false;
        m_statusLabel->setText("Не все ячейки заполнены");
        m_statusLabel->setStyleSheet("color: orange;");
        return;
    }
    
    if (!validateValue(value)) {
        item->setText("");
        QMessageBox::warning(this, "Ошибка ввода", 
                           "Ячейка должна содержать 0 или 1");
        m_isValid = false;
        m_statusLabel->setText("Некорректное значение (требуется 0 или 1)");
        m_statusLabel->setStyleSheet("color: red;");
        return;
    }
    
    // Проверяем, все ли ячейки заполнены
    bool allFilled = true;
    for (int r = 0; r < m_table->rowCount(); r++) {
        QTableWidgetItem* cell = m_table->item(r, 0);
        if (!cell || cell->text().trimmed().isEmpty()) {
            allFilled = false;
            break;
        }
    }
    
    if (allFilled) {
        m_isValid = true;
        m_statusLabel->setText("Таблица заполнена корректно");
        m_statusLabel->setStyleSheet("color: green;");
    } else {
        m_isValid = false;
        m_statusLabel->setText("Не все ячейки заполнены");
        m_statusLabel->setStyleSheet("color: orange;");
    }
}

bool TableInputWidget::validateValue(const QString& value) const {
    return (value == "0" || value == "1");
}

TruthTable TableInputWidget::getTruthTable() const {
    int varCount = m_varCountSpin->value();
    int rowCount = 1 << varCount;
    std::vector<bool> values;
    values.reserve(rowCount);
    
    for (int i = 0; i < rowCount; i++) {
        QTableWidgetItem* item = m_table->item(i, 0);
        if (item && !item->text().trimmed().isEmpty()) {
            values.push_back(item->text().trimmed() == "1");
        } else {
            throw std::runtime_error("Incomplete truth table");
        }
    }
    
    return TruthTable(varCount, values);
}

bool TableInputWidget::isValid() const {
    return m_isValid;
}
