conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan 
conan install . --build=missing --install-folder=build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - MinGW Makefiles"
cmake --build .
cd ..

