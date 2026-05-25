#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo "=== Сборка PostCriteriaChecker для Linux ==="
echo "Корень проекта: $PROJECT_ROOT"

cd "$PROJECT_ROOT"

mkdir -p builds/linux
cd builds/linux

cmake "$PROJECT_ROOT" -DCMAKE_TOOLCHAIN_FILE="$PROJECT_ROOT/cmake/linux-toolchain.cmake"

if [ $? -ne 0 ]; then
    echo "Ошибка: Конфигурация CMake не удалась!"
    exit 1
fi

make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "Ошибка: Компиляция не удалась!"
    exit 1
fi

echo ""
echo "=========================================="
echo "Сборка для Linux завершена"
echo "Исполняемый файл: $PROJECT_ROOT/builds/linux/PostCriteriaChecker"
echo ""
echo "Для запуска:"
echo "  cd $PROJECT_ROOT/builds/linux && ./MLTA_2.1"
echo "=========================================="