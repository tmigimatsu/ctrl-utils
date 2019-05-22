#!/bin/bash

mkdir -p external
cd external
git clone https://github.com/cpp-redis/cpp_redis.git --branch 4.3.1 --single-branch cpp_redis.git
cd cpp_redis.git
sed -i.old -E "s/^GENERATE_TAGFILE( +)=$/GENERATE_TAGFILE\1= cpp_redis.tag/" .doxygen
doxygen .doxygen
cd ..
cd ..

cd doc
doxygen Doxyfile
