# Билд

Для билда делаем 
```
cmake -S . -B build --target all && cmake --build build --target all
```

Если мы на macos, то расстраиваемся... ставим через brew новый flex, новый bison и перед билдом:
```
export CMAKE_INCLUDE_PATH="/opt/homebrew/opt/flex/include"
export CMAKE_LIBRARY_PATH="/opt/homebrew/opt/flex/lib;/opt/homebrew/opt/bison/lib"
export PATH="/opt/homebrew/opt/flex/bin:/opt/homebrew/opt/bison/bin:$PATH"
```

# Тесты

Важно склонировать репозиторий с опцией `--recurse-submodules`. Иначе гугле тесты работать не будут.

Для запуска тестов:
```
./build/tests/runUnitTests 
```

# Структура

*  Код структуры лежит в `include/db` (`src/db`)

*  Код парсера flex/bison в `parser`

* Тесты лежат в `tests`