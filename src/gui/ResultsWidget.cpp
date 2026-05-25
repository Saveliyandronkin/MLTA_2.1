#include "ResultsWidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QString>

ResultsWidget::ResultsWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void ResultsWidget::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QLabel* title = new QLabel("Результаты проверки:");
    title->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(title);
    
    m_textEdit = new QTextEdit();
    m_textEdit->setReadOnly(true);
    m_textEdit->setFont(QFont("Courier New", 10));
    layout->addWidget(m_textEdit);
}

void ResultsWidget::displayResults(const CheckResult& result) {
    QString output;
    
    // Выводим информацию по каждой функции
    output += "=== ПРИНАДЛЕЖНОСТЬ КЛАССАМ ПОСТА ===\n\n";
    
    for (size_t i = 0; i < result.perFunction.size(); i++) {
        output += formatFunctionInfo(result.perFunction[i], i + 1);
        output += "\n";
    }
    
    output += "=== ВЫВОД О ПОЛНОТЕ ===\n\n";
    output += result.explanation + "\n\n";
    
    if (result.isComplete) {
        output += "Набор функций является функционально полным.\n";
        if (result.isBasis) {
            output += "Набор является функционально полным базисом "
                     "(минимальным по числу функций).\n";
        } else {
            output += "Набор не является базисом, так как из него можно "
                     "удалить хотя бы одну функцию без потери полноты.\n";
        }
    } else {
        output += "Набор не является полным.\n\n";
        
        // Выводим непокрытые классы
        if (!result.uncoveredClasses.empty()) {
            output += "Непокрытые классы Поста:\n";
            for (const auto& className : result.uncoveredClasses) {
                output += "  • " + QString::fromStdString(className) + "\n";
            }
            output += "\n";
            output += "Для полноты набора необходимо добавить функции,\n";
            output += "не принадлежащие этим классам.\n";
        }
    }
    
    m_textEdit->setPlainText(output);
}

void ResultsWidget::clear() {
    m_textEdit->clear();
}

QString ResultsWidget::formatFunctionInfo(const FunctionPostInfo& info, int index) const {
    QString result;
    result += QString("Функция %1 (%2 переменных):\n").arg(index).arg(info.varCount);
    result += "  T0 (сохраняет 0): " + QString(info.isT0 ? "✔ ДА" : "✗ НЕТ") + "\n";
    result += "  T1 (сохраняет 1): " + QString(info.isT1 ? "✔ ДА" : "✗ НЕТ") + "\n";
    result += "  S  (самодвойственная): " + QString(info.isS ? "✔ ДА" : "✗ НЕТ") + "\n";
    result += "  M  (монотонная): " + QString(info.isM ? "✔ ДА" : "✗ НЕТ") + "\n";
    result += "  L  (линейная): " + QString(info.isL ? "✔ ДА" : "✗ НЕТ") + "\n";
    result += "  ---\n";
    return result;
}
