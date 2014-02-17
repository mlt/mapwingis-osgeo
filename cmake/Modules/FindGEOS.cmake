#---
# File: FindGEOS.cmake
#
# Find the native GEOS(Geometry Engine - Open Source) includes and libraries.
#
# This module defines:
#
# GEOS_INCLUDE_DIR, where to find geos.h, etc.
# GEOS_LIBRARY, libraries to link against to use GEOS.  Currently there are
# two looked for, geos and geos_c libraries.
# GEOS_FOUND, True if found, false if one of the above are not found.
# 
# For ossim, typically geos will be system installed which should be found; 
# or found in the ossim 3rd party dependencies directory from a geos build 
# and install.  If the latter it will rely on CMAKE_INCLUDE_PATH and 
# CMAKE_LIBRARY_PATH having the path to the party dependencies directory.
# 
# NOTE: 
# This script is specialized for ossim, e.g. looking in /usr/local/ossim.
#
# $Id$
#---

include(osgeo_hint)
#---
# Find include path:
# Note: Version < 3.3.0 do not have geos.h in the geos sub directory; hence,
# the check for both "geos/geos.h" and "geos.h".
#---
find_path( GEOS_INCLUDE_DIR geos/geos.h geos.h geos_c.h
           PATHS 
           ${OSGEO_ROOT}/include
           /usr/include
           /usr/local/include
           /usr/local/ossim/include )

# Find GEOS library:
find_library( GEOS_LIB NAMES geos 
              PATHS 
              /usr/lib64 
              /usr/lib 
              /usr/local/lib
              /usr/local/ossim/lib )

# Find GEOS C library:
find_library( GEOS_C_LIB NAMES geos_c 
              PATHS 
              ${OSGEO_ROOT}/lib
              /usr/lib64 
              /usr/lib 
              /usr/local/lib
              /usr/local/ossim/lib )

# Set the GEOS_LIBRARY:
if( GEOS_C_LIB )
   set( GEOS_LIBRARY ${GEOS_C_LIB} CACHE STRING INTERNAL )
endif(GEOS_C_LIB )

#---
# This function sets GEOS_FOUND if variables are valid.
#--- 
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args( GEOS DEFAULT_MSG 
                                   GEOS_LIBRARY 
                                   GEOS_INCLUDE_DIR )

if( GEOS_FOUND )
   if( NOT GEOS_FIND_QUIETLY )
      message( STATUS "Found GEOS..." )
   endif( NOT GEOS_FIND_QUIETLY )
else( GEOS_FOUND )
   if( NOT GEOS_FIND_QUIETLY )
      message( WARNING "Could not find GEOS" )
   endif( NOT GEOS_FIND_QUIETLY )
endif( GEOS_FOUND )

if( NOT GEOS_FIND_QUIETLY )
   message( STATUS "GEOS_INCLUDE_DIR=${GEOS_INCLUDE_DIR}" )
   message( STATUS "GEOS_LIBRARY=${GEOS_LIBRARY}" )
endif( NOT GEOS_FIND_QUIETLY )
