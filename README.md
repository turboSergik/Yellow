# Yellow

## Список участников

* Сергей Пискун
* Павел Амельков
* Олег Бобров

## Инструкция по установке

Для сборки необходимы conan и cmake.
Сборка не отличается от сборки с помощью cmake, за исключением того что cmake вызывает conan чтобы разрешить зависимости, и потому ему нужно явно указать DCMAKE\_BUILD\_TYPE чтобы cmake передал его в build_type canon. Таким образом сборка выглядит так:

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

Параметр --config Release необходим для компилятора Visual Studio и должен совпадать с CMAKE\_BUILD\_TYPE. Для других компиляторов он игнорируется. В репозитории есть build.bat и build.sh которые делают именно так собирают проект.

