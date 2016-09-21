cd RW.PackLib
make -f Makefile.OSX
cd ..
cd RW.GameLib
make -f Makefile.OSX
cd ..
cd PC
make -f Makefile.OSX
cd ..
./buildOSXApp.sh
./buildOSXDmg.sh
