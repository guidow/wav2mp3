# - Find Phreads4W
# Find the Pthreads4W headers and libraries.
#
#  PTHREADS4W_INCLUDE_DIRS - where to find pthread.h.
#  PTHREADS4W_LIBRARIES    - List of libraries when using pthreads4w.
#  PTHREADS4W_FOUND        - True if pthreads4w found.

# Look for the header file.
find_path(PTHREADS4W_INCLUDE_DIR NAMES pthread.h PATHS "C:\\pthreads4w")

# Look for the library.
find_library(PTHREADS4W_LIBRARY NAMES pthreadVC3 PATHS "C:\\pthreads4w")

# handle the QUIETLY and REQUIRED arguments and set LAME_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pthreads4W DEFAULT_MSG PTHREADS4W_LIBRARY PTHREADS4W_INCLUDE_DIR)

if(PTHREADS4W_FOUND)
	set(PTHREADS4W_LIBRARIES ${PTHREADS4W_LIBRARY})
	set(PTHREADS4W_INCLUDE_DIRS ${PTHREADS4W_INCLUDE_DIR})
else(PTHREADS4W_FOUND)
	set(PTHREADS4W_LIBRARIES)
	set(PTHREADS4W_INCLUDE_DIRS)
endif (PTHREADS4W_FOUND)
