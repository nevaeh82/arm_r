# Include only specification.
# provides targetd dow downloading runtime redistributable.


get-mingw:
	svn co https://svn.dfpost.ru/svn/3rdparty/qt-mingw/trunk/redist/Release/ $(SolutionDir)/build/qt-mingw
	svn co https://svn.dfpost.ru/svn/3rdparty/mingw/trunk/redist/bin/        $(SolutionDir)/build/mingw-redist 

get-msvc:
	svn co https://svn.dfpost.ru/svn/3rdparty/msvc/trunk/redist/             $(SolutionDir)/build/msvc-redist
	svn co https://svn.dfpost.ru/svn/3rdparty/qt-msvc/trunk/redist/release/  $(SolutionDir)/build/qt-msvc

