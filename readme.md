this project records my learning process of c language

##
###
#### cmake https://github.com/Modern-CMake-CN/Modern-CMake-zh_CN
#### cjson https://github.com/DaveGamble/cJSON.git
#### jemalloc
wget https://github.com/jemalloc/jemalloc/releases/download/5.3.0/jemalloc-5.3.0.tar.bz2

tar -jxvf jemalloc-5.3.0
cd jemalloc-5.3.0
./configure
make
make install

jemalloc还可以通过环境变量配置来满足特殊要求， 如：
export MALLOC_CONF="background_thread:true"
