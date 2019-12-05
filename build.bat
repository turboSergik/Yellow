mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
:: --config for visual studio.  
:: It don't breaks mingw build
cmake --build . --config Release
cd ..

