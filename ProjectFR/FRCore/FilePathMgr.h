#pragma once
#include"UserHeader.h"

class CDX9Core;
class CFilePathMgr
{
private:
	CFilePathMgr() {}
	~CFilePathMgr() { }

private:
	static TCHAR								m_DirPath[MAXPATH];
	static tstring								m_RootPath;
	static map<tstring, tstring>				m_mapFilePath;
	static map<tstring, tstring>::iterator		m_FindFileNameIter;

public:
	static vector<tstring>						ImageFolderKey;
	static vector<tstring>						ImageFile;
	static vector<tstring>						SoundFile;

public:
	friend CDX9Core;
	static bool Init();
	static bool Terminal();

public:
	static tstring		GetRootPath() { return m_RootPath; }
	static tstring		FilePath(const tstring& _Key, const tstring& _FileName);
	static tstring		PathToFileName(const tstring& _Path);

public:
	static bool AddRootFolder(tstring _Name);
	static bool AddRootFolder(tstring _Name, tstring _FilePath);
	static bool AddPlusFolder(tstring _PlusName, tstring _FolderKey);
	static bool AddPlusFolder(tstring _PlusName, tstring _FolderKey, tstring _FolderName);
	static tstring	 GetPath(tstring _Name);
	static tstring   GetDirPath(tstring _Name);
	static tstring   GetPath(const TCHAR* _Str);

};

