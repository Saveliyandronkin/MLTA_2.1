#include "CompletenessChecker.h"
#include <set>
#include <string>

CheckResult CompletenessChecker::checkSet(const FunctionSet& set) {
    CheckResult result;
    
    // Собираем информацию по каждой функции
    for (const auto& func : set.getFunctions()) {
        FunctionPostInfo info;
        info.varCount = func.getVarCount();
        info.isT0 = PostClasses::isT0(func);
        info.isT1 = PostClasses::isT1(func);
        info.isS = PostClasses::isS(func);
        info.isM = PostClasses::isM(func);
        info.isL = PostClasses::isL(func);
        result.perFunction.push_back(info);
    }
    
    // Проверяем полноту и базис
    result.isComplete = isComplete(set);
    result.isBasis = isBasis(set);
    result.uncoveredClasses = getUncoveredClasses(set);
    
    // Формируем объяснение
    if (result.isComplete) {
        result.explanation = "Набор является полным";
        if (result.isBasis) {
            result.explanation += " и функционально полным базисом";
        } else {
            result.explanation += ", но не является базисом (избыточен)";
        }
    } else {
        result.explanation = "Набор не является полным. Согласно теореме Поста, "
                            "не выполнено условие покрытия всех пяти замкнутых классов.";
    }
    
    return result;
}

bool CompletenessChecker::isComplete(const FunctionSet& set) {
    return coversAllPostClasses(set);
}

bool CompletenessChecker::isBasis(const FunctionSet& set) {
    // Должен быть полным
    if (!isComplete(set)) {
        return false;
    }
    
    // Проверяем минимальность: удаление любой функции делает набор неполным
    for (size_t i = 0; i < set.size(); i++) {
        // Создаем набор без i-й функции
        FunctionSet subset;
        for (size_t j = 0; j < set.size(); j++) {
            if (j != i) {
                subset.addFunction(set.getFunctions()[j]);
            }
        }
        
        // Если поднабор все еще полный, значит текущий набор не базис
        if (isComplete(subset)) {
            return false;
        }
    }
    
    return true;
}

bool CompletenessChecker::coversAllPostClasses(const FunctionSet& set) {
    std::set<int> coveredClasses;
    
    for (const auto& func : set.getFunctions()) {
        if (!PostClasses::isT0(func)) coveredClasses.insert(0);
        if (!PostClasses::isT1(func)) coveredClasses.insert(1);
        if (!PostClasses::isS(func)) coveredClasses.insert(2);
        if (!PostClasses::isM(func)) coveredClasses.insert(3);
        if (!PostClasses::isL(func)) coveredClasses.insert(4);
    }
    
    // Должны покрыть все 5 классов
    return coveredClasses.size() == 5;
}

std::set<std::string> CompletenessChecker::getUncoveredClasses(const FunctionSet& set) {
    std::set<std::string> uncovered;
    
    // Проверяем каждый класс Поста
    bool hasNonT0 = false;
    bool hasNonT1 = false;
    bool hasNonS = false;
    bool hasNonM = false;
    bool hasNonL = false;
    
    for (const auto& func : set.getFunctions()) {
        if (!PostClasses::isT0(func)) hasNonT0 = true;
        if (!PostClasses::isT1(func)) hasNonT1 = true;
        if (!PostClasses::isS(func)) hasNonS = true;
        if (!PostClasses::isM(func)) hasNonM = true;
        if (!PostClasses::isL(func)) hasNonL = true;
    }
    
    if (!hasNonT0) uncovered.insert("T0 (сохраняет 0)");
    if (!hasNonT1) uncovered.insert("T1 (сохраняет 1)");
    if (!hasNonS) uncovered.insert("S (самодвойственная)");
    if (!hasNonM) uncovered.insert("M (монотонная)");
    if (!hasNonL) uncovered.insert("L (линейная)");
    
    return uncovered;
}
