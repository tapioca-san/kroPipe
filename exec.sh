rm -fr build

./compilarShader.sh

mkdir build
cd build
cmake .. 
make
./nial