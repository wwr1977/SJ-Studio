#include "stdafx.h"
#include "FilePathMgr.h"

TCHAR										CFilePathMgr::m_DirPath[MAXPATH] = _T("") ;
tstring										CFilePathMgr::m_RootPath = _T("");
map<tstring, tstring>						CFilePathMgr::m_mapFilePath;
map<tstring, tstring>::iterator				CFilePathMgr::m_FindFileNameIter;
vector<tstring>								CFilePathMgr::ImageFolderKey;
vector<tstring>								CFilePathMgr::ImageFile;
vector<tstring>								CFilePathMgr::SoundFile;

bool CFilePathMgr::Init() 
{
	GetCurrentDirectory(sizeof(TCHAR)*MAXPATH, m_DirPath);
	m_RootPath = m_DirPath;
	size_t Count = m_RootPath.find_last_of(_T("\\"));
	m_RootPath.replace(Count, m_RootPath.size(), _T(""));
	m_RootPath = m_RootPath + _T("\\Bin\\");

	if (m_RootPath == _T("")) 
	{
		return FALSE;
	}

	ImageFolderKey.push_back(_T("Character"));
	ImageFolderKey.push_back(_T("Monster"));
	ImageFolderKey.push_back(_T("Effect"));
	ImageFolderKey.push_back(_T("UI"));

	ImageFile.push_back(_T("*.png"));
	ImageFile.push_back(_T("*.jpg"));

	SoundFile.push_back(_T("*.wav"));
	SoundFile.push_back(_T("*.mp3"));
	SoundFile.push_back(_T("*.mp4"));
	

	return TRUE;
}
bool CFilePathMgr::AddRootFolder(tstring _Path) 
{
	
	return AddRootFolder(_Path, _Path);
}
bool CFilePathMgr::AddRootFolder(tstring _Name, tstring _FilePath)
{
	m_FindFileNameIter = m_mapFilePath.find(_Name);
	
	if (m_FindFileNameIter != m_mapFilePath.end())
	{
		return FALSE;
	}

	tstring NewPath = m_RootPath + _FilePath + _T("\\");

	m_mapFilePath.insert(make_pair(_Name, NewPath));
	
	return TRUE;

}
tstring	 CFilePathMgr::GetPath(tstring _Name) 
{
	m_FindFileNameIter = m_mapFilePath.find(_Name);

	if (m_FindFileNameIter != m_mapFilePath.end())
	{
		return m_FindFileNameIter-> second;
	}
	return _T("");
}
tstring   CFilePathMgr::GetDirPath(tstring _Name)
{
	m_FindFileNameIter = m_mapFilePath.find(_Name);

	if (m_FindFileNameIter != m_mapFilePath.end())
	{
		tstring TempPath = m_FindFileNameIter->second;
		size_t Last_Index = TempPath.find_last_of(_T("\\"));
		size_t Count = TempPath.size() - Last_Index;
		TempPath.replace(Last_Index, Count, _T(""));
		return TempPath;
	}
	return _T("");
}
tstring CFilePathMgr::GetPath(const TCHAR* _Str)
{
	tstring a = _Str;
	m_FindFileNameIter = m_mapFilePath.find(_Str);

	if (m_FindFileNameIter != m_mapFilePath.end())
	{
		return m_FindFileNameIter->second;
	}
	return _T("");
}
bool  CFilePathMgr::AddPlusFolder(tstring _PlusName, tstring _FolderKey)
{
	return AddPlusFolder(_PlusName, _FolderKey, _FolderKey);
}
bool  CFilePathMgr::AddPlusFolder(
	tstring _PlusName
	, tstring _FolderKey
	, tstring _FolderName)
{
	m_FindFileNameIter = m_mapFilePath.find(_PlusName);

	if (m_FindFileNameIter == m_mapFilePath.end())
	{
		return FALSE;
	}

	tstring NewPath = m_FindFileNameIter->second + _FolderName + _T("\\");

	m_mapFilePath.insert(map<tstring, tstring>::value_type(_FolderKey, NewPath));

	return TRUE;

}
bool CFilePathMgr::Terminal() 
{
	m_mapFilePath.clear();
	return TRUE;
}
tstring	 CFilePathMgr::FilePath(const tstring& _Key, const tstring& _FileName)
{
	tstring Path = CFilePathMgr::GetPath(_Key);

	return Path + _FileName;

}
tstring	CFilePathMgr::PathToFileName(const tstring& _Path)
{
	tstring FileName = _Path;
	size_t Last_Index = FileName.find_last_of(_T("\\"));
	Last_Index += 1;

	FileName.replace(0, Last_Index, _T(""));

	size_t First_Index = FileName.find_last_of(_T("."));
	size_t Count = FileName.size() - First_Index;
	FileName.replace(First_Index, Count, _T(""));

	return FileName;

}