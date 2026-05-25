#include "FunctionSet.h"

FunctionSet::FunctionSet(const std::vector<TruthTable>& functions)
    : m_functions(functions)
{
    if (m_functions.size() > 5) {
        throw std::invalid_argument("Function set can contain at most 5 functions");
    }
}

void FunctionSet::addFunction(const TruthTable& function) {
    if (m_functions.size() >= 5) {
        throw std::invalid_argument("Cannot add more than 5 functions");
    }
    m_functions.push_back(function);
}
