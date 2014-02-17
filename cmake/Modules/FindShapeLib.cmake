# Locate SHAPELIB
#
# This module accepts the following environment variables:
#
#    SHAPELIB_DIR or SHAPELIB_ROOT - Specify the location of SHAPELIB
#
# This module defines the following CMake variables:
#
#    SHAPELIB_FOUND - True if libshapelib is found
#    SHAPELIB_LIBRARY - A variable pointing to the SHAPELIB library
#    SHAPELIB_INCLUDE_DIR - Where to find the headers

#=============================================================================
# Copyright 2007-2009 Kitware, Inc.
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

#
# $SHAPELIBDIR is an environment variable that would
# correspond to the ./configure --prefix=$SHAPELIB_DIR
# used in building shapelib.
#
# Created by Eric Wing. I'm not a shapelib user, but OpenSceneGraph uses it
# for osgTerrain so I whipped this module together for completeness.
# I actually don't know the conventions or where files are typically
# placed in distros.
# Any real shapelib users are encouraged to correct this (but please don't
# break the OS X framework stuff when doing so which is what usually seems
# to happen).

include(osgeo_hint)

# This makes the presumption that you are include shapefil.h like
#
#include "shapefil.h"

find_path(SHAPELIB_INCLUDE_DIR shapefil.h
  HINTS
    ENV SHAPELIB_DIR
    ENV SHAPELIB_ROOT
  PATHS
      ${OSGEO_ROOT}/include
      ~/Library/Frameworks/shapelib.framework/Headers
      /Library/Frameworks/shapelib.framework/Headers
      /sw # Fink
      /opt/local # DarwinPorts
      /opt/csw # Blastwave
      /opt
)

if(UNIX)
    # Use shapelib-config to obtain the library version (this should hopefully
    # allow us to -lshapelib1.x.y where x.y are correct version)
    # For some reason, libshapelib development packages do not contain
    # libshapelib.so...
    find_program(SHAPELIB_CONFIG shapelib-config
        HINTS
          ENV SHAPELIB_DIR
          ENV SHAPELIB_ROOT
        PATH_SUFFIXES bin
        PATHS
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
    )

    if(SHAPELIB_CONFIG)
        exec_program(${SHAPELIB_CONFIG} ARGS --libs OUTPUT_VARIABLE SHAPELIB_CONFIG_LIBS)
        if(SHAPELIB_CONFIG_LIBS)
            string(REGEX MATCHALL "-l[^ ]+" _shapelib_dashl ${SHAPELIB_CONFIG_LIBS})
            string(REGEX REPLACE "-l" "" _shapelib_lib "${_shapelib_dashl}")
            string(REGEX MATCHALL "-L[^ ]+" _shapelib_dashL ${SHAPELIB_CONFIG_LIBS})
            string(REGEX REPLACE "-L" "" _shapelib_libpath "${_shapelib_dashL}")
        endif()
    endif()
endif()

find_library(SHAPELIB_LIBRARY
  NAMES ${_shapelib_lib} shapelib_i
  HINTS
     ENV SHAPELIB_DIR
     ENV SHAPELIB_ROOT
     ${_shapelib_libpath}
  PATH_SUFFIXES lib
  PATHS
    ${OSGEO_ROOT}/lib
    /sw
    /opt/local
    /opt/csw
    /opt
    /usr/freeware
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SHAPELIB DEFAULT_MSG SHAPELIB_LIBRARY SHAPELIB_INCLUDE_DIR)

set(SHAPELIB_LIBRARIES ${SHAPELIB_LIBRARY})
set(SHAPELIB_INCLUDE_DIRS ${SHAPELIB_INCLUDE_DIR})

if( SHAPELIB_FOUND )
   if( NOT SHAPELIB_FIND_QUIETLY )
      message( STATUS "Found ShapeLib..." )
   endif( NOT SHAPELIB_FIND_QUIETLY )
else( SHAPELIB_FOUND )
   if( NOT SHAPELIB_FIND_QUIETLY )
      message( WARNING "Could not find ShapeLib" )
   endif( NOT SHAPELIB_FIND_QUIETLY )
endif( SHAPELIB_FOUND )
