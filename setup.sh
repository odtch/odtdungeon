mkdir -p external
cd external 
if [ ! -d "stb" ]; then
  git clone https://github.com/nothings/stb.git
fi
if [ ! -d "JoltPhysics" ]; then
  git clone https://github.com/jrouwe/JoltPhysics.git
  cd JoltPhysics
  cd Build
  ./cmake_linux_clang_gcc.sh
  cd Linux_Debug
  make -j$(nproc) && ./UnitTests
  cd ..
  cd ..
  cd ..
fi
cd ..

