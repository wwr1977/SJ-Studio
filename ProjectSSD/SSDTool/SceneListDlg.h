#pragma once
#include <map>
#include <list>
#include <Actor.h>

// SceneListDlg 대화 상자

class SceneListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SceneListDlg)

public:
	SceneListDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SceneListDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCENELIST };
#endif

private:
	CTreeCtrl m_SceneActorTree;
	std::map<int, std::list<SPTR<CActor>>> m_AllActorMap;
	std::map<int, std::list<SPTR<CActor>>>::iterator m_StartIter;
	std::map<int, std::list<SPTR<CActor>>>::iterator m_EndIter;

	std::list<SPTR<CActor>> m_ActorList;
	std::list<SPTR<CActor>>::iterator m_StartList;
	std::list<SPTR<CActor>>::iterator m_EndList;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedReset();

	class CBaseComponent* ActorCheck(DWORD_PTR _Ptr);
	class CFilterEffect* FilterCheck(DWORD_PTR _Ptr);
	

	std::list<SPTR<class CBase3DRenderer>> BaseList;
	std::list<SPTR<class CSkeletalFbxRenderer>> SkelList;
	std::list<SPTR<class CFbxRenderer>> FbxList;
	std::list<SPTR<class COBBCollider>> ObbList;
	std::list<SPTR<class CSphereCollider>> SphereList;
	const std::list<SPTR<CBase3DRenderer>>& Base3DCheck(CActor * _Actor);
	const std::list<SPTR<CSkeletalFbxRenderer>>& SkeletalFbxCheck(CActor* _Actor);
	const std::list<SPTR<CFbxRenderer>>& FbxCheck(CActor* _Actor);
	const std::list<SPTR<COBBCollider>>& ObbCollCheck(CActor* _Actor);
	const std::list<SPTR<CSphereCollider>>& SphereCollCheck(CActor* _Actor);

	std::list<SPTR<class CLight>> LightList;
	const std::list<SPTR<CLight>>& LightCheck(CActor* _Actor);

	std::list<SPTR<class CGaussianBlur>> GaussianBlurList;
	const std::list<SPTR<CGaussianBlur>>& GaussianBlurCheck(CActor* _Actor);
	std::list<SPTR<class CBloomFilter>> BloomList;
	const std::list<SPTR<CBloomFilter>>& BloomFilterCheck(CActor* _Actor);

	std::list<SPTR<class CNavigationMesh>> NaviMeshList;
	const std::list<SPTR<CNavigationMesh>>& NavigationMeshCheck(CActor* _Actor);

	std::list<SPTR<class CNaviMeshArea>> NaviAreaList;
	const std::list<SPTR<CNaviMeshArea>>& NavigationAreaCheck(CActor* _Actor);


	virtual BOOL OnInitDialog();
	int TActorCheck;
	int TRendererCheck;
	int TColliderCheck;
	int TLightCheck;
	int TFilterCheck;
	int TNaviCheck;

};
