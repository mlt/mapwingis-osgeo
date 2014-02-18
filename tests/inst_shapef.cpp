//#include "MapWinGIS_i.h"
//#import "C:\OSGeo4W\bin\MapWinGIS.ocx" high_property_prefixes("get", "put", "putRef")
#import "libid:C368D713-CC5F-40ED-9F53-F84FE197B96A" high_property_prefixes("get", "put", "putRef") //named_guids
//#include "MapWinGIS.tlh"

//int test() {
//	MapWinGIS::IShapefilePtr ShpFile;
//	return FAILED( ShpFile.CreateInstance(__uuidof(MapWinGIS::Shapefile)) );
//}

int main(int argc, char* argv[]) {
	::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	int err;
//	{
	MapWinGIS::IShapefilePtr ShpFile;// = new MapWinGIS::IShapefilePtr;
//	err = FAILED( ShpFile.CreateInstance("MAPWINGIS.Shapefile") );
	err = FAILED( ShpFile.CreateInstance(__uuidof(MapWinGIS::Shapefile)) );
	//int err = FAILED( ShpFile->CreateInstance(__uuidof(MapWinGIS::Shapefile)) );
//	int err = test();
	//MapWinGIS::ITilesPtr *tiles = new MapWinGIS::ITilesPtr;
	//err = FAILED( tiles->CreateInstance("MAPWINGIS.Tiles") );
//	}
	if (err)
		MessageBox(0, "Failed to instantiate IShapefile", NULL, MB_OK | MB_ICONERROR);
	else
		MessageBox(0, "Great!", NULL, MB_OK);
	::CoUninitialize();
	return err;
}
