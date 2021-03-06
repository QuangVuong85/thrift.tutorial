echo "Building client tutorial math..."
cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=/usr/bin/cc \
  -DCMAKE_CXX_COMPILER=/usr/bin/c++ \
  -G "CodeBlocks - Unix Makefiles" $(pwd) \
  -Bbuild

cmake \
  --build $(pwd)/build \
  --target client-thrift -- -j 4

cp $(pwd)/build/client-thrift .
sleep 2
rm -rf build/
echo "DONE!"
