RMDIR /S /Q build\
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -GNinja
cmake --build build --config Release -j