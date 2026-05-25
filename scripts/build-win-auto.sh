#!/bin/bash

cd "$(dirname "$0")/.."

rm -rf builds/win-auto
mkdir -p builds/win-auto
cd builds/win-auto

echo "=== Конфигурация CMake ==="
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../cmake/mingw64-toolchain.cmake

echo "=== Компиляция ==="
make 

echo "=== Определение зависимостей DLL ==="
MINGW_BIN="/usr/x86_64-w64-mingw32/sys-root/mingw/bin"

find_dlls() {
    local exe_file="$1"
    local found_dlls="$2"
    
    local dll_list=$(x86_64-w64-mingw32-objdump -p "$exe_file" 2>/dev/null | grep "DLL Name" | awk '{print $3}')
    
    for dll in $dll_list; do
        if [[ "$dll" =~ ^(KERNEL32|USER32|GDI32|ADVAPI32|SHELL32|OLE32|OLEAUT32|COMCTL32|COMDLG32|WS2_32|VERSION|SETUPAPI|IMM32|MSVCRT|CRT|UCRTBASE|WINMM) ]]; then
            continue
        fi
        
        if echo "$found_dlls" | grep -q "$dll"; then
            continue
        fi
        
        if [ -f "$MINGW_BIN/$dll" ]; then
            echo "  Найдена: $dll"
            cp "$MINGW_BIN/$dll" . 2>/dev/null
            found_dlls="$found_dlls $dll"
            found_dlls=$(find_dlls "$MINGW_BIN/$dll" "$found_dlls")
        fi
    done
    
    echo "$found_dlls"
}

cp MLTA_2.1.exe .

echo "Поиск всех зависимых DLL..."
ALL_DLLS=$(find_dlls "MLTA_2.1.exe" "")

echo "=== Копирование плагинов Qt ==="
MINGW_PLUGINS="/usr/x86_64-w64-mingw32/sys-root/mingw/lib/qt6/plugins"

mkdir -p platforms
if [ -f "$MINGW_PLUGINS/platforms/qwindows.dll" ]; then
    cp "$MINGW_PLUGINS/platforms/qwindows.dll" platforms/
    echo "  ✓ platforms/qwindows.dll"
else
    ALT_PLUGINS="/usr/x86_64-w64-mingw32/sys-root/mingw/plugins"
    if [ -f "$ALT_PLUGINS/platforms/qwindows.dll" ]; then
        cp "$ALT_PLUGINS/platforms/qwindows.dll" platforms/
        echo "  ✓ platforms/qwindows.dll"
    else
        echo "  ⚠ Предупреждение: qwindows.dll не найден"
    fi
fi


echo ""
echo "=========================================="
echo "Сборка завершена!"
echo "Папка: $(pwd)"
echo ""
echo "Структура для Windows:"
ls -la
echo ""
echo "=========================================="
echo "ВАЖНО: Скопируйте на Windows всю папку целиком:"
echo "  $(pwd)"
echo ""
echo "На Windows структура должна быть сохранена:"
echo "  Ваша_папка/"
echo "    ├── MLTA_2.1.exe"
echo "    ├── *.dll"
echo "    └── platforms/"
echo "        └── qwindows.dll"
echo "=========================================="