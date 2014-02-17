cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
project(MapWinGIS)

include(osgeo_hint)
if(EXISTS ${OSGEO_ROOT})
  SET(CMAKE_INSTALL_PREFIX ${OSGEO_ROOT})
endif()


SET(ENV{GDAL_ROOT} ${OSGEO_ROOT})

find_package(GDAL REQUIRED)
find_package(GEOS REQUIRED)
find_package(TIFF REQUIRED)
find_package(GEOTIFF REQUIRED)
find_package(SQLITE REQUIRED)
find_package(SpatialIndex REQUIRED)
find_package(SHAPELIB)# REQUIRED

include_directories(${GDAL_INCLUDE_DIR} ${GEOS_INCLUDE_DIR} ${SPATIALINDEX_INCLUDE_DIR})

set(MEMLEAK OFF CACHE BOOL "Use memory leak detection")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:forScope-")

include(runtime)
configure_msvc_runtime()

add_definitions(-D_WINDLL -D_USRDLL -DGEOS_NEW)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_BINARY_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}/Utilities
 ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/SpatialIndex
 ${CMAKE_CURRENT_SOURCE_DIR}/Control
 ${CMAKE_CURRENT_SOURCE_DIR}/Image
 ${CMAKE_CURRENT_SOURCE_DIR}/Shapefile
 ${CMAKE_CURRENT_SOURCE_DIR}/Tin
 ${CMAKE_CURRENT_SOURCE_DIR}/Tiles
 ${CMAKE_CURRENT_SOURCE_DIR}/Tiles/Providers
 ${CMAKE_CURRENT_SOURCE_DIR}/Tiles/Projections
 ${CMAKE_CURRENT_SOURCE_DIR}/SupportingLibraries/include/atlhttp
 "${CMAKE_CURRENT_SOURCE_DIR}/COM classes"
 ${CMAKE_CURRENT_SOURCE_DIR}/Processing
 ${CMAKE_CURRENT_SOURCE_DIR}/Structures
 ${CMAKE_CURRENT_SOURCE_DIR}/ShapeNetwork
 ${CMAKE_CURRENT_SOURCE_DIR}/Grid
 ${CMAKE_CURRENT_SOURCE_DIR}/Grid/fip
 ${CMAKE_CURRENT_SOURCE_DIR}/Drawing
)

set(MAPWINGIS_SRCS
"COM classes/Chart.cpp"
"COM classes/ChartField.cpp"
"COM classes/Charts.cpp"
"COM classes/ColorScheme.cpp"
"COM classes/EditShape.cpp"
"COM classes/ESRIGridManager.cpp"
"COM classes/Extents.cpp"
"COM classes/Field.cpp"
"COM classes/FieldStatOperations.cpp"
"COM classes/FileManager.cpp"
"COM classes/GeoProjection.cpp"
"COM classes/GlobalSettings.cpp"
"COM classes/Grid.cpp"
"COM classes/GridColorBreak.cpp"
"COM classes/GridColorScheme.cpp"
"COM classes/GridHeader.cpp"
"COM classes/Image.cpp"
"COM classes/LabelCategory.cpp"
"COM classes/LabelClass.cpp"
"COM classes/Labels.cpp"
"COM classes/LinePattern.cpp"
"COM classes/LineSegment.cpp"
"COM classes/Measuring.cpp"
"COM classes/PointClass.cpp"
"COM classes/Shape.cpp"
"COM classes/ShapeDrawingOptions.cpp"
"COM classes/Shapefile.cpp"
"COM classes/ShapefileCategories.cpp"
"COM classes/ShapefileCategory.cpp"
"COM classes/ShapefileColorBreak.cpp"
"COM classes/ShapefileColorScheme.cpp"
"COM classes/Shapefile_Edit.cpp"
"COM classes/Shapefile_Geoprocessing.cpp"
"COM classes/Shapefile_LabelsCharts.cpp"
"COM classes/Shapefile_Optimizations.cpp"
"COM classes/Shapefile_ReadWrite.cpp"
"COM classes/Shapefile_Selection.cpp"
"COM classes/Shapefile_SpatialIndex.cpp"
"COM classes/Shapefile_Validation.cpp"
"COM classes/ShapeValidationInfo.cpp"
"COM classes/ShapeNetwork.cpp"
"COM classes/TableClass.cpp"
"COM classes/TileProviders.cpp"
"COM classes/Tiles.cpp"
"COM classes/Tin.cpp"
"COM classes/Utils.cpp"
"COM classes/Utils_GDAL.cpp"
"COM classes/Utils_GridToImage.cpp"
"COM classes/Utils_OGR.cpp"
"COM classes/Vector.cpp"
Control/ErrorCodes.cpp
Control/GlobalVars.cpp
Control/Map.cpp
Control/Map_Core.cpp
Control/Map_Cursors.cpp
Control/Map_DispatchMap.cpp
Control/MapPpg.cpp
Control/MapTracker.cpp
Control/Map_Drawing.cpp
Control/Map_DrawingLayer.cpp
Control/Map_Events.cpp
Control/Map_ImageGrouping.cpp
Control/Map_Labels.cpp
Control/Map_Layer.cpp
Control/Map_Measuring.cpp
Control/Map_Obsolete.cpp
Control/Map_Properties.cpp
Control/Map_Projection.cpp
Control/Map_Scale.cpp
Control/Map_Scalebar.cpp
Control/Map_Serialization.cpp
Control/Map_Shapefile.cpp
Control/Map_Snapshot.cpp
Control/ToolTipEx.cpp
Drawing/BaseDrawer.cpp
Drawing/ChartDrawing.cpp
Drawing/CollisionList.cpp
Drawing/DrawingOptions.cpp
Drawing/ImageDrawing.cpp
Drawing/LabelDrawing.cpp
Drawing/LabelOptions.cpp
Drawing/LineDrawing.cpp
Drawing/PointSymbols.cpp
Drawing/RotatedRectangle.cpp
Drawing/ShapefileDrawing.cpp
Drawing/TilesDrawer.cpp
Grid/dGrid.cpp
Grid/dHeader.cpp
Grid/EsriDll.cpp
Grid/fGrid.cpp
Grid/fHeader.cpp
Grid/fip/a_toe.cpp
Grid/fip/bak_fld.cpp
Grid/fip/bak_rec.cpp
Grid/fip/bak_sfld.cpp
Grid/fip/beg_ddre.cpp
Grid/fip/beg_file.cpp
Grid/fip/beg_rec.cpp
Grid/fip/bldadscr.cpp
Grid/fip/bld_fmt.cpp
Grid/fip/bld_lab.cpp
Grid/fip/chk_fld.cpp
Grid/fip/chk_nfld.cpp
Grid/fip/chk_nrec.cpp
Grid/fip/chk_nsfl.cpp
Grid/fip/chk_rec.cpp
Grid/fip/chk_sfld.cpp
Grid/fip/ch_size.cpp
Grid/fip/cki_nfld.cpp
Grid/fip/conv_cat.cpp
Grid/fip/c_dddir.cpp
Grid/fip/c_ddlead.cpp
Grid/fip/c_drdir.cpp
Grid/fip/c_drlead.cpp
Grid/fip/deldrval.cpp
Grid/fip/del_dim.cpp
Grid/fip/del_fmt.cpp
Grid/fip/del_labs.cpp
Grid/fip/end_ddre.cpp
Grid/fip/end_file.cpp
Grid/fip/end_rec.cpp
Grid/fip/er_ddfld.cpp
Grid/fip/er_ddrec.cpp
Grid/fip/er_ddsfl.cpp
Grid/fip/e_toa.cpp
Grid/fip/free_lab.cpp
Grid/fip/gbstr.cpp
Grid/fip/gdstr.cpp
Grid/fip/get_dim.cpp
Grid/fip/get_dval.cpp
Grid/fip/get_fmt.cpp
Grid/fip/get_lvl.cpp
Grid/fip/gfstr.cpp
Grid/fip/gint.cpp
Grid/fip/gsstr.cpp
Grid/fip/gstr.cpp
Grid/fip/gt_adscr.cpp
Grid/fip/g_order.cpp
Grid/fip/incr_lab.cpp
Grid/fip/is_adscr.cpp
Grid/fip/is_intgr.cpp
Grid/fip/i_toa.cpp
Grid/fip/ld_ddrec.cpp
Grid/fip/ld_rec.cpp
Grid/fip/ld_tagp.cpp
Grid/fip/load_fld.cpp
Grid/fip/load_fmt.cpp
Grid/fip/load_lab.cpp
Grid/fip/l_tos.cpp
Grid/fip/out_fmt.cpp
Grid/fip/pop_rs.cpp
Grid/fip/push_rs.cpp
Grid/fip/rd_ddfld.cpp
Grid/fip/rd_ddrec.cpp
Grid/fip/rd_ddsfl.cpp
Grid/fip/rd_fld.cpp
Grid/fip/rd_rec.cpp
Grid/fip/rd_sfld.cpp
Grid/fip/ret_dv.cpp
Grid/fip/ret_fv.cpp
Grid/fip/ret_matc.cpp
Grid/fip/ret_pdm.cpp
Grid/fip/rt_pvfld.cpp
Grid/fip/rt_pvsfl.cpp
Grid/fip/r_dddir.cpp
Grid/fip/r_ddlead.cpp
Grid/fip/r_drdir.cpp
Grid/fip/r_drlead.cpp
Grid/fip/setup_lb.cpp
Grid/fip/set_stat.cpp
Grid/fip/stc_empt.cpp
Grid/fip/stor_dv.cpp
Grid/fip/str_tok.cpp
Grid/fip/s_tol.cpp
Grid/fip/uld_ddre.cpp
Grid/fip/uld_rec.cpp
Grid/fip/v_ddtag.cpp
Grid/fip/v_drtag.cpp
Grid/fip/wint.cpp
Grid/fip/wr_ddfld.cpp
Grid/fip/wr_ddrec.cpp
Grid/fip/wr_ddsfl.cpp
Grid/fip/wr_fld.cpp
Grid/fip/wr_rec.cpp
Grid/fip/wr_sfld.cpp
Grid/GenericGrid.cpp
Grid/GenericHeader.cpp
Grid/GridInterpolate.cpp
Grid/GridManager.cpp
Grid/lGrid.cpp
Grid/lHeader.cpp
Grid/RegistryKey.cpp
Grid/sgrid.cpp
Grid/sHeader.cpp
Grid/tkGridRaster.cpp
Image/colour.cpp
Image/cq.c
Image/ImageGroup.cpp
Image/ImageResamling.cpp
Image/RasterMatrix.cpp
Image/tkBitmap.cpp
Image/tkGif.cpp
Image/tkJpg.cpp
Image/tkRaster.cpp
MapWinGIS.cpp
Processing/Base64.cpp
Processing/clipper.cpp
Processing/ClipperConverter.cpp
Processing/Expression.cpp
Processing/GeograpicLib/Geodesic.cpp
Processing/GeograpicLib/GeodesicLine.cpp
Processing/GeograpicLib/PolygonArea.cpp
Processing/GeometryConverter.cpp
Processing/GeometryOperations.cpp
Processing/JenksBreaks.cpp
Processing/MapRotate.cpp
Processing/MeasuringTool.cpp
Processing/Projections.cpp
Processing/QTree.cpp
Shapefile/ActiveShape.cpp
Shapefile/ShapeData.cpp
Shapefile/ShapefileReader.cpp
Shapefile/ShapePoint.cpp
Shapefile/ShapeWrapper.cpp
Shapefile/ShapeWrapperCOM.cpp
Shapefile/TableRow.cpp
Shapefile/ShapeValidator.cpp
ShapeNetwork/graph.cpp
ShapeNetwork/heap.cpp
Tiles/DiskCache.cpp
Tiles/Projections/BaseProjection.cpp
Tiles/Projections/CustomProjection.cpp
Tiles/Providers/BaseProvider.cpp
Tiles/RamCache.cpp
Tiles/SQLiteCache.cpp
Tiles/TileCacher.cpp
Tiles/TileCore.cpp
Tiles/TileHelper.cpp
Tiles/TileLoader.cpp
Tin/point.cpp
Tin/point_table.cpp
Tin/table_row.cpp
Tin/TinHeap.cpp
Tin/triangle_table.cpp
Tin/vertex_table.cpp
Utilities/ColoringGraph.cpp
Utilities/cppVector.cpp
Utilities/Debugging/ReferenceCounter.cpp
Utilities/GdalHelper.cpp
Utilities/GeographLibHelper.cpp
Utilities/GeosHelper.cpp
Utilities/LineBresenham.cpp
Utilities/Logger.cpp
Utilities/Matrix.cpp
Utilities/SpatialIndex/IndexSearching.cpp
Utilities/SpatialIndex/IndexShapeFiles.cpp
Utilities/SpatialIndex/ShapeFileStream.cpp
Utilities/UtilityFunctions.cpp
Utilities/varH.cpp
Utilities/vertex.cpp
Utilities/XRedBlackTree.cpp
Utilities/YRedBlackTree.cpp
MapWinGIS.tlb
MapWinGIS.rc
MapWinGIS_i.c
)

if(MEMLEAK)
  add_definition(-DMEMLEAK)
  set(MAPWINGIS_SRCS ${MAPWINGIS_SRCS} Utilities/Debugging/MemLeakDetect.cpp)
endif()


set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h" OBJECT_OUTPUTS "stdafx.pch")
set_source_files_properties(${MAPWINGIS_SRCS} PROPERTIES OBJECT_DEPENDS "stdafx.pch")
set_source_files_properties(MapWinGIS_i.c Image/cq.c PROPERTIES COMPILE_FLAGS "/Y-")

if(NOT SHAPELIB_LIBRARY)
  message("Using bundled ShapeLib fragment")
  add_library(SHAPELIB STATIC Shapefile/dbfopen.cpp)
  set(SHAPELIB SHAPELIB)
else()
  set(SHAPELIB ${SHAPELIB_LIBRARY})
endif()

# Image/tkTiff.cpp
# Utilities/RegistryKey.cpp
# Utilities/Templates.cpp
# Utilities/Debugging/MemLeakDetect.cpp

add_library(MapWinGIS MODULE StdAfx.cpp ${MAPWINGIS_SRCS})
target_link_libraries(MapWinGIS
                      ${GDAL_LIBRARY} ${GEOS_LIBRARY}
                      ${SPATIALINDEX_LIBRARY} ${SQLITE_LIBRARY}
                      ${TIFF_LIBRARY} ${GEOTIFF_LIBRARY}
                      ${SHAPELIB}
                      wininet.lib GdiPlus.lib Version.lib)
SET_TARGET_PROPERTIES(MapWinGIS PROPERTIES SUFFIX ".ocx"
  DEBUG_POSTFIX _d
  COMPILE_FLAGS "/Yustdafx.h"
  LINK_FLAGS "/MANIFEST:NO \"/DEF:${CMAKE_SOURCE_DIR}/MapWinGISps.def\""
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OSGEO_ROOT}/bin"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OSGEO_ROOT}/bin"
)

add_custom_command(
  TARGET MapWinGIS POST_BUILD
  COMMAND mt.exe -nologo -tlb:${CMAKE_CURRENT_BINARY_DIR}/MapWinGIS.tlb -dll:$<TARGET_FILE_NAME:MapWinGIS> -outputresource:$<TARGET_FILE:MapWinGIS>\;\#2
  COMMENT "Embedding registration free manifest"
)

add_custom_command(
  OUTPUT MapWinGIS.tlb MapWinGIS_i.c MapWinGIS_i.h MapWinGIS_p.c dlldata.c
  COMMAND midl.exe /header MapWinGIS_i.h ${CMAKE_CURRENT_SOURCE_DIR}/MapWinGIS.odl
  DEPENDS MapWinGIS.odl
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating type library from ODL"
)

export(TARGETS MapWinGIS FILE MapWinGIS-config.cmake)
export(PACKAGE MapWinGIS)
