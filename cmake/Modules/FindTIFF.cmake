# - Find TIFF library
# Find the native TIFF includes and library
# This module defines
#  TIFF_INCLUDE_DIR, where to find tiff.h, etc.
#  TIFF_LIBRARIES, libraries to link against to use TIFF.
#  TIFF_FOUND, If false, do not try to use TIFF.
# also defined, but not for general use are
#  TIFF_LIBRARY, where to find the TIFF library.

#=============================================================================
# Copyright 2002-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

include(osgeo_hint)

find_path(TIFF_INCLUDE_DIR tiff.h
          PATHS ${OSGEO_ROOT}/include)

set(TIFF_NAMES ${TIFF_NAMES} tiff libtiff libtiff_i tiff3 libtiff3)
find_library(TIFF_LIBRARY NAMES ${TIFF_NAMES}
             PATHS ${OSGEO_ROOT}/lib)

if(TIFF_INCLUDE_DIR AND EXISTS "${TIFF_INCLUDE_DIR}/tiffvers.h")
    file(STRINGS "${TIFF_INCLUDE_DIR}/tiffvers.h" tiff_version_str
         REGEX "^#define[\t ]+TIFFLIB_VERSION_STR[\t ]+\"LIBTIFF, Version .*")

    string(REGEX REPLACE "^#define[\t ]+TIFFLIB_VERSION_STR[\t ]+\"LIBTIFF, Version +([^ \\n]*).*"
           "\\1" TIFF_VERSION_STRING "${tiff_version_str}")
    unset(tiff_version_str)
endif()

# handle the QUIETLY and REQUIRED arguments and set TIFF_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TIFF
                                  REQUIRED_VARS TIFF_LIBRARY TIFF_INCLUDE_DIR
                                  VERSION_VAR TIFF_VERSION_STRING)

if(TIFF_FOUND)
  set( TIFF_LIBRARIES ${TIFF_LIBRARY} )
endif()

mark_as_advanced(TIFF_INCLUDE_DIR TIFF_LIBRARY)
