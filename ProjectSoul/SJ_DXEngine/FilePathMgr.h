#pragma once
#include"DXMacro.h"
#include<string>
#include<Windows.h>
#include<unordered_map>


class CFilePathMgr
{


private:
	class CPathMgrDestroyer 
	{
		friend class CFilePathMgr;

	public:
		~CPathMgrDestroyer() 
		{
			//CFilePathMgr::Release();
		}
	};

private:
	friend class CSJ_DXCore;
	friend class CPathMgrDestroyer;

private:
	static CPathMgrDestroyer												m_PathMgrDest;
	static wchar_t															m_DirPath[MAXPATH];
	static std::wstring														m_RootPath;
	static std::unordered_map<std::wstring, std::wstring>					m_mapFilePath;
	static std::unordered_map<std::wstring, std::wstring>::iterator			m_FindFileNameIter;

private:
	static const BOOL Init();
	static const BOOL Release();

public:
	static std::wstring		GetRootPath() { return m_RootPath; }
	static std::wstring		FilePath(const std::wstring& _Key, const std::wstring& _FileName);
	static std::wstring		PathToFileName(const std::wstring& _Path);

public:
	static BOOL AddRootFolder(const std::wstring& _Name);
	static BOOL AddRootFolder(const std::wstring& _Name, const std::wstring& _FilePath);
	static BOOL AddPlusFolder(const std::wstring& _PlusName,const std::wstring& _FolderKey);
	static BOOL AddPlusFolder(const std::wstring& _PlusName,const std::wstring& _FolderKey,const std::wstring& _FolderName);

public:
	static const std::wstring GetPath(const std::wstring& _Name) ;
	static const std::string GetSTRPath(const std::wstring& _Name);
	
	static const std::wstring GetPath(const std::wstring& _FolderKey,const std::wstring& _FileName);
	static const std::string GetSTRPath(const std::wstring& _FolderKey, const std::wstring& _FileName);
	
	static const std::wstring GetDirPath(const std::wstring& _Name) ;
	static const std::wstring GetPath(const wchar_t* _Str);

private:
	CFilePathMgr() {}
	virtual ~CFilePathMgr() = 0 { }
};

