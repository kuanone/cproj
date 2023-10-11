cmake -S . -B  build -D BUILD_LOGGING=ON -D FLAG_NO=ON
cd build
make
./cproj -v
./cproj -h
./cproj