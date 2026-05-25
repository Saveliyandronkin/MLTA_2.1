#ifndef FUNCTIONSET_H
#define FUNCTIONSET_H

#include "TruthTable.h"
#include <vector>
#include <stdexcept>

class FunctionSet {
public:
    FunctionSet() = default;
    explicit FunctionSet(const std::vector<TruthTable>& functions);
    
    void addFunction(const TruthTable& function);
    const std::vector<TruthTable>& getFunctions() const { return m_functions; }
    size_t size() const { return m_functions.size(); }
    bool empty() const { return m_functions.empty(); }
    
private:
    std::vector<TruthTable> m_functions;
};

#endif // FUNCTIONSET_H
