#ifndef COMPLETENESSCHECKER_H
#define COMPLETENESSCHECKER_H

#include "FunctionSet.h"
#include "PostClasses.h"
#include <vector>
#include <string>
#include <set>

struct FunctionPostInfo {
    int varCount;
    bool isT0;
    bool isT1;
    bool isS;
    bool isM;
    bool isL;
};

struct CheckResult {
    std::vector<FunctionPostInfo> perFunction;
    bool isComplete;
    bool isBasis;
    std::string explanation;
    std::set<std::string> uncoveredClasses;
};

class CompletenessChecker {
public:
    // Проверка набора на полноту и базис
    CheckResult checkSet(const FunctionSet& set);
    
private:
    // Проверяет, является ли набор полным
    bool isComplete(const FunctionSet& set);
    
    // Проверяет, является ли набор базисом (полный и минимальный)
    bool isBasis(const FunctionSet& set);
    
    // Для каждой функции получаем множество классов, которым она НЕ принадлежит
    std::vector<int> getMissingClasses(const TruthTable& function);
    
    // Проверяет, покрывает ли набор все 5 классов Поста
    bool coversAllPostClasses(const FunctionSet& set);
    // Возвращает список непокрытых классов Поста
    std::set<std::string> getUncoveredClasses(const FunctionSet& set);
};

#endif // COMPLETENESSCHECKER_H
