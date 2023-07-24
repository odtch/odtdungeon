mkdir -p external
cd external 
if [ ! -d "stb" ]; then
  git clone https://github.com/nothings/stb.git
fi
cd ..

