@echo off

call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\vc\bin\vcvars32.bat"

cmake -E remove_directory build
cmake -E make_directory  build

cd build

cmake -G "NMake Makefiles" .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=tmprel

jom all
ctest -V -T Test
cpack