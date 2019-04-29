#pragma once
#include "Resource.h"
#include "DXVertex.h"
#include "DXStruct.h"
#include "Mesh.h"
#include "DXEnum.h"
#include <unordered_map>
#include <fbxsdk.h>
#include <vector>



#ifdef _DEBUG
#ifdef X64
#pragma comment(lib,"..\\ThirdParty\\FBXSDK\\lib\\vs2015\\x64\\debug\\libfbxsdk-md.lib")
#else 
#pragma comment(lib,"..\\ThirdParty\\FBXSDK\\lib\\vs2015\\x86\\debug\\libfbxsdk-md.lib")
#endif
#else
#ifdef X64
#pragma comment(lib,"..\\ThirdParty\\FBXSDK\\lib\\vs2015\\x64\\release\\libfbxsdk-md.lib")
#else 
#pragma comment(lib,"..\\ThirdParty\\FBXSDK\\lib\\vs2015\\x86\\release\\libfbxsdk-md.lib")
#endif
#endif

#define ONEFRAME120 0.00833333333f
#define ONEFRAME60  0.01666666666f
#define ONEFRAME45  0.02222222222f
#define ONEFRAME30  0.03333333333f

typedef class CBoneKeyFrame 
{
public:
	Vec4						Scale;
	Vec4						Trans;
	Vec4						Quat;
	double						dTime;

}KeyFrame;


// Bone Parette(�� ���)�� Weights(����ġ) , Indices(��� �ε���)
class CParetteData 
{
public:
	int						BoneIndex;
	int						Indices;
	double					dWeights;
};


typedef class CBoneAnimationInfo  : public CReferenceBase
{
public:
	UINT								AnimIdx;
	std::wstring						Name;
	FbxTime								StartTime;
	FbxTime								EndTime;
	FbxLongLong							StartFrame;
	FbxLongLong							EndFrame;
	FbxLongLong							TimeLength;
	FbxTime::EMode						TimeMode;
	// ��� ���̴����� ���� Ű������ ������ ���� �ؽ���
	SPTR<class CTexture>				KeyFrameTex;

}CBoneAniInfo;

class CBoneMaterialColor
{
public:
	Vec4								Diffuse;	//���ݻ籤
	Vec4								Specular;	//���ݻ籤
	Vec4								Ambient;	//�ֺ���
	Vec4								Emissive;	//��ü�߱�

public:
	CBoneMaterialColor() : Diffuse(Vec4()), Specular(Vec4()), Ambient(Vec4()), Emissive(Vec4())
	{}
	~CBoneMaterialColor()
	{}
};

class CBoneMaterial : public CRefBase
{
public:
	std::string							Name;
	CBoneMaterialColor					MaterialColor;
	std::string							DiffTexName;
	std::string							BumpTexName;
	std::string							SpecTexName;
	std::string							EmivTexName;

public:
	void SetDiffName(const std::string _DiffPath)
	{
		DiffTexName = _DiffPath;
	}
	void SetBumpName(const std::string _BumpPath)
	{
		BumpTexName = _BumpPath;
	}
	void SetSpecName(const std::string _SpecPath)
	{
		SpecTexName = _SpecPath;
	}
	void SetEmivName(const std::string _EmivPath)
	{
		EmivTexName = _EmivPath;
	}
};

class CBoneMesh : public CReferenceBase
{
private:
	std::vector<std::list<SPTR<CMaterial>>>	m_vecMaterial;

public:
	FbxMesh*								pMesh;
	std::string								Name;
	std::vector<FBXVTX>						VtxDataVec;

	//�ε��� ������ ������ŭ ���׸����� �ִ�(������� ������ŭ)
	std::vector<std::vector<DWORD>>			IdxDataVec;
	std::vector<SPTR<CBoneMaterial>>		MaterialVec;

public:
	std::vector<std::vector<CParetteData>>  ParetteDataVec;


public:
	// ���� ���� ���� ������ ������ ���
	const UINT GetVtxCount();

	// �׷����� �ε��� ������ ������ ���
	const UINT GetIBCount();
	// �ε��� ���۾��� �ε��� ������ ������ ���(DWORD�� ����)
	const UINT GetIdxCount(const UINT& _Idx);
	// �ε��� ���۾��� �ε��� ������ ������ ���(IDX32�� ���� = GetIdxCount() / 3)
	const UINT GetTriIdxCount(const UINT& _Idx);
	const UINT GetAllTriCount();
	// �� ���׸����� ����
	const UINT GetMaterialCount();
	SPTR<CBoneMaterial> GetMaterialValue(const UINT _IdxNum)
	{
		return MaterialVec[_IdxNum];
	}

public:
	CFBXMesh* CreateFBXMesh();
};

class CBone : public CRefBase
{
public:
	CBone*													Parent;
	std::string												Name;

	FbxNode*												pNode;

	UINT													Index;
	UINT													Depth;

	//���� ��� ����
	FbxAMatrix												FbxOffsetMat;
	Mat														OffsetMat;

	FbxAMatrix												FbxBoneMat;
	Mat														BoneMat;

	std::vector<std::vector<KeyFrame>>						KeyFrameMap;
	CBoneMesh*												MeshData;

};

class CFbx : public CResource
{
private:
	static FbxAMatrix									ReverseXY;
	static FbxAMatrix									ReverseXZ;
	static FbxAMatrix									ReverseYZ;

public:
	static void Init();

private:
	friend class CRenderer;
	friend class CSkeletalFbxRenderer;
	friend class CDissolveSkeletalRenderer;
	friend class CFbxRenderer;
	friend class CSkeletal;

private:
	FbxManager*													m_FbxMgr;
	FbxScene*													m_FbxScene;
	FbxIOSettings*												m_IOSet;
	FbxImporter*												m_Imp;

private:
	UINT														m_AllBoneCount;
	std::vector<SPTR<CBone>>									m_AllBoneVec;
	std::unordered_multimap<std::string, SPTR<CBone>>			m_AllBoneFindMap;
	SPTR<class CTexture>										m_BoneMatTex;
	SPTR<class CTexture>										m_ComputeBoneMatTex;
	// ��� ���̴����� ���� �� & �������� ��� �ؽ���
	SPTR<CTexture>												m_BoneOffsetTex;


	// FBX ���� ������ �޽� ����ü
	std::vector<SPTR<CBoneMesh>>								m_AllMeshVec;
	// BoneMesh������ �����͸� ������� ���� �׸���� �޽�
	// �������Ҷ� �� �޽��� �������� �����
	std::vector<SPTR<CFBXMesh>>									m_AllFBXMeshVec;

	std::vector<SPTR<CBoneAniInfo>>								m_AllAniInfoVec;
	
private:
	FBX_ANIMATION_TYPE											m_eAnimationType;
	FbxAxisSystem::EPreDefinedAxisSystem						m_eLocalSpace;

public:
	const bool IsAni() 
	{
		return m_AllAniInfoVec.size() != 0;
	}
	const size_t GetFbxAnimCount() 
	{
		return m_AllAniInfoVec.size();
	}
	const std::vector<SPTR<CFBXMesh>>& GetAllFBXMeshVec()
	{
		return m_AllFBXMeshVec;
	}
	SPTR<CBoneMesh> GetBoneMesh(int _Index)
	{
		return m_AllMeshVec[_Index];
	}
	SPTR<CFBXMesh> GetSingleFbxMesh(int _Index)
	{
		return m_AllFBXMeshVec[_Index];
	}

public:
	CFbx();
	~CFbx();

public:
	bool BoneDataSetting(class FbxNode* _Node, CBone* _Bone, UINT _Depth);
	SPTR<CBone> FindBone(const char* _Name);
	const UINT FindBoneIdx(const char* _Name);

public:
	bool GetUVValue(FbxMesh* _Mesh, CBoneMesh* _MeshData, DWORD _CurIdx, UINT _CurVtx);
	bool GetNormalValue(FbxMesh* _Mesh, CBoneMesh* _MeshData, DWORD _CurIdx, UINT _CurVtx);
	bool GetTangentValue(FbxMesh* _Mesh, CBoneMesh* _MeshData, DWORD _CurIdx, UINT _CurVtx);
	bool GetBiNormalValue(FbxMesh* _Mesh, CBoneMesh* _MeshData, DWORD _CurIdx, UINT _CurVtx);

	Vec4 GetMatColor(FbxSurfaceMaterial* _Surf, const char* _ColorName, const char* _FactorName);
	std::string GetMatTexName(FbxSurfaceMaterial* _Surf, const char* _TexName);


public:
	CBoneAniInfo* CheckExistAnimation(FbxScene* _pScene,const std::wstring& _AnimKey = L"");					// �ִϸ��̼��� �ִ��� �������� üũ�Ѵ�.
	void BoneAnimationSetting(CBoneAniInfo* _AniInfo);															// ������ ����� �־��ִ°�.

public:
	FbxAMatrix GetBoneFbxMat(FbxNode* _Node);																	// ��忡�� �� ����� �����´�.
	void GetOffsetMat(FbxCluster* _Cluster, CBone* _pBone, CBoneMesh* _Mesh);									// Ŭ�������� ������ ����� �����´�
	void GetKeyFrameMat(CBoneAniInfo* _Info,FbxNode* _Node, FbxCluster* _Cluster, CBone* _pBone);				// �� ������ ��� Ű������ ����� �����´�

	void FbxToParetteData(FbxCluster* _Cluster, CBone* _pBone, CBoneMesh* _MeshData);							// �� �޽����� �ִϸ��̼� Weight,Indices �� �����´�
	void ParetteDataToVtx(FbxMesh* _pMesh, CBoneMesh* _pBoneMesh);												// Weight�� ���� ������� ���ؽ� ���� �ʱⰪ(Weight,Indices)�� ä���

	const Mat ConvertFbxMatToMat(const FbxAMatrix& _Mat);														// FbxAMatix	 ->  Mat ��ȯ
	const Vec4 ConvertFbxVecToVec(const FbxVector4& _Vector);													// FbxVector4	 ->  Vec4 ��ȯ
	const Vec4 ConvertFbxQuatToVec(const FbxQuaternion& _Quat);													// FbxQuaternion ->  Vec4 ��ȯ

public:
	const int GetAnimFPS(const unsigned int& _AnimTake);
	const float GetOneFrameTime(const unsigned int& _AnimTake);

public:
	void LoadFbxData();
	const bool LoadFbxAnimation(const std::wstring& _FbxPath,const wchar_t* _AnimKey);
	void FindFbxMesh(std::vector<FbxMesh*>& _vecMesh, FbxNode* _Node);
	void LoadFbxKeyFrame(CBoneAniInfo* _AniInfo,std::vector<FbxMesh*>& _vecMesh,FbxNode* _Node);
	// ��ǻƮ ���̴����� ����� ���ҽ��� �����.(�ִϸ��̼� ����)
	void CreateFBXSubTexture();


public:
	SPTR<CFBXMesh> GetFBXMesh(const int& _MeshIdx, const wchar_t* _MatKey);
	const bool BinaryLoad(FBX_ANIMATION_TYPE _AniType, const wchar_t* _BinaryPath);
	const void BinarySave(const wchar_t* _Path);
	const std::string GetSaveTexName(const std::string& _TexPath,const std::string& _SaveFileName);

public:
	void CheckTriangleTB();

public:
	NONE_CREATE_RESOURCE(CFbx)
	LOAD_DESC_BODY(
		FBX_INIT_FLAG		eFbxInitFlag;
		std::wstring		AnimKey;
	)
	const bool Load(const LOAD_DESC& _Desc);
	const bool FbxLoad(const LOAD_DESC& _Desc);
};

