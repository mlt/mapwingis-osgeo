/**************************************************************************************
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
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
 // lsu 17 apr 2012 - created the file

#pragma once
#include <list>
#include "baseprovider.h"
#include "tilecacher.h"

#define THREADPOOL_SIZE	5

// A point with X, Y coordinates and distance from center of screen
class CTilePoint: public CPoint
{
public:
	double dist;
	CTilePoint(int x, int y)
		: CPoint(x, y), dist(0.0) {}
};
bool compPoints(CTilePoint* p1, CTilePoint* p2);

// Handles the loading queue of map tiles, schedules the loaded tiles for caching
class TileLoader
{
private:
	// 2 pools will be used in alternate fashion,
	// as it's problematic to terminate threads correctly until their current task is finished
	// but it's quite desirable to start the processing of new requests ASAP
	CThreadPool<ThreadWorker>* m_pool;
	CThreadPool<ThreadWorker>* m_pool2;
	list<void*> m_tasks;
	TileCacher m_sqliteCacher;
	TileCacher m_diskCacher;
	void* tiles;
	bool isSnapshot;
	CString key;
	void CleanTasks();
	std::list<void*> _activeTasks;	// http requests are performed
	::CCriticalSection _activeTasksLock;
public:
	long m_sleepBeforeRequestTimeout;
	int m_errorCount;
	int m_totalCount;
	int m_sumCount;
	int m_count;
	static ::CCriticalSection section;
	static ::CCriticalSection _requestLock;

	ICallback* m_callback;			 // to report progress to clients via COM interface
	IStopExecution* m_stopCallback;	 // to stop execution by clients via COM interface
	int tileGeneration;
	bool stopped;
	bool doCacheSqlite;
	bool doCacheDisk;

	TileLoader::TileLoader()
	{
		m_diskCacher.cacheType = CacheType::DiskCache;
		m_sqliteCacher.cacheType = CacheType::SqliteCache;
		tileGeneration = 0;
		m_pool = NULL;
		m_pool2 = NULL;
		m_callback = NULL;
		m_stopCallback = NULL;
		stopped = false;
		doCacheSqlite = true;
		doCacheDisk = false;
		tiles = NULL;
		m_errorCount = 0;
		m_sumCount = 0;
		m_sleepBeforeRequestTimeout = 0;
		m_totalCount = 0;
		isSnapshot = false;
		m_count = 0;
		_requestLock.Unlock();
	}

	TileLoader::~TileLoader(void)
	{
		Debug::WriteLine("Tile loader destructor");
		
		CleanTasks();

		if (m_pool != NULL) {
			m_pool->Shutdown();
			delete m_pool;
		}
		if (m_pool2 != NULL) {
			m_pool2->Shutdown();
			delete m_pool2;
		}
	}

	std::list<void*> GetActiveTasks() { return _activeTasks; }
	void LockActiveTasks(bool lock);
	void AddActiveTask(void* task);
	void RemoveActiveTask(void* task);
	bool HasActiveTask(void* task);

	void StopCaching()
	{
		m_diskCacher.stopped = true;
		m_sqliteCacher.stopped = true;
	}

	void ScheduleForCaching(TileCore* tile)
	{
		if (tile)
		{		
			if (doCacheSqlite) 
			{
				tile->AddRef();
				m_sqliteCacher.Enqueue(tile);
			}
			if (doCacheDisk)
			{
				tile->AddRef();
				m_diskCacher.Enqueue(tile);
			}
		}
	}
	void RunCaching()
	{
		if (doCacheSqlite) 
		{
			m_sqliteCacher.Run();
		}
		if (doCacheDisk)
		{
			m_diskCacher.Run();
		}
	}
	void Load(std::vector<CTilePoint*> &points, int zoom, BaseProvider* provider, void* tiles, bool isSnaphot, CString key, 
		int generation, bool cacheOnly = false);
	bool InitPools();
	void Stop();
	void TileLoaded(TileCore* tile);
	void CheckComplete();
};

// Represents a single loading task (a single tile to load)
class LoadingTask: ITask
{
public:
	int x;
	int y;
	int zoom;
	int generation;
	BaseProvider* Provider;
	bool cacheOnly;
	bool completed;
	bool busy;
	TileLoader* Loader;

	LoadingTask(int x, int y, int zoom, BaseProvider* provider, int generation, bool cacheOnly)
		: x(x), y(y), zoom(zoom), cacheOnly(cacheOnly)
	{
		Loader = NULL;
		this->busy = false;
		this->completed = false;
		this->Provider = provider;
		this->generation = generation;
	}

	bool Compare(LoadingTask* other) {
		return this->x == other->x && this->y == other->y && this->zoom == other->zoom;
	}

	void DoTask();
};



