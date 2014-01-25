// This file stores properties of shapefile layers and separate shapes
// The properties are obsolete, new ShapeDrawingOptions and ShapeCategory classes should be used instead
// Where possible the call where redirected to the new classes

// TODO: old and new implementations should be merged
// m_drawingMethod = dmNewSymbology will serve as a switch

#pragma once
#include "stdafx.h"
#include "MapWinGis.h"
#include "Map.h"
#include <vector>
#include "Enumerations.h"
//#include "UtilityFunctions.h"
#include "ShapeDrawingOptions.h"

// TODO: the following propoerties for the new symbology must be implemented
// ShapeLayerLineStipple
// ShapeLayerFillStipple
// ShapeLayerPointType
// SetUDPointType

#pragma region General
// *************************************************************
//	  ShapefileDrawingOptions
// *************************************************************
// Returns drawing options for a given shapefile
CDrawingOptionsEx* CMapView::get_ShapefileDrawingOptions(long layerHandle)
{
	if (layerHandle >= 0 && layerHandle < (long)m_allLayers.size())
	{
		Layer * layer = m_allLayers[layerHandle];
		if( layer->type == ShapefileLayer )
		{
			IShapefile* sf = NULL;
			//layer->object->QueryInterface(IID_IShapefile, (void**)&sf);
			//if (sf)
			if (layer->QueryShapefile(&sf))
			{
				IShapeDrawingOptions* options = NULL;
				sf->get_DefaultDrawingOptions(&options);
				sf->Release();
				if (options)
				{
					CDrawingOptionsEx* retVal = ((CShapeDrawingOptions*)options)->get_UnderlyingOptions();
					options->Release();
					return retVal;
				}
			}
		}
		else
		{
			this->ErrorMessage(tkUNEXPECTED_LAYER_TYPE);
		}
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
	}
	return NULL;
}

// *************************************************************
//	  GetShapefileType
// *************************************************************
// Returns type of the shapefile with a given handle
ShpfileType CMapView::get_ShapefileType(long layerHandle)
{
	if (layerHandle >= 0 && layerHandle < (long)m_allLayers.size())
	{
		Layer * layer = m_allLayers[layerHandle];
		if( layer->type == ShapefileLayer )
		{
			IShapefile* sf = NULL;
			layer->object->QueryInterface(IID_IShapefile, (void**)&sf);
			if (sf)
			{
				ShpfileType type;
				sf->get_ShapefileType(&type);
				sf->Release();
				return type;
			}
		}
		else
		{
			this->ErrorMessage(tkUNEXPECTED_LAYER_TYPE);
		}
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
	}
	return SHP_NULLSHAPE;
}

// ***************************************************************
//		get_ShapefileLayer()
// ***************************************************************
// Returns pointer to the shapefile layer with the given index, checks it's type
Layer* CMapView::get_ShapefileLayer(long layerHandle)
{
	if (layerHandle >= 0 && layerHandle < (long)m_allLayers.size())
	{
		Layer * layer = m_allLayers[layerHandle];
		if( layer->type == ShapefileLayer )
		{
			return layer;
		}
		else
		{
			this->ErrorMessage(tkUNEXPECTED_LAYER_TYPE);
			return NULL;
		}
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
		return NULL;
	}
}

// ***************************************************************
//		IsValidLayer()
// ***************************************************************
bool CMapView::IsValidLayer( long layerHandle )
{
	if (layerHandle >= 0 && layerHandle < (long)m_allLayers.size())
	{
		return (m_allLayers[layerHandle]!=NULL)?true:false;
	}
	else
	{
		return false;
	}
}

// ***************************************************************
//		IsValidShape()
// ***************************************************************
bool CMapView::IsValidShape( long layerHandle, long shape )
{	
	if( IsValidLayer(layerHandle) )
	{
		Layer * l = m_allLayers[layerHandle];
		if( l->type == ShapefileLayer )
		{	
			this->AlignShapeLayerAndShapes(l);
			
			IShapefile * ishp = NULL;
			//IDispatch * object = l->object;
			//object->QueryInterface(IID_IShapefile,(void**)&ishp);
			l->QueryShapefile(&ishp);

			long numShapes = 0;
			if( ishp == NULL )
			{
				numShapes = 0;
			}
			else
			{	
				ishp->get_NumShapes(&numShapes);
				ishp->Release();
				ishp = NULL;
			}
			
			if( shape >= 0 && shape < (long)numShapes )
			{
				return TRUE;
			}
			else
			{	
				ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
				return FALSE;
			}
		}
		else
		{	
			ErrorMessage(tkUNEXPECTED_LAYER_TYPE);
			return FALSE;
		}
	}
	else
	{
		ErrorMessage(tkINVALID_LAYER_HANDLE);
		return FALSE;
	}
	
}
#pragma endregion

#pragma region LayerProperties

#pragma region Points

// *****************************************************************
//		GetShapeLayerDrawPoint()
// *****************************************************************
BOOL CMapView::GetShapeLayerDrawPoint(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ShpfileType type = this->get_ShapefileType(LayerHandle);
		if (type == SHP_POINT || type == SHP_POINTM || type == SHP_POINTZ ||
			type == SHP_MULTIPOINT || type == SHP_MULTIPOINTZ || type == SHP_MULTIPOINTM)
		{
			return options->fillVisible?TRUE:FALSE;
		}
		else
		{
			return options->verticesVisible?TRUE:FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

// *****************************************************************
//		SetShapeLayerDrawPoint()
// *****************************************************************
void CMapView::SetShapeLayerDrawPoint(long LayerHandle, BOOL bNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	
	if (options)
	{
		ShpfileType type = this->get_ShapefileType(LayerHandle);
		if (type == SHP_POINT || type == SHP_POINTM || type == SHP_POINTZ ||
			type == SHP_MULTIPOINT || type == SHP_MULTIPOINTZ || type == SHP_MULTIPOINTM)
		{
			options->fillVisible = (bNewValue == TRUE);
			options->linesVisible = (bNewValue == TRUE);

		}
		else
		{
			options->verticesVisible = bNewValue;
		}
		m_canbitblt = FALSE;
	}
}

// *****************************************************************
//		GetShapeLayerPointSize()
// *****************************************************************
float CMapView::GetShapeLayerPointSize(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return options->pointSize;
	else
		return 0.0f;
}

// *****************************************************************
//		SetShapeLayerPointSize()
// *****************************************************************
void CMapView::SetShapeLayerPointSize(long LayerHandle, float newValue)
{
	if( newValue < 0.0 )
		newValue = 0.0;

	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{		
		options->pointSize = newValue;
		m_canbitblt = FALSE;
	}
}

// *****************************************************************
//		ShapeLayerPointType()
// *****************************************************************
short CMapView::GetShapeLayerPointType(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);	// TODO: write conversions between point types
		return ptSquare;
	}
	else
		return 0;
}

// *****************************************************************
//		ShapeLayerPointType()
// *****************************************************************
void CMapView::SetShapeLayerPointType(long LayerHandle, short nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);	// TODO: write conversions between point types	
	}
}	

// *****************************************************************
//		GetShapeLayerPointColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeLayerPointColor(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ShpfileType type = this->get_ShapefileType(LayerHandle);
		if (type == SHP_POINT || type == SHP_POINTM || type == SHP_POINTZ ||
			type == SHP_MULTIPOINT || type == SHP_MULTIPOINTZ || type == SHP_MULTIPOINTM)
		{
			return options->fillColor;
		}
		else
		{
			return options->verticesColor;
		}
	}
	else
	{
		return RGB(0,0,0);
	}
}

// *****************************************************************
//		ShapeLayerPointColor()
// *****************************************************************
void CMapView::SetShapeLayerPointColor(long LayerHandle, OLE_COLOR nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		ShpfileType type = this->get_ShapefileType(LayerHandle);
		if (type == SHP_POINT || type == SHP_POINTM || type == SHP_POINTZ ||
			type == SHP_MULTIPOINT || type == SHP_MULTIPOINTZ || type == SHP_MULTIPOINTM)
		{
			options->fillColor = nNewValue;
		}
		else
		{
			options->verticesColor = nNewValue;
		}
		m_canbitblt = FALSE;
	}
}

// *****************************************************************
//		GetUDPointType()
// *****************************************************************
LPDISPATCH CMapView::GetUDPointType(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		if (options->picture)
		{
			options->picture->AddRef();
			return options->picture;
		}
		else
			return NULL;
	}
	else
		return NULL;
}

// *****************************************************************
//		SetUDPointType()
// *****************************************************************
void CMapView::SetUDPointType(long LayerHandle, LPDISPATCH newValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		IImage * iimg = NULL;
		newValue->QueryInterface( IID_IImage, (void**)&iimg );
		Utility::put_ComReference((IDispatch*)iimg, (IDispatch**)&options->picture, true);
		if (iimg)
		{
			iimg->Release();
		}
	}
}
#pragma endregion

#pragma region Lines
// *****************************************************************
//		GetShapeLayerDrawLine()
// *****************************************************************
BOOL CMapView::GetShapeLayerDrawLine(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return (BOOL)options->linesVisible;
	else
		return FALSE;
}

// *****************************************************************
//		SetShapeLayerDrawLine()
// *****************************************************************
void CMapView::SetShapeLayerDrawLine(long LayerHandle, BOOL bNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->linesVisible = (bNewValue == TRUE);
		m_canbitblt = FALSE;
	}
}	

// ***********************************************************
//		GetShapeLayerLineColor()
// ***********************************************************
OLE_COLOR CMapView::GetShapeLayerLineColor(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return options->lineColor;
	else
		return RGB(0,0,0);
}

// *****************************************************************
//		SetShapeLayerLineColor()
// *****************************************************************
void CMapView::SetShapeLayerLineColor(long LayerHandle, OLE_COLOR nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->lineColor = nNewValue;
		m_canbitblt = FALSE;
	}
}

// *****************************************************************
//		GetShapeLayerLineWidth()
// *****************************************************************
float CMapView::GetShapeLayerLineWidth(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		return options->lineWidth;
	}
	else
	{
		return 0.0f;
	}
}

// *****************************************************************
//		ShapeLayerLineWidth()
// *****************************************************************
void CMapView::SetShapeLayerLineWidth(long LayerHandle, float newValue)
{
	if( newValue < 0.0 )
			newValue = 0.0;

	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->lineWidth = newValue;
		m_canbitblt = FALSE;
		if( !m_lockCount )
			InvalidateControl();
	}
}

// *****************************************************************
//		ShapeLayerLineStipple()
// *****************************************************************
short CMapView::GetShapeLayerLineStipple(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return (short)options->lineStipple;				// TODO: convert between enumerations
	else
		return -1;
}

// *****************************************************************
//		SetShapeLayerLineStipple()
// *****************************************************************
void CMapView::SetShapeLayerLineStipple(long LayerHandle, short nNewValue)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{	
		options->lineStipple = (tkDashStyle)nNewValue;	// TODO: convert between enumerations
		m_canbitblt = FALSE;
	}		
}

// *****************************************************************
//		GetUDLineStipple()
// *****************************************************************
long CMapView::GetUDLineStipple(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	return 0;
}

// *****************************************************************
//		UDLineStipple()
// *****************************************************************
void CMapView::SetUDLineStipple(long LayerHandle, long nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma region Fill

// *****************************************************************
//		GetShapeLayerDrawFill()
// *****************************************************************
BOOL CMapView::GetShapeLayerDrawFill(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return (BOOL)options->fillVisible;
	else
		return FALSE;
}

// *****************************************************************
//		SetShapeLayerDrawFill()
// *****************************************************************
void CMapView::SetShapeLayerDrawFill(long LayerHandle, BOOL bNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillVisible = (bNewValue == TRUE);
		m_canbitblt = FALSE;
	}
}

// *********************************************************
//		GetShapeLayerFillColor()
// *********************************************************
OLE_COLOR CMapView::GetShapeLayerFillColor(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return options->fillColor;
	else
		return RGB(0,0,0);
}

// **********************************************************
//		SetShapeLayerFillColor()
// **********************************************************
void CMapView::SetShapeLayerFillColor(long LayerHandle, OLE_COLOR nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillColor = nNewValue;
		m_canbitblt = FALSE;
	}
}

// *****************************************************************
//		GetShapeLayerFillTransparency
// *****************************************************************
float CMapView::GetShapeLayerFillTransparency(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		return options->fillTransparency/255.0f;
	}
	else
	{
		return 0.0f;
	}
}

// *****************************************************************
//		SetShapeLayerFillTransparency
// *****************************************************************
void CMapView::SetShapeLayerFillTransparency(long LayerHandle, float newValue)
{
	if( newValue < 0.0 )		newValue = 0.0;
	else if( newValue > 1.0 )	newValue = 1.0;

	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillTransparency = newValue * 255.0f;
		m_canbitblt = FALSE;
	}
}
#pragma endregion

#pragma region FillStipple
// *****************************************************************
//		GetShapeLayerFillStipple()
// *****************************************************************
short CMapView::GetShapeLayerFillStipple(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		return fsNone;			// TODO: write conversion between tkFillStipple and tkGDIPlusHatchStyle enumerations
	}
	else
	{
		return -1;
	}
}

// *****************************************************************
//		SetShapeLayerFillStipple()
// *****************************************************************
void CMapView::SetShapeLayerFillStipple(long LayerHandle, short nNewValue)
{
	ErrorMessage(tkPROPERTY_NOT_IMPLEMENTED);
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)	
	{
		m_canbitblt = FALSE;		// TODO: write conversion between tkFillStipple and tkGDIPlusHatchStyle enumerations
	}
}	

// *****************************************************************
//		GetShapeLayerStippleTransparent()
// *****************************************************************
BOOL CMapView::GetShapeLayerStippleTransparent(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return (BOOL)options->fillBgTransparent;
	else
		return TRUE;
}

// *****************************************************************
//		SetShapeLayerStippleTransparent()
// *****************************************************************
void CMapView::SetShapeLayerStippleTransparent(long LayerHandle, BOOL nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillBgTransparent = (nNewValue == TRUE ? true : false);
		m_canbitblt = FALSE;
	}
}

// *****************************************************************
//		GetShapeLayerStippleColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeLayerStippleColor(long LayerHandle)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
		return options->fillBgColor;
	else
		return RGB(0,0,0);
}

// *****************************************************************
//		SetShapeLayerStippleColor()
// *****************************************************************
void CMapView::SetShapeLayerStippleColor(long LayerHandle, OLE_COLOR nNewValue)
{
	CDrawingOptionsEx* options = get_ShapefileDrawingOptions(LayerHandle);
	if (options)
	{
		options->fillBgColor = nNewValue;
		m_canbitblt = FALSE;
	}
}

// *****************************************************************
//		GetUDFillStipple()
// *****************************************************************
long CMapView::GetUDFillStipple(long LayerHandle, long StippleRow)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0;
}

// *****************************************************************
//		SetUDFillStipple()
// *****************************************************************
void CMapView::SetUDFillStipple(long LayerHandle, long StippleRow, long nNewValue)
{
		ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma endregion
	
#pragma region REGION PerShapeDrawingOptions

#pragma region ShapeVisible
// *****************************************************************
//		GetShapeVisible()
// *****************************************************************
BOOL CMapView::GetShapeVisible(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return FALSE;
}

// *****************************************************************
//		SetShapeVisible()
// *****************************************************************
void CMapView::SetShapeVisible(long LayerHandle, long Shape, BOOL bNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma region Points

// *****************************************************************
//		GetShapeDrawPoint()
// *****************************************************************
BOOL CMapView::GetShapeDrawPoint(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return FALSE;
}

// *****************************************************************
//		SetShapeDrawPoint()
// *****************************************************************
void CMapView::SetShapeDrawPoint(long LayerHandle, long Shape, BOOL bNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapePointColor(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapePointColor()
// *****************************************************************
void CMapView::SetShapePointColor(long LayerHandle, long Shape, OLE_COLOR nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointSize()
// *****************************************************************
float CMapView::GetShapePointSize(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0.0f;
}

// *****************************************************************
//		SetShapePointSize()
// *****************************************************************
void CMapView::SetShapePointSize(long LayerHandle, long Shape, float newValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointType()
// *****************************************************************
short CMapView::GetShapePointType(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0;
}

// *****************************************************************
//		SetShapePointType()
// *****************************************************************
void CMapView::SetShapePointType(long LayerHandle, long Shape, short nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma region Lines

// *****************************************************************
//		GetShapeDrawLine()
// *****************************************************************
BOOL CMapView::GetShapeDrawLine(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return FALSE;
}

// *****************************************************************
//		SetShapeDrawLine()
// *****************************************************************
void CMapView::SetShapeDrawLine(long LayerHandle, long Shape, BOOL bNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeLineColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeLineColor(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapeLineColor()
// *****************************************************************
void CMapView::SetShapeLineColor(long LayerHandle, long Shape, OLE_COLOR nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeLineWidth()
// *****************************************************************
float CMapView::GetShapeLineWidth(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0.0f;
}

// *****************************************************************
//		SetShapeLineWidth()
// *****************************************************************
void CMapView::SetShapeLineWidth(long LayerHandle, long Shape, float newValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeLineStipple()
// *****************************************************************
short CMapView::GetShapeLineStipple(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}

// *****************************************************************
//		SetShapeLineStipple()
// *****************************************************************
void CMapView::SetShapeLineStipple(long LayerHandle, long Shape, short nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma region Fill

// *****************************************************************
//		GetShapeDrawFill()
// *****************************************************************
BOOL CMapView::GetShapeDrawFill(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapeDrawFill()
// *****************************************************************
void CMapView::SetShapeDrawFill(long LayerHandle, long Shape, BOOL bNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeFillColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeFillColor(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapeFillColor()
// *****************************************************************
void CMapView::SetShapeFillColor(long LayerHandle, long Shape, OLE_COLOR nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeFillTransparency()
// *****************************************************************
float CMapView::GetShapeFillTransparency(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0.0f;
}

// *****************************************************************
//		SetShapeFillTransparency()
// *****************************************************************
void CMapView::SetShapeFillTransparency(long LayerHandle, long Shape, float newValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion 

#pragma region FillStipple

// *****************************************************************
//		GetShapeFillStipple()
// *****************************************************************
short CMapView::GetShapeFillStipple(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}

// *****************************************************************
//		SetShapeFillStipple()
// *****************************************************************
void CMapView::SetShapeFillStipple(long LayerHandle, long Shape, short nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeStippleTransparent()
// *****************************************************************
BOOL CMapView::GetShapeStippleTransparent(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return TRUE;
}

// *****************************************************************
//		SetShapeStippleTransparent()
// *****************************************************************
void CMapView::SetShapeStippleTransparent(long LayerHandle, long Shape, BOOL nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapeStippleColor()
// *****************************************************************
OLE_COLOR CMapView::GetShapeStippleColor(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return RGB(220,220,220);
}

// *****************************************************************
//		SetShapeStippleColor()
// *****************************************************************
void CMapView::SetShapeStippleColor(long LayerHandle, long Shape, OLE_COLOR nNewValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma endregion

#pragma region PointImageList
// *****************************************************************
//		UDPointImageListCount()
// *****************************************************************
long CMapView::get_UDPointImageListCount(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return 0;
}

// *****************************************************************
//		get_UDPointImageListItem()
// *****************************************************************
IDispatch* CMapView::get_UDPointImageListItem(long LayerHandle, long ImageIndex)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return NULL;
}

// *****************************************************************
//		ClearUDPointImageList()
// *****************************************************************
void CMapView::ClearUDPointImageList(long LayerHandle)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		set_UDPointImageListAdd()
// *****************************************************************
long CMapView::set_UDPointImageListAdd(long LayerHandle, LPDISPATCH newValue)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}

	// *****************************************************************
//		SetShapePointImageListID()
// *****************************************************************
void CMapView::SetShapePointImageListID(long LayerHandle, long Shape, long ImageIndex)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointImageListID()
// *****************************************************************
long CMapView::GetShapePointImageListID(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}
#pragma endregion

#pragma region PointFontCharList
// *****************************************************************
//		set_UDPointFontCharListAdd()
// *****************************************************************
long CMapView::set_UDPointFontCharListAdd(long LayerHandle, long newValue, OLE_COLOR color)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}

// *****************************************************************
//		set_UDPointFontCharFont()
// *****************************************************************
void CMapView::set_UDPointFontCharFont(long LayerHandle, LPCTSTR FontName, float FontSize, BOOL isBold, BOOL isItalic, BOOL isUnderline)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		set_UDPointFontCharFontSize()
// *****************************************************************
void CMapView::set_UDPointFontCharFontSize(long LayerHandle,  float FontSize)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		SetShapePointFontCharListID()
// *****************************************************************
void CMapView::SetShapePointFontCharListID(long LayerHandle, long Shape, long FontCharIndex)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}

// *****************************************************************
//		GetShapePointFontCharListID()
// *****************************************************************
long CMapView::GetShapePointFontCharListID(long LayerHandle, long Shape)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	return -1;
}
#pragma endregion
	



