if(TBB_INCLUDE_DIR AND TBB_LIBRARY)
  set(TBB_FIND_QUIETLY TRUE)
endif(TBB_INCLUDE_DIR AND TBB_LIBRARY)

find_library(TBB_LIBRARY_DEBUG tbb_debug PATHS ${PROJECT_BASE_DIR}/redist/tbb/lib)
find_library(TBB_LIBRARY_RELEASE tbb PATHS ${PROJECT_BASE_DIR}/redist/tbb/lib)
set(TBB_LIBRARY ${TBB_LIBRARY_RELEASE} debug ${TBB_LIBRARY_DEBUG} )

set(TBB_INCLUDE_DIR ${PROJECT_BASE_DIR}/redist/tbb/include)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Tbb DEFAULT_MSG TBB_LIBRARY TBB_INCLUDE_DIR)

MARK_AS_ADVANCED(
  TBB_INCLUDE_DIR
  TBB_LIBRARY
)
