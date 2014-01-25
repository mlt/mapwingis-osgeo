/**************************************************************************************
 * File name: ShapePoint.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Implementation of CShapefileDrawer
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
 // Sergei Leschinski (lsu) 8 feb 2011 - created the file

#include "stdafx.h"
#include "ShapePoint.h"

#pragma region CShapePoint

// *******************************************************
//		get_Point()
// *******************************************************
IPoint* CShapePoint::get_Point()
{
	IPoint* pnt = NULL;
	CoCreateInstance( CLSID_Point, NULL, CLSCTX_INPROC_SERVER, IID_IPoint, (void**)&pnt);
	return pnt;
}

// *******************************************************
//		put_Point()
// *******************************************************
bool CShapePoint::put_Point(IPoint* newPoint)
{
	if (!newPoint)
	{
		return false;
	}
	else
	{
		newPoint->get_X(&x);
		newPoint->get_Y(&y);
		newPoint->get_Z(&z);
		newPoint->get_M(&m);
		return true;
	}
}

// *******************************************************
//		get_XY()
// *******************************************************
void CShapePoint::get_XY(double& X, double& Y)
{
	X = x;
	Y = y;
}

// *******************************************************
//		put_XY()
// *******************************************************
void CShapePoint::put_XY(double X, double Y)
{
	x = X;
	y = Y;
}
#pragma endregion

#pragma region ShapePointCOM

// *******************************************************
//		get_Point()
// *******************************************************
IPoint* CShapePointCOM::get_Point()
{
	if (pnt)
		pnt->AddRef();
	return pnt;
}

// *******************************************************
//		put_Point()
// *******************************************************
bool CShapePointCOM::put_Point(IPoint* newPoint)
{
	if (newPoint)
	{
		if (pnt)
		{
			pnt->Release();
			pnt = NULL;
		}
		pnt = newPoint;
		pnt->AddRef();
		return true;
	}
	else
	{
		return false;
	}
}

// *******************************************************
//		get_XY()
// *******************************************************
void CShapePointCOM::get_XY(double& x, double& y)
{
	pnt->get_X(&x);
	pnt->get_Y(&y);
}

// *******************************************************
//		put_XY()
// *******************************************************
void CShapePointCOM::put_XY(double x, double y)
{
	pnt->put_X(x);
	pnt->put_Y(y);
}

// *******************************************************
//		get_Key()
// *******************************************************
void CShapePointCOM::get_Key(BSTR* pVal)
{
	pnt->get_Key(pVal);
}
void CShapePointCOM::put_Key(BSTR newVal)
{
	pnt->put_Key(newVal);
}

// *******************************************************
//		get_X()
// *******************************************************
double CShapePointCOM::get_X()
{
	double x;
	return pnt->get_X(&x);
}
double CShapePointCOM::get_Y()
{
	double y;
	return pnt->get_Y(&y);
}
double CShapePointCOM::get_Z()
{
	double z;
	return pnt->get_Z(&z);
}
double CShapePointCOM::get_M()
{
	double m;
	return pnt->get_M(&m);
}
#pragma endregion