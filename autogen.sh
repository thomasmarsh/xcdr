aclocal && automake && autoconf
flags="-Wall -g -O3 -pipe -march=athlon-xp -falign-functions=4 -fomit-frame-pointer -mfancy-math-387"
export CFLAGS=$flags
export CXXFLAGS=$flags
./configure -C --disable-shared
