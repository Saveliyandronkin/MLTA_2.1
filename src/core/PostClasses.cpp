#include "PostClasses.h"
#include <vector>
#include <functional>

namespace PostClasses {

bool isT0(const TruthTable& table) {
    // Набор 0..0 имеет индекс 0
    return !table.getValue(0);
}

bool isT1(const TruthTable& table) {
    // Набор 1..1 имеет последний индекс (2^n - 1)
    return table.getValue(table.getRowCount() - 1);
}

uint32_t bitwiseNot(uint32_t mask, int bitCount) {
    uint32_t bitMask = (1 << bitCount) - 1;
    return (~mask) & bitMask;
}

bool isS(const TruthTable& table) {
    int n = table.getVarCount();
    int rowCount = table.getRowCount();
    
    for (int i = 0; i < rowCount; i++) {
        uint32_t mask = static_cast<uint32_t>(i);
        uint32_t invertedMask = bitwiseNot(mask, n);
        int invertedIndex = static_cast<int>(invertedMask);
        
        // Проверяем f(~x) = ~f(x)
        bool f_x = table.getValue(i);
        bool f_notx = table.getValue(invertedIndex);
        
        if (f_x == f_notx) {
            // Должно быть f_notx != f_x, т.е. f_x != f_notx
            return false;
        }
        
        // Проходим только до половины, чтобы не проверять дважды
        if (invertedIndex <= i) {
            // Можно оптимизировать, но для простоты проверим все пары
        }
    }
    
    return true;
}

bool isM(const TruthTable& table) {
    int n = table.getVarCount();
    int rowCount = table.getRowCount();
    
    // Проверяем все пары a и b, где a <= b покомпонентно
    for (int a = 0; a < rowCount; a++) {
        for (int b = 0; b < rowCount; b++) {
            // Проверяем, что a <= b (покомпонентно)
            bool a_le_b = true;
            for (int bit = 0; bit < n; bit++) {
                bool a_bit = (a >> bit) & 1;
                bool b_bit = (b >> bit) & 1;
                if (a_bit && !b_bit) {
                    a_le_b = false;
                    break;
                }
            }
            
            if (a_le_b) {
                // Если a <= b, то должно быть f(a) <= f(b)
                bool f_a = table.getValue(a);
                bool f_b = table.getValue(b);
                if (f_a && !f_b) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool isL(const TruthTable& table) {
    // Проверка линейности через построение полинома Жегалкина
    // Используем метод треугольника (преобразование таблицы в полином)
    
    int n = table.getVarCount();
    int rowCount = table.getRowCount();
    
    // Копируем значения таблицы в вектор целых (0/1)
    std::vector<int> poly(rowCount);
    for (int i = 0; i < rowCount; i++) {
        poly[i] = table.getValue(i) ? 1 : 0;
    }
    
    // Преобразование Уолша-Адамара (треугольник)
    // для получения коэффициентов полинома Жегалкина
    for (int step = 1; step < rowCount; step <<= 1) {
        for (int i = 0; i < rowCount; i += (step << 1)) {
            for (int j = 0; j < step; j++) {
                int idx1 = i + j;
                int idx2 = i + j + step;
                int val1 = poly[idx1];
                int val2 = poly[idx2];
                poly[idx2] = (val1 + val2) % 2;
            }
        }
    }
    
    // Проверяем: линейная функция, если нет членов степени >= 2
    // Коэффициенты для всех наборов с весом Хэмминга > 1 должны быть 0
    for (int i = 0; i < rowCount; i++) {
        int hammingWeight = __builtin_popcount(i); // GCC/Clang, для MSVC используй другой метод
        if (hammingWeight >= 2 && poly[i] != 0) {
            return false;
        }
    }
    
    return true;
}

} // namespace PostClasses
