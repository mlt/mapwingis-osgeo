# Locate SPATIALINDEX
#
# This module accepts the following environment variables:
#
#    SPATIALINDEX_DIR or SPATIALINDEX_ROOT - Specify the location of SPATIALINDEX
#
# This module defines the following CMake variables:
#
#    SPATIALINDEX_FOUND - True if libspatialindex is found
#    SPATIALINDEX_LIBRARY - A variable pointing to the SPATIALINDEX library
#    SPATIALINDEX_INCLUDE_DIR - Where to find the headers

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
# $SPATIALINDEXDIR is an environment variable that would
# correspond to the ./configure --prefix=$SPATIALINDEX_DIR
# used in building spatialindex.
#
# Created by Eric Wing. I'm not a spatialindex user, but OpenSceneGraph uses it
# for osgTerrain so I whipped this module together for completeness.
# I actually don't know the conventions or where files are typically
# placed in distros.
# Any real spatialindex users are encouraged to correct this (but please don't
# break the OS X framework stuff when doing so which is what usually seems
# to happen).

include(osgeo_hint)

# This makes the presumption that you are include spatialindex.h like
#
#include "spatialindex.h"

find_path(SPATIALINDEX_INCLUDE_DIR spatialindex.h
  HINTS
    ENV SPATIALINDEX_DIR
    ENV SPATIALINDEX_ROOT
  PATH_SUFFIXES
     spatialindex
  PATHS
      ${OSGEO_ROOT}/include
      ~/Library/Frameworks/spatialindex.framework/Headers
      /Library/Frameworks/spatialindex.framework/Headers
      /sw # Fink
      /opt/local # DarwinPorts
      /opt/csw # Blastwave
      /opt
)

if(UNIX)
    # Use spatialindex-config to obtain the library version (this should hopefully
    # allow us to -lspatialindex1.x.y where x.y are correct version)
    # For some reason, libspatialindex development packages do not contain
    # libspatialindex.so...
    find_program(SPATIALINDEX_CONFIG spatialindex-config
        HINTS
          ENV SPATIALINDEX_DIR
          ENV SPATIALINDEX_ROOT
        PATH_SUFFIXES bin
        PATHS
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
    )

    if(SPATIALINDEX_CONFIG)
        exec_program(${SPATIALINDEX_CONFIG} ARGS --libs OUTPUT_VARIABLE SPATIALINDEX_CONFIG_LIBS)
        if(SPATIALINDEX_CONFIG_LIBS)
            string(REGEX MATCHALL "-l[^ ]+" _spatialindex_dashl ${SPATIALINDEX_CONFIG_LIBS})
            string(REGEX REPLACE "-l" "" _spatialindex_lib "${_spatialindex_dashl}")
            string(REGEX MATCHALL "-L[^ ]+" _spatialindex_dashL ${SPATIALINDEX_CONFIG_LIBS})
            string(REGEX REPLACE "-L" "" _spatialindex_libpath "${_spatialindex_dashL}")
        endif()
    endif()
endif()

find_library(SPATIALINDEX_LIBRARY
  NAMES ${_spatialindex_lib} spatialindex_i spatialindex spatialindex-64 spatialindex_c-64
  HINTS
     ENV SPATIALINDEX_DIR
     ENV SPATIALINDEX_ROOT
     ${_spatialindex_libpath}
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
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SPATIALINDEX DEFAULT_MSG SPATIALINDEX_LIBRARY SPATIALINDEX_INCLUDE_DIR)

set(SPATIALINDEX_LIBRARIES ${SPATIALINDEX_LIBRARY})
set(SPATIALINDEX_INCLUDE_DIRS ${SPATIALINDEX_INCLUDE_DIR})

if( SPATIALINDEX_FOUND )
   if( NOT SPATIALINDEX_FIND_QUIETLY )
      message( STATUS "Found SpatialIndex..." )
   endif( NOT SPATIALINDEX_FIND_QUIETLY )
else( SPATIALINDEX_FOUND )
   if( NOT SPATIALINDEX_FIND_QUIETLY )
      message( WARNING "Could not find SpatialIndex" )
   endif( NOT SPATIALINDEX_FIND_QUIETLY )
endif( SPATIALINDEX_FOUND )
