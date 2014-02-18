// MapWinGIS.cpp : Implementation of CMapWinGISApp and DLL registration.
#include "stdafx.h"
#include <initguid.h>
#include "ShapefileColorScheme.h"
#include "ShapefileColorBreak.h"
#include "cpl_conv.h" 
#include "cpl_string.h"
#include "SQLiteCache.h"
#include "RamCache.h"

#ifdef _DEBUG
#include "gdal.h"
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CMapWinGISModule :
	public ATL::CAtlMfcModule
{
public:
	DECLARE_LIBID(LIBID_MapWinGIS);
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MAPWINGIS, "{8308CC9E-4AEF-4D31-9081-86CD61B9E641}");
};

const GUID CDECL BASED_CODE _tlid = { 0xc368d713, 0xcc5f, 0x40ed, { 0x9f, 0x53, 0xf8, 0x4f, 0xe1, 0x97, 0xb9, 0x6a } };
const WORD _wVerMajor = 4;
const WORD _wVerMinor = 9;

CMapWinGISApp NEAR theApp;
CMapWinGISModule _AtlModule;
GlobalClassFactory m_factory;	// make sure that this one is initialized after the _Module above

// ******************************************************
// CMapWinGISApp::InitInstance - DLL initialization
// ******************************************************
BOOL CMapWinGISApp::InitInstance()
{
	//Neio modified 2009, following http://www.mapwindow.org/phorum/read.php?7,12162 by gischai, for multi-language support
	//std::locale::global(std::locale(""));
	//19-Oct-09 Rob Cairns: (See Bug 1446) - I hate doing this if it prevents our Chinese friends opening Chinese character shapefiles and data.
	//However, there are just too many bugs associated with this change. See Bug 1446 for more information. Changing back to classic.
	std::locale::global(std::locale("C"));
	
	// initialize all static variables, to keep our memory leaking report clean from them
	#ifdef MEMLEAK
		gMemLeakDetect.stopped = true;
		GDALAllRegister();
		gMemLeakDetect.stopped = false;
	#endif

	// UTF8 string are expected by default; the enviroment variable shoud be set to restore older behavior
	// see more details here: http://trac.osgeo.org/gdal/wiki/ConfigOptions
	if( CSLTestBoolean(CPLGetConfigOption( "GDAL_FILENAME_IS_UTF8", "YES" ) ) )
	{
		CPLSetConfigOption( "GDAL_FILENAME_IS_UTF8", "NO" );
	}
		
	m_utils = NULL;
	return COleControlModule::InitInstance();
}

// *****************************************************
// CMapWinGISApp::ExitInstance - DLL termination
// *****************************************************
int CMapWinGISApp::ExitInstance()
{
	//if (false)	{ AFX_MANAGE_STATE(NULL);}
	
	if (m_utils)
	{
		m_utils->Release();
	}
	
	// both caches are stored in static variables; 
	// we clear them explicitly to be able to use memory leaking detection tool;
	// otherwise the tool reports hundreds of false leaks; so it's difficult to find relevant ones
	RamCache::Close();
	SQLiteCache::Close();

	_AtlModule.Term();
	return COleControlModule::ExitInstance();
}

// **************************************************************
// DllRegisterServer - Adds entries to the system registry
// **************************************************************
STDAPI DllRegisterServer(void)
{
	_AtlModule.UpdateRegistryAppId(TRUE);
	HRESULT hRes2 = _AtlModule.RegisterServer(TRUE);
	if (hRes2 != S_OK)
		return hRes2;
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	

	return NOERROR;
}

// **************************************************************
// DllUnregisterServer - Removes entries from the system registry
// **************************************************************
STDAPI DllUnregisterServer(void)
{
	_AtlModule.UpdateRegistryAppId(FALSE);
	HRESULT hRes2 = _AtlModule.UnregisterServer(TRUE);
	if (hRes2 != S_OK)
		return hRes2;
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}

// **************************************************************
//		DllCanUnloadNow()
// **************************************************************
STDAPI DllCanUnloadNow(void)
{
	if (_AtlModule.GetLockCount() > 0)
		return S_FALSE;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// ******************************************************************
// Returns a class factory to create an object of the requested type
// ******************************************************************
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
#ifdef MEMLEAK
	bool state = gMemLeakDetect.stopped;
	gMemLeakDetect.stopped = true;
#endif

	HRESULT hres;
	if (S_OK == _AtlModule.GetClassObject(rclsid, riid, ppv))
		hres = S_OK;
	else if(AfxDllGetClassObject(rclsid, riid, ppv) == S_OK)
		hres = S_OK;
	else
		hres = S_FALSE;
	
#ifdef MEMLEAK
	gMemLeakDetect.stopped = state;
#endif

	return hres;
}

// *****************************************************************
//		Object map
// *****************************************************************
// Provides support for the registration, initialization, and creation of instances of ATL COM classes
// According to MSDN this macro is obsolete. OBJECT_ENTRY_AUTO should be used instead
BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_ShapefileColorScheme, CShapefileColorScheme)
	OBJECT_ENTRY(CLSID_ShapefileColorBreak, CShapefileColorBreak)
END_OBJECT_MAP()
