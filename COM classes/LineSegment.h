/**************************************************************************************
 * File name: LineSegment.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Declaration of CLineSegment
 *
 **************************************************************************************
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * (the "License"); you may not use this file except in compliance with 
 * the License. You may obtain a copy of the License at http://www.mozilla.org/mpl/ 
 * See the License for the specific language governing rights and limitations
 * under the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ************************************************************************************** 
 * Contributor(s): 
 * (Open source contributors should list themselves and their modifications here). */
 // Sergei Leschinski (lsu) 18 august 2010 - created the file.

#pragma once
#include "MapWinGis.h"
#include "Gdiplus.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// -------------------------------------------------------
// CLineSegment interface
// -------------------------------------------------------
class ATL_NO_VTABLE CLineSegment :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLineSegment, &CLSID_LineSegment>,
	public IDispatchImpl<ILineSegment, &IID_ILineSegment, &LIBID_MapWinGIS, /*wMajor =*/ VERSION_MAJOR, /*wMinor =*/ VERSION_MINOR>
{
public:
	
	CLineSegment()
	{
		_lineType = lltSimple;
		_lineColor = RGB(0,0,0);
		_markerOutlineColor = RGB(120, 120, 120);
		_lineWidth = 1.0f;
		_lineStyle = dsSolid;
		_marker = dpsDiamond;
		_markerSize = 12;
		_markerInterval = 16;
		_markerOffset = 0.0f;
		_markerOrientation = lorParallel;
		_markerFlipFirst = VARIANT_TRUE;
		CoCreateInstance( CLSID_ShapeDrawingOptions, NULL, CLSCTX_INPROC_SERVER, IID_IShapeDrawingOptions, (void**)&_options );
	}

	~CLineSegment()
	{
		if (_options)
		{
			_options->Release();
			_options = NULL;
		}
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_LINESEGMENT)

	BEGIN_COM_MAP(CLineSegment)
		COM_INTERFACE_ENTRY(ILineSegment)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(get_LineType)(tkLineType* retVal);
	STDMETHOD(put_LineType)(tkLineType newVal);
	STDMETHOD(get_Color)(OLE_COLOR* retVal);
	STDMETHOD(put_Color)(OLE_COLOR newVal);
	STDMETHOD(get_LineWidth)(float* retVal);
	STDMETHOD(put_LineWidth)(float newVal);
	STDMETHOD(get_LineStyle)(tkDashStyle* retVal);
	STDMETHOD(put_LineStyle)(tkDashStyle newVal);
	// marker line
	STDMETHOD(get_Marker)(tkDefaultPointSymbol* retVal);
	STDMETHOD(put_Marker)(tkDefaultPointSymbol newVal);
	STDMETHOD(get_MarkerSize)(float* retVal);
	STDMETHOD(put_MarkerSize)(float newVal);
	STDMETHOD(get_MarkerInterval)(float* retVal);
	STDMETHOD(put_MarkerInterval)(float newVal);
	STDMETHOD(get_MarkerOrientation)(tkLineLabelOrientation* retVal);
	STDMETHOD(put_MarkerOrientation)(tkLineLabelOrientation newVal);
	STDMETHOD(get_MarkerFlipFirst)(VARIANT_BOOL* retVal);
	STDMETHOD(put_MarkerFlipFirst)(VARIANT_BOOL newVal);
	STDMETHOD(get_MarkerOffset)(float* retVal);
	STDMETHOD(put_MarkerOffset)(float newVal);
	STDMETHOD(get_MarkerOutlineColor)(OLE_COLOR* retVal);
	STDMETHOD(put_MarkerOutlineColor)(OLE_COLOR newVal);

	STDMETHOD(Draw)(int** hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal);
	STDMETHOD(DrawVB)(int hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal);
	VARIANT_BOOL DrawCore(CDC* dc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor);
	STDMETHOD(Draw)(Gdiplus::Graphics& g, BYTE transparency, int ImageWidth, int ImageHeight, int xOrigin, int yOrigin, VARIANT_BOOL* retVal);

private:
	tkLineType _lineType;
	OLE_COLOR _lineColor;
	float _lineWidth;
	tkDashStyle _lineStyle;
	//tkDashStyle _lineStiple;
	tkDefaultPointSymbol _marker;
	float _markerSize;
	float _markerInterval;
	float _markerOffset;
	OLE_COLOR _markerOutlineColor;
	tkLineLabelOrientation _markerOrientation;
	VARIANT_BOOL _markerFlipFirst;
	IShapeDrawingOptions* _options;
};
OBJECT_ENTRY_AUTO(__uuidof(LineSegment), CLineSegment)
