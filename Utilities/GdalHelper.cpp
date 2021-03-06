#include "stdafx.h"
#include "GdalHelper.h"

// **************************************************************
//		GdalOpen
// **************************************************************
GDALDataset* GdalHelper::OpenDatasetA( char* filenameUtf8 )
{
	m_globalSettings.SetGdalUtf8(true);

	GDALAllRegister();
	GDALDataset* dt = (GDALDataset *) GDALOpen(filenameUtf8, GA_Update );

	if (dt == NULL) 
		dt = (GDALDataset *) GDALOpen(filenameUtf8, GA_ReadOnly );

	m_globalSettings.SetGdalUtf8(false);

	return dt;
}

GDALDataset* GdalHelper::OpenDatasetA( CStringA& filenameUtf8 )
{
	return OpenDatasetA(filenameUtf8.GetBuffer());
}

GDALDataset* GdalHelper::OpenDatasetW( CStringW filenameW )
{
	CStringA filenameA = Utility::ConvertToUtf8(filenameW);
	return OpenDatasetA(filenameA.GetBuffer());
}

GDALDataset* GdalHelper::OpenDatasetA(CStringA& filenameUtf8, GDALAccess accessType )
{
	return OpenDatasetA(filenameUtf8.GetBuffer(), accessType);
}

GDALDataset* GdalHelper::OpenDatasetA(char* filenameUtf8, GDALAccess accessType )
{
	m_globalSettings.SetGdalUtf8(true);

	GDALAllRegister();
	GDALDataset* dt = (GDALDataset *) GDALOpen(filenameUtf8, accessType );

	m_globalSettings.SetGdalUtf8(false);

	return dt;
}

GDALDataset* GdalHelper::OpenDatasetW(CStringW filenameW, GDALAccess accessType )
{
	CStringA filenameA = Utility::ConvertToUtf8(filenameW);
	return OpenDatasetA(filenameA.GetBuffer(), accessType);
}

// **************************************************************
//		CPLParseXMLFile
// **************************************************************
bool GdalHelper::IsRgb(GDALDataset* dt)
{
	bool isRgb = false;
	if (dt)
	{
		bool r, g, b;
		r = g = b = false;
		int numBands = dt->GetRasterCount();
		for(int i = 1; i <= numBands; i++)
		{
			GDALRasterBand* band = dt->GetRasterBand(i);
			if (band)
			{
				GDALColorInterp ci = band->GetColorInterpretation();
				if (ci == GCI_RedBand) r = true;
				if (ci == GCI_GreenBand) g = true;
				if (ci == GCI_BlueBand) b = true;
			}
		}
		isRgb = r && g && b;
	}
	return isRgb;
}

// **************************************************************
//		CPLParseXMLFile
// **************************************************************
CPLXMLNode* GdalHelper::ParseXMLFile(CStringW filename)
{
	CStringA nameA = Utility::ConvertToUtf8(filename);
	m_globalSettings.SetGdalUtf8(true);
	CPLXMLNode* node = CPLParseXMLFile(nameA);
	m_globalSettings.SetGdalUtf8(false);
	return node;
}

// **************************************************************
//		SerializeXMLTreeToFile
// **************************************************************
int GdalHelper::SerializeXMLTreeToFile(CPLXMLNode* psTree, CStringW filename)
{
	CStringA nameA = Utility::ConvertToUtf8(filename);	
	m_globalSettings.SetGdalUtf8(true);
	int val = CPLSerializeXMLTreeToFile(psTree, nameA);
	m_globalSettings.SetGdalUtf8(false);
	return val;
}

// **************************************************************
//		CloseDataset
// **************************************************************
void GdalHelper::CloseDataset(GDALDataset* dt)
{
	if (dt)
	{
		int count = dt->Dereference();
		dt->Reference();
		if (count > 0)
			Debug::WriteLine("References remain on closing dataset: %d", count);
		GDALClose(dt);
	}
}

// **************************************************************
//		CanOpenWithGdal
// **************************************************************
bool GdalHelper::CanOpenWithGdal(CStringW filename)
{
	GDALAllRegister();
	GDALDataset* dt = GdalHelper::OpenDatasetW(filename, GDALAccess::GA_ReadOnly);
	bool gdalFormat = dt != NULL;
	if (dt)
	{
		dt->Dereference();
		delete dt;
		dt = NULL;
	}
	return gdalFormat;
}

bool ClearOverviews(GDALDataset* dt, ICallback* cb, bool clear)
{
	int overviewList = clear ? 0 : 2;
	bool success = dt->BuildOverviews("NONE", (clear ? 0 : 1), &overviewList, 0, NULL, (GDALProgressFunc)GDALProgressFunction, cb) == CPLErr::CE_None;
	return success;
}

// **************************************************************
//		SupportsOverviews
// **************************************************************
bool GdalHelper::SupportsOverviews(CStringW filename, ICallback* callback)
{
	GDALAllRegister();
	bool supports = false;
	GDALDataset* dt = OpenDatasetW(filename, GDALAccess::GA_ReadOnly);
	if (dt) {
		supports = HasOverviews(dt);
		if (!supports) {
			// TODO: can we check just by running clear?
			supports = ClearOverviews(dt, callback, true);
			// otherwise it's seems that the only way to check is try to create them
			// TODO: check driver to weed out formats which don't support overviews
		}
		CloseDataset(dt);
	}
	return supports;
}

// **************************************************************
//		TryOpenWithGdal
// **************************************************************
GdalSupport GdalHelper::TryOpenWithGdal(CStringW filename)
{
	GDALAllRegister();
	GDALDataset* dt = GdalHelper::OpenDatasetW(filename, GDALAccess::GA_ReadOnly);
	if (!dt) {
		return GdalSupport::GdalSupportNone;
	}
	else
	{
		bool isRgb = IsRgb(dt);
		dt->Dereference();
		delete dt;
		dt = NULL;
		return isRgb ? GdalSupportRgb : GdalSupportGrid;
	}
}

// **************************************************************
//		GDALProgressFunction
// **************************************************************
int CPL_STDCALL GDALProgressFunction( double dfComplete, const char* pszMessage, void *pData)
{
	if( pData != NULL )
	{
		long percent = long(dfComplete * 100.0);
		ICallback* cback = (ICallback*)pData;
		if (cback) {
			cback->Progress(NULL,percent, A2BSTR("Building overviews"));
		}
	}
	return TRUE;
}

// *******************************************************
//		HasOverviews()
// *******************************************************
bool GdalHelper::HasOverviews(GDALDataset* dt) 
{
	if (dt) {
		GDALRasterBand* band = dt->GetRasterBand(1);
		if (band)
		{
			int numOverviews = band->GetOverviewCount();
			return numOverviews > 0;
		}
	}
	return false;
}

// *******************************************************
//		RemoveOverviews()
// *******************************************************
bool GdalHelper::RemoveOverviews(CStringW filename) 
{
	GDALDataset* dt = GdalHelper::OpenDatasetW(filename, GDALAccess::GA_ReadOnly);
	if (dt ) {
		bool result = ClearOverviews(dt, NULL, true);
		CloseDataset(dt);
		return result;
	}
	return false;
}

// *******************************************************
//		HasOverviews()
// *******************************************************
bool GdalHelper::HasOverviews(CStringW filename) 
{
	GDALDataset* dt = GdalHelper::OpenDatasetW(filename);
	if (dt ) {
		bool hasOverviews = GdalHelper::HasOverviews(dt);
		GdalHelper::CloseDataset(dt);
		return hasOverviews;
	}
	return false;
}

// *******************************************************
//		NeedsOverviews()
// *******************************************************
bool GdalHelper::NeedsOverviews(GDALDataset* dt)
{
	if (dt) {
		int w = dt->GetRasterXSize();
		int h = dt->GetRasterYSize();
		return w > m_globalSettings.minOverviewWidth || h > m_globalSettings.minOverviewWidth;
	}
	return false;
}

// *******************************************************
//		BuildOverviewsIfNeeded()
// *******************************************************
bool GdalHelper::BuildOverviewsIfNeeded(CStringW filename, bool external, ICallback* callback) 
{
	//if (!external)
		//CPLSetConfigOption( "COMPRESS_OVERVIEW", m_globalSettings.GetTiffCompression());

	// dataset must be opened in read-only mode, so that overviews are written in external ovr file
	GDALAccess accessMode = external ? GDALAccess::GA_ReadOnly : GDALAccess::GA_Update;
	GDALDataset* dt = GdalHelper::OpenDatasetW(filename, accessMode);
	if (dt) {
		GdalHelper::BuildOverviewsIfNeeded(dt, callback);
		GdalHelper::CloseDataset(dt);
		return true;
	}
	else {
		return false;
	}
}

// *******************************************************
//		BuildOverviewsIfNeeded()
// *******************************************************
void GdalHelper::BuildOverviewsIfNeeded(GDALDataset* dt, ICallback* callback) 
{
	if (m_globalSettings.rasterOverviewCreation == rocAuto || 
		m_globalSettings.rasterOverviewCreation == rocYes)
	{
		if (dt && !GdalHelper::HasOverviews(dt) && NeedsOverviews(dt))
		{
			int w = dt->GetRasterXSize() / 2;
			int h = dt->GetRasterYSize() / 2;
			
			int ratio = 2;
			std::vector<int> overviews;
			while(w > m_globalSettings.minOverviewWidth || h > m_globalSettings.minOverviewWidth)
			{
				overviews.push_back(ratio);
				w /= 2;
				h /= 2;
				ratio *= 2;
			}

			bool result = BuildOverviewsCore(dt, m_globalSettings.rasterOverviewResampling, &(overviews[0]), overviews.size(), callback);
		}
	}
	return;
}

// *******************************************************
//		BuildOverviewsCore()
// *******************************************************
bool GdalHelper::BuildOverviewsCore(GDALDataset* dt, tkGDALResamplingMethod resamlingMethod, int* overviewList, int numOverviews, ICallback* callback) 
{
	if (dt)
	{
		if (numOverviews == 0) {
			Debug::WriteLine("ERROR: no overviews passed to BuildOverviewsCore");
			return false;
		}

		const char* pszResampling;
		switch(resamlingMethod)			//"MODE", "AVERAGE_MAGPHASE"
		{
			case grmAverage: 
				pszResampling = "AVERAGE"; break;
			case grmBicubic: 
				pszResampling = "CUBIC"; break;
			case grmGauss:	
				pszResampling = "GAUSS"; break;
			case grmNearest: 
				pszResampling = "NEAREST"; break;
			default: 
				pszResampling = "NONE";
		}

		if (dt->BuildOverviews(pszResampling, numOverviews, overviewList, 0, NULL, (GDALProgressFunc)GDALProgressFunction, callback) == CE_None)
			return true;

		Utility::DisplayProgressCompleted(callback);
	}
	return false;
}
