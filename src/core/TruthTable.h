#ifndef TRUTHTABLE_H
#define TRUTHTABLE_H

#include <vector>
#include <string>
#include <cstdint>

class TruthTable {
public:
    // Конструктор: количество переменных (1-7) и значения функции
    TruthTable(int varCount, const std::vector<bool>& values);
    
    // Конструктор из строки битов (например "0101")
    TruthTable(int varCount, const std::string& bitString);
    
    // Геттеры
    int getVarCount() const { return m_varCount; }
    int getRowCount() const { return m_rowCount; }
    const std::vector<bool>& getValues() const { return m_values; }
    
    // Получить значение по индексу строки (0..2^n-1)
    bool getValue(int rowIndex) const;
    
    // Получить значение по набору переменных (в виде битовой маски)
    bool getValueByMask(uint32_t mask) const;
    
    // Получить набор переменных для строки (битовая маска)
    uint32_t getMaskForRow(int rowIndex) const;
    
    // Проверка корректности
    bool isValid() const { return m_valid; }
    
private:
    int m_varCount;          // количество переменных (1-7)
    int m_rowCount;          // = 1 << varCount
    std::vector<bool> m_values; // значения функции
    bool m_valid;
};

#endif // TRUTHTABLE_H
