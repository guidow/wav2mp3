# - Find LAME
# Find the native LAME headers and libraries.
#
#  LAME_INCLUDE_DIRS - where to find lame/lame.h, etc.
#  LAME_LIBRARIES    - List of libraries when using lame.
#  LAME_FOUND        - True if lame found.

# Look for the header file.
find_path(LAME_INCLUDE_DIR NAMES lame/lame.h)
#mark_as_advanced(LAME_INCLUDE_DIR)

# Look for the library.
find_library(LAME_LIBRARY NAMES libmp3lame.a mp3lame)
#mark_as_advanced(LAME_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set LAME_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LAME DEFAULT_MSG LAME_LIBRARY LAME_INCLUDE_DIR)

if(LAME_FOUND)
	set(LAME_LIBRARIES ${LAME_LIBRARY})
	set(LAME_INCLUDE_DIRS ${LAME_INCLUDE_DIR})
else(LAME_FOUND)
	set(LAME_LIBRARIES)
	set(LAME_INCLUDE_DIRS)
endif (LAME_FOUND)
