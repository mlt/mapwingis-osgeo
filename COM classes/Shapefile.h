//********************************************************************************************************
//File name: Shapefile.h
//Description: Declaration of the CShapefile
//********************************************************************************************************
//The contents of this file are subject to the Mozilla Public License Version 1.1 (the "License"); 
//you may not use this file except in compliance with the License. You may obtain a copy of the License at 
//http://www.mozilla.org/MPL/ 
//Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF 
//ANY KIND, either express or implied. See the License for the specific language governing rights and 
//limitations under the License. 
//
//The Original Code is MapWindow Open Source. 
//
//The Initial Developer of this version of the Original Code is Daniel P. Ames using portions created by 
//Utah State University and the Idaho National Engineering and Environmental Lab that were released as 
//public domain in March 2004.  
//********************************************************************************************************

#pragma once
#include <set>
#include "IndexSearching.h"
#include "QTree.h"
#include "ClipperConverter.h"
#include "ShapeInfo.h"
#include "ColoringGraph.h"

//Shapefile File Info
#define HEADER_BYTES_16 50
#define HEADER_BYTES_32 100
#define FILE_CODE      9994
#define VERSION        1000
#define UNUSEDSIZE 5
#define UNUSEDVAL		0     	    
#define RECORD_HEADER_LENGTH_32 8
#define RECORD_SHAPE_TYPE_32 8

// *********************************************************************
// CShapefile declaration
// *********************************************************************
class ATL_NO_VTABLE CShapefile : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShapefile, &CLSID_Shapefile>,
	public IDispatchImpl<IShapefile, &IID_IShapefile, &LIBID_MapWinGIS, /*wMajor =*/ VERSION_MAJOR, /*wMinor =*/ VERSION_MINOR>
{
public:
	
	CShapefile();
	~CShapefile();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SHAPEFILE)

	DECLARE_NOT_AGGREGATABLE(CShapefile)

	BEGIN_COM_MAP(CShapefile)
		COM_INTERFACE_ENTRY(IShapefile)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()


// *********************************************************************
//		IShapefile interface
// *********************************************************************
public:
	STDMETHOD(QuickPoints)(/*[in]*/ long ShapeIndex, /*[in, out]*/ long * NumPoints, /*[out, retval]*/ SAFEARRAY ** retval);
	STDMETHOD(QuickExtents)(/*[in]*/ long ShapeIndex, /*[out, retval]*/ IExtents ** retval);
	STDMETHOD(QuickPoint)(/*[in]*/ long ShapeIndex, /*[in]*/ long PointIndex, /*[out, retval]*/ IPoint ** retval);
	STDMETHOD(get_Filename)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FileHandle)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_EditingTable)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_CellValue)(/*[in]*/ long FieldIndex, /*[in]*/ long ShapeIndex, /*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_Field)(/*[in]*/ long FieldIndex, /*[out, retval]*/ IField * *pVal);
	STDMETHOD(get_FieldByName)(/*[in]*/ BSTR Fieldname, /*[out, retval] */ IField * *pVal);
	STDMETHOD(StopEditingTable)(/*[in, optional, defaultvalue(TRUE)]*/ VARIANT_BOOL ApplyChanges, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(StartEditingTable)(/*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(EditCellValue)(/*[in]*/ long FieldIndex, /*[in]*/ long ShapeIndex, /*[in]*/ VARIANT NewVal, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(EditDeleteField)(/*[in]*/ long FieldIndex, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(EditInsertField)(/*[in]*/ IField * NewField, /*[in,out]*/long * FieldIndex, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(get_ErrorMsg)(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(StopEditingShapes)(/*[in, optional, defaultvalue(TRUE)]*/VARIANT_BOOL ApplyChanges,/*[in,optional,defaultvalue(TRUE)]*/VARIANT_BOOL StopEditTable, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(StartEditingShapes)(/*[in,optional,defaultvalue(TRUE)]*/VARIANT_BOOL StartEditTable,/*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(SelectShapes)(/*[in]*/ IExtents * BoundBox, /*[in, optional, defaultvalue(0.0)]*/double Tolerance, /*[in, optional, defaultvalue(INTERSECTION)]*/ SelectMode SelectMode, /*[in, out]*/ VARIANT * Result, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(EditDeleteShape)(/*[in]*/long ShapeIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(EditInsertShape)(/*[in]*/IShape * Shape, /*[in, out]*/ long * ShapeIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(EditClear)(/*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(Close)(/*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(SaveAs)(/*[in]*/ BSTR ShapefileName, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(CreateNew)(/*[in]*/ BSTR ShapefileName, /*[in]*/ ShpfileType ShapefileType, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(CreateNewWithShapeID)(/*[in]*/ BSTR ShapefileName, /*[in]*/ ShpfileType ShapefileType, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(Open)(/*[in]*/ BSTR ShapefileName, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/VARIANT_BOOL * retval);
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Key)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_GlobalCallback)(/*[out, retval]*/ ICallback * *pVal);
	STDMETHOD(put_GlobalCallback)(/*[in]*/ ICallback * newVal);
	STDMETHOD(get_CdlgFilter)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LastErrorCode)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_EditingShapes)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_Shape)(/*[in]*/long ShapeIndex, /*[out, retval]*/ IShape * *pVal);
	STDMETHOD(get_ShapefileType)(/*[out, retval]*/ ShpfileType *pVal);
	STDMETHOD(get_Extents)(/*[out, retval]*/ IExtents * *pVal);
	STDMETHOD(get_NumFields)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumShapes)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Projection)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Projection)(/*[in]*/BSTR proj4String);
	STDMETHOD(get_NumPoints)(/*[in]*/ long ShapeIndex, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_UseSpatialIndex)(/*[out, retval]*/VARIANT_BOOL *pVal);
	STDMETHOD(put_UseSpatialIndex)(/*[in]*/VARIANT_BOOL pVal);
 	STDMETHOD(get_HasSpatialIndex)(/*[out, retval]*/VARIANT_BOOL *pVal);
	STDMETHOD(put_HasSpatialIndex)(/*[in]*/VARIANT_BOOL pVal);
    STDMETHOD(CreateSpatialIndex)(/*[in]*/BSTR ShapefileName, /*[out, retval]*/ VARIANT_BOOL  *pVal);
	STDMETHOD(Resource)(/*[in]*/ BSTR newSrcPath, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(IsSpatialIndexValid)(/*[out, retval]*/ VARIANT_BOOL  *pVal);
	STDMETHOD(put_SpatialIndexMaxAreaPercent)(/*[in]*/ DOUBLE newVal);
	STDMETHOD(get_SpatialIndexMaxAreaPercent)(/*[out, retval]*/ DOUBLE* pVal);
	STDMETHOD(get_CanUseSpatialIndex)(/*[in]*/ IExtents* pArea, /*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(PointInShape)(LONG ShapeIndex, DOUBLE x, DOUBLE y, VARIANT_BOOL* retval);
	STDMETHOD(PointInShapefile)(DOUBLE x, DOUBLE y, LONG* ShapeIndex);
	STDMETHOD(BeginPointInShapefile)(VARIANT_BOOL* retval);
	STDMETHOD(EndPointInShapefile)(void);
	STDMETHOD(get_CacheExtents)(VARIANT_BOOL * pVal);
	STDMETHOD(put_CacheExtents)(VARIANT_BOOL newVal);
	STDMETHOD(RefreshExtents)(VARIANT_BOOL * pVal);
	STDMETHOD(RefreshShapeExtents)(LONG ShapeId, VARIANT_BOOL *pVal);
	STDMETHOD(QuickQueryInEditMode)(/*[in]*/IExtents * BoundBox,int ** Result, int* ResultCount);
	STDMETHOD(get_UseQTree)(VARIANT_BOOL * pVal);
	STDMETHOD(put_UseQTree)(VARIANT_BOOL pVal);
	STDMETHOD(Save)(/*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	STDMETHOD(GetIntersection)(/*[in]*/VARIANT_BOOL SelectedOnlyOfThis, /*[in]*/IShapefile* sf, /*[in]*/VARIANT_BOOL SelectedOnly, /*[in]*/ ShpfileType fileType, /*[in, optional, defaultvalue(NULL)]*/ ICallback * cBack, /*[out, retval]*/ IShapefile** retval);
	STDMETHOD(SelectByShapefile)(/*[in]*/IShapefile* sf,  /*[in]*/tkSpatialRelation Relation, /*[in]*/VARIANT_BOOL SelectedOnly, /*[in, out]*/ VARIANT *Result, /*[in, optional, defaultvalue(NULL)]*/ ICallback* cBack, /*[out, retval]*/VARIANT_BOOL *retval);
	STDMETHOD(get_SelectionDrawingOptions)(/*[out, retval]*/IShapeDrawingOptions** pVal);
	STDMETHOD(put_SelectionDrawingOptions)(/*[in]*/IShapeDrawingOptions* newVal);
	STDMETHOD(get_ShapeSelected)(/*[in]*/long ShapeIndex,/*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(put_ShapeSelected)(/*[in]*/long ShapeIndex, /*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_NumSelected)(/*[out, retval]*/long *pVal);
	STDMETHOD(SelectAll)();
	STDMETHOD(SelectNone)();
	STDMETHOD(InvertSelection)();
	STDMETHOD(Dissolve)(long FieldIndex, VARIANT_BOOL SelectedOnly, IShapefile** sf);
	STDMETHOD(get_Labels)(ILabels** pVal);
	STDMETHOD(put_Labels)(ILabels* newVal);
	STDMETHOD(GenerateLabels)(long FieldIndex, tkLabelPositioning Method, VARIANT_BOOL LargestPartOnly, long* Count);
	STDMETHOD(Clone)(IShapefile** retVal);
	STDMETHOD(get_DefaultDrawingOptions)(IShapeDrawingOptions** pVal);
	STDMETHOD(put_DefaultDrawingOptions)(IShapeDrawingOptions* newVal);
	STDMETHOD(get_Categories)(IShapefileCategories** pVal);
	STDMETHOD(put_Categories)(IShapefileCategories* newVal);
	STDMETHOD(get_Charts)(ICharts** pVal);
	STDMETHOD(put_Charts)(ICharts* newVal);
	STDMETHOD(get_ShapeCategory)(/*[in]*/long ShapeIndex,/*[out, retval]*/ long* pVal);
	STDMETHOD(put_ShapeCategory)(/*[in]*/long ShapeIndex, /*[in]*/ long newVal);
	STDMETHOD(get_Table)(ITable** retVal);
	STDMETHOD(get_VisibilityExpression)(BSTR* retval);
	STDMETHOD(put_VisibilityExpression)(BSTR newVal);
	STDMETHOD(get_FastMode)(VARIANT_BOOL* retval);		// in fast editing mode CShapeWrapper class is used to store shape points
	STDMETHOD(put_FastMode)(VARIANT_BOOL newVal);		// there are some restrictions on editing this mode though
	STDMETHOD(get_MinDrawingSize)(LONG* pVal);
	STDMETHOD(put_MinDrawingSize)(LONG newVal);
	STDMETHOD(get_SourceType)(tkShapefileSourceType* pVal);
	STDMETHOD(BufferByDistance)(double Distance, LONG nSegments, VARIANT_BOOL SelectedOnly, VARIANT_BOOL MergeResults, IShapefile** sf);
	STDMETHOD(get_GeometryEngine)(tkGeometryEngine* pVal);
	STDMETHOD(put_GeometryEngine)(tkGeometryEngine pVal);
	STDMETHOD(Difference)(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, IShapefile** retval);
	STDMETHOD(Clip)(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, IShapefile** retval);
	STDMETHOD(SymmDifference)(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, IShapefile** retval);
	STDMETHOD(Union)(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, IShapefile** retval);
	STDMETHOD(ExplodeShapes)(VARIANT_BOOL SelectedOnly, IShapefile** retval);
	STDMETHOD(AggregateShapes)(VARIANT_BOOL SelectedOnly, LONG FieldIndex, IShapefile** retval);
	STDMETHOD(ExportSelection)(IShapefile** retval);
	STDMETHOD(Sort)(LONG FieldIndex, VARIANT_BOOL Ascending, IShapefile** retval);
	STDMETHOD(Merge)(VARIANT_BOOL SelectedOnlyThis, IShapefile* sf, VARIANT_BOOL SelectedOnly, IShapefile** retval);
	STDMETHOD(get_SelectionColor)(OLE_COLOR* retval);
	STDMETHOD(put_SelectionColor)(OLE_COLOR newVal);
	STDMETHOD(get_SelectionAppearance)(tkSelectionAppearance* retval);
	STDMETHOD(put_SelectionAppearance)(tkSelectionAppearance newVal);
	STDMETHOD(get_CollisionMode)(tkCollisionMode* retval);
	STDMETHOD(put_CollisionMode)(tkCollisionMode newVal);
	STDMETHOD(get_SelectionTransparency)(BYTE* retval);
	STDMETHOD(put_SelectionTransparency)(BYTE newVal);
	STDMETHOD(put_StopExecution)(IStopExecution* stopper);
	STDMETHOD(Serialize)(VARIANT_BOOL SaveSelection, BSTR* retVal);
	STDMETHOD(Serialize2)(VARIANT_BOOL SaveSelection, VARIANT_BOOL SerializeCategories, BSTR* retVal);
	STDMETHOD(Deserialize)(VARIANT_BOOL LoadSelection, BSTR newVal);
	STDMETHOD(get_GeoProjection)(IGeoProjection** retVal);
	STDMETHOD(put_GeoProjection)(IGeoProjection* pVal);
	STDMETHOD(Reproject)(IGeoProjection* newProjection, LONG* reprojectedCount, IShapefile** retVal);
	STDMETHOD(ReprojectInPlace)(IGeoProjection* newProjection, LONG* reprojectedCount, VARIANT_BOOL* retVal);
	STDMETHOD(SimplifyLines)(DOUBLE Tolerance, VARIANT_BOOL SelectedOnly, IShapefile** retVal);
	STDMETHOD(FixUpShapes)(IShapefile** retVal, VARIANT_BOOL* fixed);
	STDMETHOD(GetRelatedShapes)(long referenceIndex, tkSpatialRelation relation, VARIANT* resultArray, VARIANT_BOOL* retval);
	STDMETHOD(GetRelatedShapes2)(IShape* referenceShape, tkSpatialRelation relation, VARIANT* resultArray, VARIANT_BOOL* retval);
	STDMETHOD(get_Identifiable)(VARIANT_BOOL* retVal);
	STDMETHOD(put_Identifiable)(VARIANT_BOOL newVal);
	STDMETHOD(HasInvalidShapes)(VARIANT_BOOL* result);
	STDMETHOD(EditAddShape)(IShape* shape, long* shapeIndex);
	STDMETHOD(EditAddField)(BSTR name, FieldType type, int precision, int width, long* fieldIndex);
	STDMETHOD(GetClosestVertex)(double x, double y, double maxDistance, long* shapeIndex, long* pointIndex, double* distance, VARIANT_BOOL* retVal);
	STDMETHOD(get_ShapeCategory2)(long ShapeIndex, BSTR* categoryName);
	STDMETHOD(put_ShapeCategory2)(long ShapeIndex, BSTR categoryName);
	STDMETHOD(get_ShapeCategory3)(long ShapeIndex, IShapefileCategory** category);
	STDMETHOD(put_ShapeCategory3)(long ShapeIndex, IShapefileCategory* category);
	STDMETHOD(Dump)(BSTR ShapefileName, ICallback *cBack, VARIANT_BOOL *retval);
	STDMETHOD(LoadDataFrom)(BSTR ShapefileName, ICallback *cBack, VARIANT_BOOL *retval);
	STDMETHOD(Segmentize)(IShapefile** retVal);
	STDMETHOD(get_LastInputValidation)(IShapeValidationInfo** retVal);
	STDMETHOD(get_LastOutputValidation)(IShapeValidationInfo** retVal);
	STDMETHOD(ClearCachedGeometries)();
	STDMETHOD(AggregateShapesWithStats)(VARIANT_BOOL SelectedOnly, LONG FieldIndex, IFieldStatOperations* statOperations, IShapefile** retval);
	STDMETHOD(DissolveWithStats)(long FieldIndex, VARIANT_BOOL SelectedOnly, IFieldStatOperations* statOperations, IShapefile** sf);
	STDMETHOD(get_ShapeRotation)(long ShapeIndex, double* pVal);
	STDMETHOD(put_ShapeRotation)(long ShapeIndex, double newVal);
	STDMETHOD(get_ShapeVisible)(long ShapeIndex, VARIANT_BOOL* pVal);
	STDMETHOD(get_Volatile)(VARIANT_BOOL* retval);
	STDMETHOD(put_Volatile)(VARIANT_BOOL newVal);
	STDMETHOD(EditUpdateShape)(long shapeIndex, IShape* shpNew, VARIANT_BOOL* retVal);
	STDMETHOD(get_ShapeModified)(long ShapeIndex, VARIANT_BOOL* retVal);
	STDMETHOD(put_ShapeModified)(long ShapeIndex, VARIANT_BOOL newVal);
	STDMETHOD(Validate)(tkShapeValidationMode validationMode, VARIANT_BOOL selectedOnly, IShapeValidationInfo** results);
	STDMETHOD(get_UndoList)(IUndoList** pVal);
	STDMETHOD(get_InteractiveEditing)(VARIANT_BOOL* pVal);
	STDMETHOD(put_InteractiveEditing)(VARIANT_BOOL newVal);
	STDMETHOD(get_ShapeIsHidden)(LONG shapeIndex, VARIANT_BOOL* pVal);
	STDMETHOD(put_ShapeIsHidden)(LONG shapeIndex, VARIANT_BOOL newVal);
	STDMETHOD(get_Snappable)(VARIANT_BOOL* pVal);
	STDMETHOD(put_Snappable)(VARIANT_BOOL newVal);
	STDMETHOD(get_ShapefileType2D)(ShpfileType* pVal);
	STDMETHOD(get_FieldIndexByName)(BSTR fieldName, LONG* pVal);
	STDMETHOD(Move)(DOUBLE xProjOffset, DOUBLE yProjOffset, VARIANT_BOOL* retVal);
	STDMETHOD(RemoveSpatialIndex)(VARIANT_BOOL* retVal);
	STDMETHOD(get_ShapeRendered)(LONG ShapeIndex, VARIANT_BOOL* pVal);

private:

	// data for point in shapefile test
	struct ShapeHeader
	{
		double MinX, MinY;
		double MaxX, MaxY;
		int NumParts;
		int NumPoints;
	};
	struct PolygonShapefile
	{
		ShapeHeader shpHeader;
		std::vector<Point2D> Points;
		std::vector<int> Parts;
	};

private:

	std::vector<PolygonShapefile> _polySf;
	
	tkShapefileSourceType _sourceType;		// is it disk-based or in-memory?
	ShpfileType _shpfiletype;
	BSTR _key;
	long _lastErrorCode;
	
	//Extent Information
	double _minX;
	double _minY;
	double _minZ;
	double _maxX;
	double _maxY;
	double _maxZ;
	double _minM;
	double _maxM;

	//Disk access
	FILE * _shpfile;
	FILE * _shxfile;

	CStringW _shpfileName;
	CStringW _shxfileName;
	CStringW _dbffileName;
	CStringW _prjfileName;
	
	std::vector<ShapeData*> _shapeData;
	std::vector<long> _shpOffsets;		//(32 bit words)

	// table is initialized in CreateNew or Open methods
	// it is is destroyed in Close() method
	// in case table is null, shapefile will be considered uninitialized
	ITable * _table;
	IGeoProjection* _geoProjection;
	IStopExecution* _stopExecution;
	ICharts* _charts;
	ILabels* _labels;
	IShapefileCategories* _categories;
	ICallback * _globalCallback;
	IShapeDrawingOptions* _selectDrawOpt;	
	IShapeDrawingOptions* _defaultDrawOpt;
	IShapeValidationInfo* _inputValidation;
	IShapeValidationInfo* _outputValidation;
	IUndoList* _undoList;
	
	VARIANT_BOOL _hotTracking;
	bool _geosGeometriesRead;
	tkCollisionMode _collisionMode;		// collision mode for point shapefiles
	tkGeometryEngine _geometryEngine;		// GEOS or Clipper
	bool _writing;		// is currently writing to the file
	bool _reading;		// is currently reading data into memory
	
	BSTR _expression;		// visibility expression
	BOOL _isEditingShapes;		//Flag for Disk vs. Memory
	long _nextShapeHandle;		// the next unique handle to assign
	VARIANT_BOOL _interactiveEditing;
	VARIANT_BOOL _snappable;
	
	// When this flag is on CShapeWrapper will be used in the Shape class to store the points
	// otherwise usual COM points
	BOOL _fastMode;
	int _minDrawingSize;	// objects which are less than this value in pixels for current scale, will drawn as point
	BOOL _cacheExtents;	// extents won't be recalculated in each get_Extents call
	bool _volatile;

	//Flags for Spatial Indexing
	BOOL _useSpatialIndex;
	BOOL _hasSpatialIndex;
	IndexSearching::CSpatialIndexID _spatialIndexID;
	BOOL _spatialIndexLoaded;	
	DOUBLE _spatialIndexMaxAreaPercent;
	int _spatialIndexNodeCapacity;
	
	// drawing options
	tkSelectionAppearance _selectionAppearance;
	OLE_COLOR _selectionColor;
	unsigned char _selectionTransparency;

	QTree* _qtree;
	BOOL _useQTree;
	vector<int> _deleteElement;
	
	// during processing operations only
	QTree* _tempTree;
	
	bool _useValidationList;

private:
	// memory shapes
	BOOL ReleaseMemoryShapes();
	BOOL VerifyMemShapes(ICallback * cBack);
	void TrimMemShapes();

	// read/write
	BOOL ReadShx();
	BOOL WriteShx(FILE * _shxfile, ICallback * cBack);
	BOOL WriteShp(FILE * shpfile, ICallback * cBack);	

	// selection
	BOOL DefineShapePoints(long ShapeIndex, ShpfileType & ShapeType, std::vector<long> & parts, std::vector<double> & xPts, std::vector<double> & yPts);
	VARIANT_BOOL SelectShapesAlt(IExtents *BoundBox, double Tolerance, SelectMode SelectMode, VARIANT* arr);

	// initialization
	void put_ReferenceToLabels(bool bNullReference = false);
	void put_ReferenceToCategories(bool bNullReference = false);
	void put_ReferenceToCharts(bool bNullReference = false);

	// quad tree
	void GenerateQTree();
	QTree* GenerateQTreeCore(bool SelectedOnly);
	bool GenerateTempQTree(bool SelectedOnly);
	void ClearTempQTree();
	QTree* GetTempQTree();

	// geoprocessing
	void DoClipOperation(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, IShapefile** retval, tkClipOperation operation, ShpfileType returnType = SHP_NULLSHAPE);
	void DissolveClipper(long FieldIndex, VARIANT_BOOL SelectedOnly, IFieldStatOperations* operations, IShapefile* sf);
	void DissolveGEOS(long FieldIndex, VARIANT_BOOL SelectedOnly, IFieldStatOperations* operations, IShapefile* sf);
	void IntersectionGEOS(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfClip, VARIANT_BOOL SelectedOnlyClip, IShapefile* sfResult, map<long, long>* fieldMap = NULL, std::set<int>* subjectShapesToSkip = NULL,  std::set<int>* clippingShapesToSkip = NULL );
	void IntersectionClipper(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfClip, VARIANT_BOOL SelectedOnlyClip, IShapefile* sfResult, map<long, long>* fieldMap = NULL, std::set<int>* subjectShapesToSkip = NULL,  std::set<int>* clippingShapesToSkip = NULL );
	IShapefile* IntersectionClipperNoAttributes(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfClip, VARIANT_BOOL SelectedOnlyClip );
	void DifferenceGEOS(IShapefile* sfSubject, VARIANT_BOOL SelectedOnlySubject, IShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay,  IShapefile* sfResult, map<long, long>* fieldMap = NULL, std::set<int>* shapesToSkip = NULL);
	void DifferenceClipper(IShapefile* sfSubject, VARIANT_BOOL SelectedOnlySubject, IShapefile* sfClip, VARIANT_BOOL SelectedOnlyClip, IShapefile* sfResult, map<long, long>* fieldMap = NULL, std::set<int>* shapesToSkip = NULL);
	void ClipGEOS(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, IShapefile* sfResult);
	void ClipClipper(VARIANT_BOOL SelectedOnlySubject, IShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, IShapefile* sfResult);
	void AggregateShapesCore(VARIANT_BOOL SelectedOnly, LONG FieldIndex, IFieldStatOperations* statOperations, IShapefile** retval);
	void DissolveCore(long FieldIndex, VARIANT_BOOL SelectedOnly, IFieldStatOperations* statOperations, IShapefile** sf);
	void CalculateFieldStats(map<int, vector<int>*>& indicesMap, IFieldStatOperations* operations, IShapefile* output);
	void InsertShapesVector(IShapefile* sf, vector<IShape* >& vShapes, IShapefile* sfSubject, long subjectId, std::map<long, long>* fieldMapSubject = NULL,	IShapefile* sfClip = NULL, long clipId = -1, std::map<long, long>* fieldMapClip = NULL);
	void GetRelatedShapeCore(IShape* referenceShape, long referenceIndex, tkSpatialRelation relation, VARIANT* resultArray, VARIANT_BOOL* retval);
	
	
public:
	// accessing shapes
	bool ShapeAvailable(int shapeIndex, VARIANT_BOOL selectedOnly);
	HRESULT GetValidatedShape(int shapeIndex, IShape** retVal);
	void ReadGeosGeometries(VARIANT_BOOL selectedOnly);
	GEOSGeometry* GetGeosGeometry(int shapeIndex);

	// geoprocessing
	Coloring::ColorGraph* GeneratePolygonColors();
	bool ReprojectCore(IGeoProjection* newProjection, LONG* reprojectedCount, IShapefile** retVal, bool reprojectInPlace);

	// errors
	void ErrorMessage(long ErrorCode);
	void ErrorMessage(long ErrorCode, ICallback* cBack);

	// underlying data
	std::vector<ShapeData*>* get_ShapeVector();
	IShapeWrapper* get_ShapeWrapper(int ShapeIndex);
	IShapeData* get_ShapeData(int ShapeIndex);
	FILE* get_File(){ return _shpfile; }
	
	// serialization
	bool DeserializeCore(VARIANT_BOOL LoadSelection, CPLXMLNode* node);
	CPLXMLNode* SerializeCore(VARIANT_BOOL SaveSelection, CString ElementName, bool serializeCategories);

	// charts
	void SetChartsPositions(tkLabelPositioning Method);
	void ClearChartFrames();
	
	// selection
	bool SelectShapesCore(Extent& extents, double Tolerance, SelectMode SelectMode, std::vector<long>& selectResult, bool renderedOnly);
	bool QuickExtentsCore(long ShapeIndex, Extent& result);
	bool QuickExtentsCore(long ShapeIndex, double* xMin, double* yMin, double* xMax, double* yMax);

	// editing
	bool OpenCore(CStringW tmp_shpfileName, ICallback* cBack);
	HRESULT CreateNewCore(BSTR ShapefileName, ShpfileType ShapefileType, bool applyRandomOptions, VARIANT_BOOL *retval);
	void RegisterNewShape(IShape* Shape, long ShapeIndex);
	void ReregisterShape(int shapeIndex);

	// validation
	void SetValidationInfo(IShapeValidationInfo* info, tkShapeValidationType validationType);
	void CreateValidationList(bool selectedOnly);
	void ClearValidationList();
	void SetValidatedShape(int shapeIndex, ShapeValidationStatus status, IShape* shape = NULL);
	bool ValidateInput(IShapefile* isf, CString methodName, CString parameterName, VARIANT_BOOL selectedOnly, CString className = "Shapefile");
	IShapeValidationInfo* ValidateInputCore(IShapefile* isf, CString methodName, CString parameterName,	VARIANT_BOOL selectedOnly, tkShapeValidationMode validationMode, CString className = "Shapefile", bool reportOnly = false);
	IShapeValidationInfo* ValidateOutput(IShapefile** isf, CString methodName, CString className = "Shapefile", bool abortIfEmpty = true);
	bool ValidateOutput(IShapefile* sf, CString methodName, CString className= "Shapefile", bool abortIfEmpty = true);
	void RestoreShapeRecordsMapping();
	
	// drawing 
	void MarkUndrawn();
	
};
OBJECT_ENTRY_AUTO(__uuidof(Shapefile), CShapefile)
