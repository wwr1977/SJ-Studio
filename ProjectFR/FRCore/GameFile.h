#pragma once
#include "ObjBase.h"

class CGameFile : public CObjBase
{
public:
	static vector<tstring>	vecSpriteDataFileName;
	static vector<tstring>	vecAniDataFileName;

private:		
	tstring					m_FilePath;
	FILE*					m_pFile;
	bool					m_bClose;
	size_t					m_FileSize;

public:
	FILE* GetFile();
	const size_t GetFileSize();
	BOOL Open(const tstring& _FileMode);
	void Close();

	template<typename T>
	void operator<<(const T& _Data) 
	{
		assert(m_pFile);

		int Size = sizeof(_Data);
		fwrite(&Data, Size, 1, m_pFile);
	}

	template<typename T>
	void operator>>(const T& _Data) 
	{
		assert(m_pFile);

		fread_s((void*)&_Data, sizeof(_Data), sizeof(_Data), 1, m_pFile);
	}


public:
	static void Init();
	static int ExistGameFile(const tstring _Path);
	static int ExistGameFile(const tstring& _FolderKey, const tstring& _FileName);
	
public:
	void InitFileCursor();

public:
	CGameFile(const tstring& _Path);
	CGameFile(const tstring& _FolderKey, const tstring& _FileName);
	~CGameFile();

};

