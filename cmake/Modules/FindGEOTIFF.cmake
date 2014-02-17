# - Find geotiff library
# Find the native geotiff includes and library
# This module defines
#  GEOTIFF_INCLUDE_DIR, where to find tiff.h, etc.
#  GEOTIFF_LIBRARIES, libraries to link against to use GEOTIFF.
#  GEOTIFF_FOUND, If false, do not try to use GEOTIFF.
# also defined, but not for general use are
#  GEOTIFF_LIBRARY, where to find the GEOTIFF library.

include(osgeo_hint)

FIND_PATH(GEOTIFF_INCLUDE_DIR geotiff.h
          PATHS /usr/include/geotiff /usr/include/libgeotiff /usr/local/include/libgeotiff
                /usr/local/include/geotiff ${OSGEO_ROOT}/include)

SET(GEOTIFF_NAMES ${GEOTIFF_NAMES} geotiff_i geotiff libgeotiff_i libgeotiff)
FIND_LIBRARY(GEOTIFF_LIBRARY NAMES ${GEOTIFF_NAMES} PATHS ${OSGEO_ROOT}/lib)

# handle the QUIETLY and REQUIRED arguments and set GEOTIFF_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GEOTIFF  DEFAULT_MSG  GEOTIFF_LIBRARY  GEOTIFF_INCLUDE_DIR)

IF(GEOTIFF_FOUND)
  SET( GEOTIFF_LIBRARIES ${GEOTIFF_LIBRARY} )
  message( STATUS "Found GEOTIFF..." )
else( GEOTIFF_FOUND )
  message( WARNING "Could not find GEOS" )
ENDIF(GEOTIFF_FOUND)

MARK_AS_ADVANCED(GEOTIFF_INCLUDE_DIR GEOTIFF_LIBRARY)
