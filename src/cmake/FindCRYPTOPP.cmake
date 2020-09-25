# - Find cryptopp
# Find the cryptopp cryptographic library
#
# This module defines the following variables:
#   CRYPTOPP_FOUND  -  True if library and include directory are found
# If set to TRUE, the following are also defined:
#   CRYPTOPP_INCLUDE_DIRS  -  The directory where to find the header file
#   CRYPTOPP_LIBRARIES  -  Where to find the library file
#
# For conveniance, these variables are also set. They have the same values
# than the variables above.  The user can thus choose his/her prefered way
# to write them.
#   CRYPTOPP_LIBRARY
#   CRYPTOPP_INCLUDE_DIR
#
# This file is in the public domain

include(FindPkgConfig)

if(NOT CRYPTOPP_FOUND)
  pkg_check_modules(CRYPTOPP cryptopp)
endif()

if(NOT CRYPTOPP_FOUND)
  find_path(CRYPTOPP_INCLUDE_DIRS NAMES cryptlib.h
      PATH_SUFFIXES cryptopp
      DOC "The cryptopp include directory")

  find_library(CRYPTOPP_LIBRARIES NAMES cryptopp
      DOC "The cryptopp library")

  # Use some standard module to handle the QUIETLY and REQUIRED arguments, and
  # set BOTAN_FOUND to TRUE if these two variables are set.
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(CRYPTOPP REQUIRED_VARS CRYPTOPP_LIBRARIES CRYPTOPP_INCLUDE_DIRS)

  if(CRYPTOPP_FOUND)
    set(CRYPTOPP_LIBRARY ${CRYPTOPP_LIBRARIES} CACHE INTERNAL "")
    set(CRYPTOPP_INCLUDE_DIR ${CRYPTOPP_INCLUDE_DIRS} CACHE INTERNAL "")
    set(CRYPTOPP_FOUND ${CRYPTOPP_FOUND} CACHE INTERNAL "")
  endif()
endif()

mark_as_advanced(CRYPTOPP_INCLUDE_DIRS CRYPTOPP_LIBRARIES)
