#include "stdafx.h"
#include "GameFile.h"

vector<tstring>	CGameFile::vecSpriteDataFileName;
vector<tstring>	CGameFile::vecAniDataFileName;

CGameFile::CGameFile(const tstring& _Path)
	:m_pFile(nullptr), m_FilePath(_Path)
	, m_bClose(false), m_FileSize(0)
{

}
CGameFile::CGameFile(const tstring& _FolderKey, const tstring& _FileName)
	:m_pFile(nullptr), m_FilePath(_T("")), m_FileSize(0), m_bClose(false)
{
	tstring Path  = CFilePathMgr::GetPath(_FolderKey) + _FileName;

	m_FilePath = Path;

	if (-1 != ExistGameFile(m_FilePath)) 
	{
		FILE* Temp = nullptr;
		_tfopen_s(&Temp, m_FilePath.c_str(), _T("rb"));
		
		if (nullptr == Temp)
			return;

		fseek(Temp, 0, SEEK_END);
		m_FileSize = ftell(Temp);
		fclose(Temp);
	}
}
CGameFile::~CGameFile()
{
	if(false == m_bClose)
		Close();
}
void CGameFile::Init()
{
	vecSpriteDataFileName.reserve(10);

	vecSpriteDataFileName.push_back(_T("CharacterSprite.bin"));
	vecSpriteDataFileName.push_back(_T("MonsterSprite.bin"));
	vecSpriteDataFileName.push_back(_T("EffectSprite.bin"));
	vecSpriteDataFileName.push_back(_T("EtcSprite.bin"));

	vecAniDataFileName.reserve(10);
	
	vecAniDataFileName.push_back(_T("CharacterAni.bin"));
	vecAniDataFileName.push_back(_T("MonsterAni.bin"));
	vecAniDataFileName.push_back(_T("EffectAni.bin"));
	vecAniDataFileName.push_back(_T("EtcAni.bin"));

}
int CGameFile::ExistGameFile(const tstring _Path)
{
	return _taccess(_Path.c_str(), 0);
	
}
int CGameFile::ExistGameFile(const tstring& _FolderKey, const tstring& _FileName)
{
	tstring FilePath = CFilePathMgr::GetPath(_FolderKey);

	if (_T("") == FilePath)
	{
		return -1;
	}

	FilePath += _FileName;

	return _taccess(FilePath.c_str(), 0);
}
FILE* CGameFile::GetFile()
{
	return m_pFile;
}

const size_t CGameFile::GetFileSize()
{
	return m_FileSize;
}
BOOL CGameFile::Open(const tstring& _FileMode)
{
	if (-1 == CGameFile::ExistGameFile(m_FilePath)) 
	{
		return FALSE;
	}

	_tfopen_s(&m_pFile, m_FilePath.c_str(), _FileMode.c_str());
	
	return TRUE;
}
void CGameFile::Close()
{
	if (nullptr != m_pFile) 
	{
		fclose(m_pFile);
		m_pFile = nullptr;
		m_bClose = true;
	}
}
void CGameFile::InitFileCursor()
{
	if(nullptr != m_pFile)
		fseek(m_pFile, 0, SEEK_SET);
}