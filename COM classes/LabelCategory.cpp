/**************************************************************************************
 * File name: LabelCategory.cpp 
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Implementation of CLabelCategory
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
 // lsu: jan 2010 - created the file.

#include "stdafx.h"
#include "LabelCategory.h"
#include "cpl_minixml.h"
#include "cpl_string.h"

// *****************************************************************
//		get/put_Name
// *****************************************************************
STDMETHODIMP CLabelCategory::get_Name(BSTR* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = OLE2BSTR(m_name);
	return S_OK;
}
STDMETHODIMP CLabelCategory::put_Name(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	::SysFreeString(m_name);
	m_name = OLE2BSTR(newVal);
	return S_OK;
}

// *****************************************************************
//		get/put_Expression
// *****************************************************************
STDMETHODIMP CLabelCategory::get_Expression(BSTR* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = OLE2BSTR(m_expression);
	return S_OK;
}
STDMETHODIMP CLabelCategory::put_Expression(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	::SysFreeString(m_expression);
	m_expression = OLE2BSTR(newVal);
	return S_OK;
}

// *****************************************************************
//		get/put_StartValue
// *****************************************************************
STDMETHODIMP CLabelCategory::get_MinValue(VARIANT *pVal)
{
	VariantClear(pVal);
	VariantInit(pVal);
	VariantCopy(pVal,&m_minValue);
	return S_OK;
}

STDMETHODIMP CLabelCategory::put_MinValue(VARIANT newVal)
{
	VariantClear(&m_minValue);	
	VariantInit(&m_minValue);	
	VariantCopy(&m_minValue,&newVal);
	return S_OK;
}

// *****************************************************************
//		get/put_EndValue
// *****************************************************************
STDMETHODIMP CLabelCategory::get_MaxValue(VARIANT *pVal)
{
	VariantClear(pVal);
	VariantInit(pVal);
	VariantCopy(pVal,&m_maxValue);
	return S_OK;
}
STDMETHODIMP CLabelCategory::put_MaxValue(VARIANT newVal)
{
	VariantClear(&m_maxValue);
	VariantInit(&m_maxValue);	
	VariantCopy(&m_maxValue,&newVal);
	return S_OK;
}


// ////////////////////////////////////////////////////////////// //
//	PROPERTIES COMMON FOR CLABELS AND CLABELCATEGORY
//	CAN BE COPIED TO THE OTHER CLASS; DON'T BREAK THIS BLOCK
// ////////////////////////////////////////////////////////////// //

// *****************************************************************
//		FontName()
// *****************************************************************
STDMETHODIMP CLabelCategory::get_FontName(BSTR* retval)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = OLE2BSTR(m_options.fontName);		
	return S_OK;
};	
STDMETHODIMP CLabelCategory::put_FontName(BSTR newVal)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	::SysFreeString(m_options.fontName);
	m_options.fontName = OLE2BSTR(newVal);
	return S_OK;
};	

// *****************************************************************
//		Font/FrameTransparency()
// *****************************************************************
STDMETHODIMP CLabelCategory::get_FontTransparency(long* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = m_options.fontTransparency;		
	return S_OK;
};		
STDMETHODIMP CLabelCategory::put_FontTransparency(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (newVal < 0) newVal = 0;
	if (newVal > 255) newVal = 255;
	m_options.fontTransparency = newVal;
	return S_OK;
};
STDMETHODIMP CLabelCategory::get_FrameTransparency(long* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = m_options.frameTransparency;		
	return S_OK;
};		
STDMETHODIMP CLabelCategory::put_FrameTransparency(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (newVal < 0) newVal = 0;
	if (newVal > 255) newVal = 255;
	m_options.frameTransparency = newVal;
	return S_OK;
};

// *****************************************************************
//			Font style options
// *****************************************************************
STDMETHODIMP CLabelCategory::get_FontItalic(VARIANT_BOOL* retval)
{
	*retval = ((m_options.fontStyle & fstItalic)?VARIANT_TRUE:VARIANT_FALSE);
	return S_OK;
};
STDMETHODIMP CLabelCategory::put_FontItalic(VARIANT_BOOL newVal)
{
	if (newVal)		m_options.fontStyle |= fstItalic;
	else			m_options.fontStyle &= (0xFFFFFFFF ^ fstItalic);
	return S_OK;
};
STDMETHODIMP CLabelCategory::get_FontBold(VARIANT_BOOL* retval)
{
	*retval = ((m_options.fontStyle & fstBold)?VARIANT_TRUE:VARIANT_FALSE);
	return S_OK;
};
STDMETHODIMP CLabelCategory::put_FontBold(VARIANT_BOOL newVal)
{
	if (newVal)		m_options.fontStyle |= fstBold;
	else			m_options.fontStyle &= (0xFFFFFFFF ^ fstBold);
	return S_OK;
};
STDMETHODIMP CLabelCategory::get_FontUnderline(VARIANT_BOOL* retval)
{
	*retval = ((m_options.fontStyle & fstUnderline)?VARIANT_TRUE:VARIANT_FALSE);
	return S_OK;
};
STDMETHODIMP CLabelCategory::put_FontUnderline(VARIANT_BOOL newVal)
{
	if (newVal)		m_options.fontStyle |= fstUnderline;
	else			m_options.fontStyle &= (0xFFFFFFFF ^ fstUnderline);
	return S_OK;
};
STDMETHODIMP CLabelCategory::get_FontStrikeOut(VARIANT_BOOL* retval)
{
	*retval = ((m_options.fontStyle & fstStrikeout)?VARIANT_TRUE:VARIANT_FALSE);
	return S_OK;
};
STDMETHODIMP CLabelCategory::put_FontStrikeOut(VARIANT_BOOL newVal)
{
	if (newVal)		m_options.fontStyle |= fstStrikeout;
	else			m_options.fontStyle &= (0xFFFFFFFF ^ fstStrikeout);
	return S_OK;
};
////////////////////////////////////////////////////////////////
//      END OF COMMON OPTIONS
////////////////////////////////////////////////////////////////

//// ***********************************************************
////		LabelOptions
//// ***********************************************************
//// Returning class options as pointer to CLabelInfo struture for easier access in CMapView
//// Meant for inner use only
//STDMETHODIMP CLabelCategory::get_LabelOptions(BYTE** pVal)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	*pVal = reinterpret_cast<BYTE*>(&m_options);
//	return S_OK;
//};
//
//// ***********************************************************
////		LabelOptions
//// ***********************************************************
//// Passing pointer to CLabelInfo structure of parent class
//// Meant for inner use only
//STDMETHODIMP CLabelCategory::put_LabelOptions(BYTE* pVal)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	CLabelOptions* lblInfo = reinterpret_cast<CLabelOptions*>(pVal);
//	m_options =	*lblInfo;	// redefined operator
//	return S_OK;
//};

// *************************************************************
//		get_LabelOptions
// *************************************************************
CLabelOptions* CLabelCategory::get_LabelOptions()
{
	return &m_options;
}
void CLabelCategory::put_LabelOptions(CLabelOptions* newVal)
{
	m_options =	*newVal;	// redefined operator
}

// ********************************************************
//     Serialize()
// ********************************************************
STDMETHODIMP CLabelCategory::Serialize(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	CPLXMLNode* node = SerializeCore("LabelCategoryClass");
	if (node)
	{
		CString str = CPLSerializeXMLTree(node);
		CPLDestroyXMLNode(node);
		*retVal = A2BSTR(str);
	}
	else
	{
		*retVal = A2BSTR("");
	}
	return S_OK;
}

// ********************************************************
//     Serialize()
// ********************************************************
CPLXMLNode* CLabelCategory::SerializeCore(CString ElementName)
{
	CPLXMLNode* node = m_options.SerializeToTree(ElementName);
	if (node)
	{
		// non-drawing properties
		USES_CONVERSION;
		CString str = OLE2CA(m_name);
		Utility::CPLCreateXMLAttributeAndValue( node, "Name", str);
		
		str = OLE2CA(m_expression);
		Utility::CPLCreateXMLAttributeAndValue( node, "Expression", str);
		Utility::CPLCreateXMLAttributeAndValue( node, "Enabled", CPLString().Printf("%d", (int)m_enabled ));
		
		// TODO: do we really need to serialize these ?
		//VARIANT m_minValue;
		//VARIANT m_maxValue;
	}
	return node;
}

// ********************************************************
//     Deserialize()
// ********************************************************
STDMETHODIMP CLabelCategory::Deserialize(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	CString s = OLE2CA(newVal);
	CPLXMLNode* node = CPLParseXMLString(s.GetString());
	this->DeserializeFromNode(node);
	CPLDestroyXMLNode(node);
	return S_OK;
}

// ********************************************************
//     DeserializeFromNode()
// ********************************************************
void CLabelCategory::DeserializeFromNode(CPLXMLNode* node)
{
	if (!node)
		return;
	
	m_options.DeserializeFromNode(node);

	// non-drawing properties
	CString s = CPLGetXMLValue( node, "Name", NULL );
	if (s != "")
	{
		SysFreeString(m_name);
		m_name = A2BSTR(s);
	}

	s = CPLGetXMLValue( node, "Expression", NULL );
	if (s != "")
	{
		s.Replace("&#xA;", "\r\n");
		SysFreeString(m_expression);
		m_expression = A2BSTR(s);
	}

	s = CPLGetXMLValue( node, "Enabled", NULL );
	if (s != "") m_enabled = (VARIANT_BOOL)atoi(s.GetString());
}

