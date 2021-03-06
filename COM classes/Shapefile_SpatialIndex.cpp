//********************************************************************************************************
//File name: Shapefile.cpp
//Description: Implementation of the CShapefile (see other cpp files as well)
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
//
//Contributor(s): (Open source contributors should list themselves and their modifications here). 
// -------------------------------------------------------------------------------------------------------
// lsu 3-02-2011: split the initial Shapefile.cpp file to make entities of the reasonble size

#include "stdafx.h"
#include "Shapefile.h"

#pragma region SpatialIndex
// *****************************************************************
//		get_HasSpatialIndex()
// *****************************************************************
//ajp June 2008 Property does spatial index exist
STDMETHODIMP CShapefile::get_HasSpatialIndex(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    try 
	{
		hasSpatialIndex	= FALSE;
		CString mwdfileName = _shpfileName.Left(_shpfileName.GetLength() - 3) + "mwd";
		CString mwxfileName = _shpfileName.Left(_shpfileName.GetLength() - 3) + "mwx";
		if (Utility::fileExists(mwdfileName) && Utility::fileExists(mwxfileName))
		{
			hasSpatialIndex = TRUE;
		}
	}
	catch (...)
	{
	    hasSpatialIndex = FALSE;
	}

 	*pVal = hasSpatialIndex?VARIANT_TRUE:VARIANT_FALSE;

	return S_OK;
}

// *****************************************************************
//		get_HasSpatialIndex()
// *****************************************************************
//ajp June 2008 Property does spatial index exist
STDMETHODIMP CShapefile::put_HasSpatialIndex(VARIANT_BOOL pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	hasSpatialIndex = pVal;	// CreateSpatialIndex should be used to create it
	return S_OK;
}

bool TestIndexSearching()
{
	__try
	{
		IndexSearching::isValidSpatialIndex("", 0);
		return true;
	}
	__except(1)
	{
		return false;
	}
}

// *****************************************************************
//		get/put_UseSpatialIndex()
// *****************************************************************
//ajp June 2008 Property use spatial indexing
STDMETHODIMP CShapefile::get_UseSpatialIndex(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (useSpatialIndex)
	{
		//useSpatialIndex = TestIndexSearching();
	}
	*pVal = useSpatialIndex?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}
STDMETHODIMP CShapefile::put_UseSpatialIndex(VARIANT_BOOL pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	useSpatialIndex = pVal;
	
	// Unload spatial index in case it needs to be recreated
	if (!useSpatialIndex && spatialIndexLoaded) 
	{
		IndexSearching::unloadSpatialIndex(spatialIndexID);
	}
	return S_OK;
}

// *****************************************************************
//		get/put_SpatialIndexMaxAreaPercent()
// *****************************************************************
//08-24-2009 (sm) spatial index performance
STDMETHODIMP CShapefile::put_SpatialIndexMaxAreaPercent(DOUBLE newVal)
{
	spatialIndexMaxAreaPercent = newVal;
	return S_OK;
}
STDMETHODIMP CShapefile::get_SpatialIndexMaxAreaPercent(DOUBLE* pVal)
{
	*pVal = spatialIndexMaxAreaPercent;
	return S_OK;
}

bool CShapefile::get_CanUseSpatialIndexCore(Extent& extents)
{
	if (!_isEditingShapes && useSpatialIndex)
	{
		VARIANT_BOOL spatialIndexExists;
		get_HasSpatialIndex(&spatialIndexExists);
		
		if (spatialIndexExists)
		{
			double xM = min(_maxX, extents.right);
			double xm = max(_minX, extents.left);
			double yM = min(_maxY, extents.top);
			double ym = max(_minY, extents.bottom);
			
			double shapeFileArea = (_maxX - _minX)*(_maxY-_minY);
			double selectShapeArea = (xM - xm) * (yM - ym);
			
			if (selectShapeArea/shapeFileArea < spatialIndexMaxAreaPercent) 
			{
				//when large portions of the map are being drawn,
				//the spatial index *probably* won't help, don't use it.
				if (spatialIndexLoaded)
				{
					return true;
				}
				else
				{
					USES_CONVERSION;
					string baseName = W2A(_shpfileName.Left(_shpfileName.GetLength() - 4));		// TODO: use Unicode
					if (IndexSearching::loadSpatialIndex(baseName, false, spatialIndexNodeCapacity, spatialIndexID))
					{
						spatialIndexLoaded = true;
						return true;
					}
				}
			}
		}
	}
	return false;
}

// *****************************************************************
//		get_CanUseSpatialIndex()
// *****************************************************************
//Check that the spatial index exists, is set to be used and should be used.
STDMETHODIMP CShapefile::get_CanUseSpatialIndex(IExtents* pArea, VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = VARIANT_FALSE;
	double xm, xM, ym, yM, zm, zM;
	pArea->GetBounds(&xm, &ym, &zm, &xM, &yM, &zM);
	Extent box(xm, xM, ym, yM);
	*pVal = this->get_CanUseSpatialIndexCore(box);
	return S_OK;
}

// ***********************************************************
//		CreateSpatialIndex()
// ***********************************************************
//ajp June 2008 Function to create an Index file
STDMETHODIMP CShapefile::CreateSpatialIndex(BSTR ShapefileName, VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	*retval = VARIANT_TRUE;

    CString tmp_shpfileName = OLE2CA(ShapefileName);
	if( tmp_shpfileName.GetLength() <= 3 )
	{	
		*retval = VARIANT_FALSE;
		ErrorMessage(tkINVALID_FILENAME);
	}
	else
	{
		string baseName;
		baseName = tmp_shpfileName.Left(tmp_shpfileName.GetLength() - 4);
	    
		// 0.9 = utilization rate, 100 = Node Capacity
		// Creates two files baseName.dat and baseName.idx
		try 
		{
			if (!IndexSearching::createSpatialIndex(0.9, spatialIndexNodeCapacity, (char *)baseName.c_str()))
			{	
				*retval = VARIANT_FALSE;
				ErrorMessage(tkINVALID_FILENAME);
			}
		}
		catch (...)
		{
		    *retval = VARIANT_FALSE;
		}
	}

	return S_OK;
}

// ***********************************************************
//		IsSpatialIndexValid()
// ***********************************************************
STDMETHODIMP CShapefile::IsSpatialIndexValid(VARIANT_BOOL *retval)
{
	VARIANT_BOOL hasSpatialIndex;
	get_HasSpatialIndex(&hasSpatialIndex);
	if (!hasSpatialIndex)
		*retval = VARIANT_FALSE;
	else
	{
		USES_CONVERSION;
		string baseName = W2A(_shpfileName.Left(_shpfileName.GetLength() - 4));		// TODO: use Unicode
		bool bIsValid = IndexSearching::isValidSpatialIndex(baseName.c_str(), spatialIndexNodeCapacity);
		*retval = bIsValid ? VARIANT_TRUE : VARIANT_FALSE;
	}

	return S_OK;
}
#pragma endregion


#pragma region QuadTree
// ********************************************************************
//		QuickQueryInEditMode()
// ********************************************************************
//Neio 2009/07/21 
STDMETHODIMP CShapefile::QuickQueryInEditMode(IExtents *BoundBox, int ** Result, int* ResultCount )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! _isEditingShapes )
	{
		ErrorMessage( tkSHPFILE_NOT_IN_EDIT_MODE );
		return S_OK;
	}
	else if( useQTree )
	{	
		double xMin, yMin, zMin, xMax, yMax, zMax;
		BoundBox->GetBounds(&xMin,&yMin,&zMin,&xMax,&yMax,&zMax);

		vector<int> r = m_qtree->GetNodes(QTreeExtent(xMin,xMax,yMax,yMin));
		int size = r.size();
		*Result = new int[size];
		
		memcpy( *Result, &r[0], sizeof(int) * size);
		*ResultCount = size;
	}
	return S_OK;
}

// *****************************************************************
//		get_UseQTree()
// *****************************************************************
STDMETHODIMP CShapefile::get_UseQTree(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = (useQTree)?VARIANT_TRUE:VARIANT_FALSE;	
	return S_OK;
}

// *****************************************************************
//		put_UseQTree()
// *****************************************************************
STDMETHODIMP CShapefile::put_UseQTree(VARIANT_BOOL pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (pVal && !useQTree)
	{
		this->GenerateQTree();
		useQTree = VARIANT_TRUE;
	}
	else if(!pVal && useQTree)
	{
		delete m_qtree;
		m_qtree = NULL;
		useQTree = VARIANT_FALSE;
	}
	else
	{
		useQTree = pVal?VARIANT_TRUE:VARIANT_FALSE;
	}
	return S_OK;
}

// **********************************************************
//		GenerateQTree
// **********************************************************
void CShapefile::GenerateQTree()
{
	if(m_qtree)
	{
		delete m_qtree;
		m_qtree = NULL;
	}
	
	m_qtree = GenerateQTreeCore(false);
}

// **********************************************************************
// 						GenerateQTreeCore()				           
// **********************************************************************
QTree* CShapefile::GenerateQTreeCore(bool SelectedOnly)
{	
	if (_shapeData.size() == 0)
		return NULL;
	
	IExtents* ext = NULL;
	double xMin,xMax,yMin,yMax,zMin,zMax;
	this->get_Extents(&ext);
	if (!ext) return NULL;
	ext->GetBounds(&xMin,&yMin,&zMin,&xMax,&yMax,&zMax);
	ext->Release();

	QTree* qtree = new QTree(QTreeExtent(xMin,xMax,yMax,yMin));

	long percent;
	int numShapes = (int)_shapeData.size();
	for(int i = 0; i < numShapes; i++ )
	{	
		if (!ShapeAvailable(i, SelectedOnly))
			continue;

		this->QuickExtentsCore(i, &xMin,&yMin,&xMax,&yMax);
		
		QTreeNode node;
		node.Extent.left =xMin;
		node.Extent.right = xMax;
		node.Extent.top = yMax;
		node.Extent.bottom = yMin;
		node.index = i;
		qtree->AddNode(node);
		
		Utility::DisplayProgress(globalCallback, i, numShapes, "Building index...", key, percent);
	}
	Utility::DisplayProgressCompleted(globalCallback, key);

	return qtree;
}
#pragma endregion

// Build the tree anew for geoprocessing operations, as the original one
// probably not 100% accurate/optimal + we may need only selected shapes

// **********************************************************************
// 						GenerateTempQTree()				           
// **********************************************************************
bool CShapefile::GenerateTempQTree(bool SelectedOnly)
{
	ClearTempQTree();
	_tempTree = GenerateQTreeCore(SelectedOnly);
	return _tempTree != NULL;
}

// **********************************************************************
// 						ClearTempQTree()				           
// **********************************************************************
void CShapefile::ClearTempQTree()
{
	if (_tempTree)
	{
		delete _tempTree;
		_tempTree = NULL;
	}
}

// **********************************************************************
// 						GetTempQtree()				           
// **********************************************************************
QTree* CShapefile::GetTempQTree()
{
	return _tempTree;
}


