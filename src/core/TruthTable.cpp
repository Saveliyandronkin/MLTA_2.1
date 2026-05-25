#include "TruthTable.h"
#include <stdexcept>
#include <algorithm>

TruthTable::TruthTable(int varCount, const std::vector<bool>& values)
    : m_varCount(varCount)
    , m_valid(false)
{
    if (varCount < 1 || varCount > 7) {
        throw std::invalid_argument("Number of variables must be between 1 and 7");
    }
    
    m_rowCount = 1 << varCount;
    
    if (values.size() != static_cast<size_t>(m_rowCount)) {
        throw std::invalid_argument("Values size does not match number of rows");
    }
    
    m_values = values;
    m_valid = true;
}

TruthTable::TruthTable(int varCount, const std::string& bitString)
    : m_varCount(varCount)
    , m_valid(false)
{
    if (varCount < 1 || varCount > 7) {
        throw std::invalid_argument("Number of variables must be between 1 and 7");
    }
    
    m_rowCount = 1 << varCount;
    
    if (bitString.length() != static_cast<size_t>(m_rowCount)) {
        throw std::invalid_argument("Bit string length does not match number of rows");
    }
    
    m_values.reserve(m_rowCount);
    for (char c : bitString) {
        if (c == '0') m_values.push_back(false);
        else if (c == '1') m_values.push_back(true);
        else throw std::invalid_argument("Invalid character in bit string (only '0' or '1')");
    }
    
    m_valid = true;
}

bool TruthTable::getValue(int rowIndex) const {
    if (rowIndex < 0 || rowIndex >= m_rowCount) {
        throw std::out_of_range("Row index out of range");
    }
    return m_values[rowIndex];
}

bool TruthTable::getValueByMask(uint32_t mask) const {
    // mask использует только младшие varCount бит
    uint32_t rowIndex = mask & ((1 << m_varCount) - 1);
    return m_values[rowIndex];
}

uint32_t TruthTable::getMaskForRow(int rowIndex) const {
    if (rowIndex < 0 || rowIndex >= m_rowCount) {
        throw std::out_of_range("Row index out of range");
    }
    return static_cast<uint32_t>(rowIndex);
}
