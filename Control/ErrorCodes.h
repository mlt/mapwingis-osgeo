# ifndef TKERROR_CODES
/*
0 = tkNO_ERROR
1-200 = Common
201-400 = tkshp
401-600 = tkgrd
601-800 = tkimg
801-1000 = tkdbf
1001-1200 = tkutils
1201-1400 = tkmap
1401-1600 = tktinvuc
1501-1600 = tkFeatureSpecific
1601-5000 = Reserved
5001- = UserDefined
*/

const char * ErrorMsg( long ErrorCode );

// 0 = tkNO_ERROR
# define tkNO_ERROR 0

//1-200 = Common
# define tkINDEX_OUT_OF_BOUNDS 1
# define tkUNEXPECTED_NULL_PARAMETER 2
# define tkINVALID_FILE_EXTENSION 3
# define tkINVALID_FILENAME 4
# define tkUNRECOVERABLE_ERROR 5
# define tkFILE_NOT_OPEN 6
# define tkZERO_LENGTH_STRING 7
# define tkINCORRECT_VARIANT_TYPE 8
# define tkINVALID_PARAMETER_VALUE 9
# define tkINTERFACE_NOT_SUPPORTED 10
# define tkUNAVAILABLE_IN_DISK_MODE 11
# define tkCANT_OPEN_FILE 12
# define tkUNSUPPORTED_FILE_EXTENSION 13
# define tkCANT_CREATE_FILE 14
# define tkINVALID_FILE 15
# define tkINVALID_VARIANT_TYPE 16
# define tkOUT_OF_RANGE_0_TO_1 17
# define tkCANT_COCREATE_COM_INSTANCE 18
# define tkFAILED_TO_ALLOCATE_MEMORY 19
# define tkUNSUPPORTED_FORMAT 20
# define tkPROPERTY_DEPRECATED 21
# define tkPROPERTY_NOT_IMPLEMENTED 22
# define tkINVALID_FOR_INMEMORY_OBJECT 23
# define tkCANT_DELETE_FILE 24
# define tkINVALID_EXPRESSION 25
# define tkFILE_EXISTS 26
# define tkBOUNDS_NOT_INTERSECT 27
# define tkGDAL_ERROR 28
# define tkMETHOD_NOT_IMPLEMENTED 29
# define tkFOLDER_NOT_EXISTS 30
# define tkFILE_NOT_EXISTS 31
# define tkMETHOD_DEPRECATED 32
# define tkINVALID_OPEN_STRATEGY 33
# define tkSPATIAL_OPERATION_FAILED 34
# define tkFAILED_TO_READ_INPUT_NAMES 35
# define tkINVALID_PARAMETERS_ARRAY 36

//201 - 400 = tkshp
# define tkUNSUPPORTED_SHAPEFILE_TYPE 201
# define tkINCOMPATIBLE_SHAPEFILE_TYPE 202
# define tkCANT_OPEN_SHP 203
# define tkCANT_OPEN_SHX 204
# define tkINVALID_SHP_FILE 205
# define tkINVALID_SHX_FILE 206
# define tkSHPFILE_IN_EDIT_MODE 207
# define tkSHPFILE_NOT_IN_EDIT_MODE 208
# define tkCANT_CREATE_SHP 209
# define tkCANT_CREATE_SHX 210
# define tkSHP_FILE_EXISTS 211
# define tkSHX_FILE_EXISTS 212
# define tkINCOMPATIBLE_SHAPE_TYPE 213
# define tkPARENT_SHAPEFILE_NOT_EXISTS 214
# define tkCANT_CONVERT_SHAPE_GEOS 215
# define tkSHAPEFILE_UNINITIALIZED 216
# define tkSHP_READ_VIOLATION 217
# define tkSHP_WRITE_VIOLATION 218
# define tkSELECTION_EMPTY 219
# define tkINVALID_SHAPE 220
# define tkUNEXPECTED_SHAPE_TYPE 221
# define tkINVALID_RETURN_TYPE 222
# define tkSHPFILE_WITH_INVALID_SHAPES 223
# define tkCATEGORY_WASNT_FOUND 224
# define tkRESULTINGSHPFILE_EMPTY 225 /* Added by Paul Meems. 23-10-2013 */
# define tkABORTED_ON_INPUT_VALIDATION 226
# define tkABORTED_ON_OUTPUT_VALIDATION 227
# define tkSHAPEFILE_IS_EMPTY 228
# define tkFAILED_TO_BUILD_SPATIAL_INDEX 229
# define tkINMEMORY_SHAPEFILE_EXPECTED 230
# define tkCOLOR_SCHEME_IS_EMPTY 231
# define tkNOT_ENOUGH_POINTS_FOR_SHAPE_TYPE 232

//401-600 = tkgrd
# define tkGRID_NOT_INITIALIZED 401
# define tkINVALID_DATA_TYPE 402
# define tkINVALID_GRID_FILE_TYPE 403
# define tkZERO_ROWS_OR_COLS 404
# define tkINCOMPATIBLE_DATA_TYPE 405
# define tkESRI_DLL_NOT_INITIALIZED 406
# define tkESRI_INVALID_BOUNDS 407
# define tkESRI_ACCESS_WINDOW_SET 408
# define tkCANT_ALLOC_MEMORY 409
# define tkESRI_LAYER_OPEN 410
# define tkESRI_LAYER_CREATE 411
# define tkESRI_CANT_DELETE_FILE 412
# define tkSDTS_BAD_FILE_HEADER 413
# define tkGDAL_GRID_NOT_OPENED 414
# define tkCANT_DISPLAY_WO_PROXY 415
# define tkFAILED_TO_SAVE_GRID 416

//601-800 = tkimg
# define tkCANT_WRITE_WORLD_FILE 601
# define tkINVALID_WIDTH_OR_HEIGHT 602
# define tkINVALID_DY 603
# define tkINVALID_DX 604
# define tkCANT_CREATE_DDB_BITMAP 605
# define tkNOT_APPLICABLE_TO_BITMAP 606
# define tkNOT_APPLICABLE_TO_GDAL 607
# define tkGDAL_DATASET_IS_READONLY 608
# define tkIMAGE_BUFFER_IS_EMPTY 609
# define tkICON_OR_TEXTURE_TOO_BIG 610
# define tkFAILED_TO_OBTAIN_DC 611
# define tkIMAGE_UNINITIALIZED 612
# define tkCANT_DISPLAY_WITHOUT_EXTERNAL_SCHEME 613


//801-1000 = tkdbf
# define tkCANT_OPEN_DBF 801
# define tkDBF_IN_EDIT_MODE 802
# define tkDBF_NOT_IN_EDIT_MODE 803
# define tkDBF_FILE_EXISTS 804
# define tkDBF_FILE_DOES_NOT_EXIST 805
# define tkCANT_CREATE_DBF 806
# define tkDBF_CANT_ADD_DBF_FIELD 807
# define tkCANT_CHANGE_FIELD_TYPE 808
# define tkSAME_FIELD_TYPE_EXPECTED 809

//1001-1200 = tkutils
# define tkOUT_OF_RANGE_0_TO_180 1001
# define tkOUT_OF_RANGE_M360_TO_360 1002
# define tkSHAPEFILE_LARGER_THAN_GRID 1003
# define tkCONCAVE_POLYGONS 1004
# define tkINCOMPATIBLE_DX 1005
# define tkINCOMPATIBLE_DY 1006
# define tkINVALID_FINAL_POINT_INDEX 1007
# define tkTOLERANCE_TOO_LARGE 1008
# define tkNOT_ALIGNED 1009
# define tkINVALID_NODE 1010
# define tkNODE_AT_OUTLET 1011
# define tkNO_NETWORK 1012
# define tkCANT_CHANGE_OUTLET_PARENT 1013
# define tkNET_LOOP 1014
# define tkMISSING_FIELD 1015
# define tkINVALID_FIELD 1016
# define tkINVALID_FIELD_VALUE 1017
# define tkNON_SINGLE_BYTE_PER_BAND 1018
# define tkFAILED_READ_BLOCK 1019
# define tkFAILED_WRITE_BLOCK 1020
# define tkINPUT_RASTERS_DIFFER 1021
# define tkAT_LEAST_TWO_DATASOURCES_EXPECTED 1022
# define tkIMAGES_MUST_HAVE_THE_SAME_SIZE 1023

//1201-1400 = tkmap
# define tkINVALID_LAYER_HANDLE 1201
# define tkINVALID_DRAW_HANDLE 1202
# define tkWINDOW_LOCKED 1203
# define tkINVALID_LAYER_POSITION 1204
# define tkINIT_INVALID_DC 1205
# define tkINIT_CANT_SETUP_PIXEL_FORMAT 1206
# define tkINIT_CANT_CREATE_CONTEXT 1207
# define tkINIT_CANT_MAKE_CURRENT 1208
# define tkUNEXPECTED_LAYER_TYPE 1209
# define tkMAP_NOT_INITIALIZED 1210
# define tkMAP_INVALID_MAPSTATE 1211
# define tkMAP_MAPSTATE_LAYER_LOAD_FAILED 1212
# define tkMAP_PROJECTION_NOT_SET 1213
# define tkINVALID_GEOGRAPHIC_COORDINATES 1214

//1401-1600 = tktinvuc
# define tkVALUE_MUST_BE_2_TO_N 1401
# define tkNOT_INITIALIZED 1402
//1501-1600 = Itkfeature

//1601-1800 = labels
# define tkLABELS_CANT_SYNCHRONIZE 1601
# define tkLABELS_NOT_SYNCHRONIZE 1602
# define tkLABELS_NOT_SAVED 1603

//1801-2000 = geoprojections
#define tkOGR_NOT_ENOUGH_DATA			1801    /* not enough data to deserialize */
#define tkOGR_NOT_ENOUGH_MEMORY			1802
#define tkOGR_UNSUPPORTED_GEOMETRY_TYPE	1803
#define tkOGR_UNSUPPORTED_OPERATION		1804
#define tkOGR_CORRUPT_DATA				1805
#define tkOGR_FAILURE					1806
#define tkOGR_UNSUPPORTED_SRS			1807
#define tkOGR_INVALID_HANDLE			1808
#define tkFAILED_TO_REPROJECT			1809
#define tkPROJECTION_NOT_INITIALIZED	1810
#define tkPRJ_FILE_EXISTS				1811
#define tkTRANSFORMATION_NOT_INITIALIZED	1812
#define tkFAILED_TO_COPY_PROJECTION			1813
#define tkPROJECTION_IS_FROZEN			1814
#define tkGEOGRAPHIC_PROJECTION_EXPECTED 1815
#define tkMISSING_GEOPROJECTION 1816
#define tkPROJECTION_MISMATCH 1817
#define tkNO_REPROJECTION_FOR_IMAGES 1818


//2001-2200 = tiles
#define tkINVALID_PROVIDER_ID 2001
#define tkCANT_DELETE_DEFAULT_PROVIDER 2002
#define tkPROVIDER_ID_RESERVED 2003
#define tkPROVIDER_ID_IN_USE 2004
#define tkINVALID_PROJECTION 2005
#define tkINVALID_URL 2006
#define tkINVALID_ZOOM_LEVEL 2007


# endif