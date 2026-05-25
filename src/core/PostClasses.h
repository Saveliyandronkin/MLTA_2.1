#ifndef POSTCLASSES_H
#define POSTCLASSES_H

#include "TruthTable.h"

namespace PostClasses {
    
    // T0: сохраняет 0 (f(0,0,...,0) = 0)
    bool isT0(const TruthTable& table);
    
    // T1: сохраняет 1 (f(1,1,...,1) = 1)
    bool isT1(const TruthTable& table);
    
    // S: самодвойственная (f(~x) = ~f(x))
    bool isS(const TruthTable& table);
    
    // M: монотонная (a ≤ b => f(a) ≤ f(b))
    bool isM(const TruthTable& table);
    
    // L: линейная (полином Жегалкина содержит только константу и одиночные переменные)
    bool isL(const TruthTable& table);
    
    // Вспомогательная функция: инверсия битов для n-битных чисел
    uint32_t bitwiseNot(uint32_t mask, int bitCount);
    
} // namespace PostClasses

#endif // POSTCLASSES_H
