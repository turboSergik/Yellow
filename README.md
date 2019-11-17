# Yellow

## Список участников

* Сергей Пискун
* Павел Амельков
* Олег Бобров

## Инструкция по установке

Необходимо создать Build директорию, зайти в нее, добавить remote и разрешить зависимости с помощью canon. Если в conan profile используется компилятор g++, то важно чтобы в settings было compiler.libcxx=libstdc++11

```
canon remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install . --build=missing --install-folder=build
```

Далее необходимо собрать проект с помощью cmake

Пример сборки для Unix Makefiles

```
cd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Для автоматизации этих действий добавлены build.bat и build.sh которые делают то же самое, используя в качестве генератора cmake Mingw Makefiles и Unix Makefiles соответственно

