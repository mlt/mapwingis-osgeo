// MapWinGIS.h : main header file for MAPWINGIS.DLL
#pragma once

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"
#include "errorcodes.h"
#include "MapWinGIS_i.h"
#include "GlobalClassFactory.h"

#define VERSION_MAJOR 4
#define VERSION_MINOR 9
#define FORCE_NEW_LABELS true	// new labels will be used regardless of the mode

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

// **********************************************************
//		CMapWinGISApp class
// **********************************************************
class CMapWinGISApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern GlobalClassFactory m_factory;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
