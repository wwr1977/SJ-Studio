#pragma once
#include "ObjBase.h"

//struct RENLIST 
//{
//	list<SPTR<CRenderer>>	GeneralRenList;
//	list<SPTR<CRenderer>>	PerspectiveRenList;
//	list<SPTR<CRenderer>>	UIRenList;
//
//	bool					YSort;
//
//
//	void YSortting() 
//	{
//		if (false == YSort)
//			return;
//
//		stable_sort(GeneralRenList.begin(), GeneralRenList.end(), RendererSortting);
//		stable_sort(PerspectiveRenList.begin(), PerspectiveRenList.end(), RendererSortting);
//		stable_sort(UIRenList.begin(), UIRenList.end(), RendererSortting);
//
//	}
//
//	RENLIST():YSort(false)
//	{
//		GeneralRenList = list<SPTR<CRenderer>>();
//		PerspectiveRenList = list<SPTR<CRenderer>>();
//		UIRenList = list<SPTR<CRenderer>>();
//	};
//	~RENLIST() 
//	{
//		GeneralRenList.clear(); 
//		PerspectiveRenList.clear();
//		UIRenList.clear();
//	};
//};

class CGameScene;
class CRenderer;
class CSceneRenderMgr : public CObjBase
{
	friend CGameScene;

	struct RENLIST
	{
		list<SPTR<CRenderer>>	GeneralRenList;
		list<SPTR<CRenderer>>	PerspectiveRenList;
		list<SPTR<CRenderer>>	UIRenList;

		bool					YSort;


		void YSortting();

		RENLIST() :YSort(false)
		{
			GeneralRenList = list<SPTR<CRenderer>>();
			PerspectiveRenList = list<SPTR<CRenderer>>();
			UIRenList = list<SPTR<CRenderer>>();
		};
		~RENLIST()
		{
			GeneralRenList.clear();
			PerspectiveRenList.clear();
			UIRenList.clear();
		};
	};





private:

	bool													m_bCheck;
	CGameScene*												m_PairScene;
	map<int, RENLIST>										m_mapRendererGroup;
	
	map<int, RENLIST>::iterator								m_RenderGroupStart;
	map<int, RENLIST >::iterator							m_RenderGroupEnd;

	list<SPTR<CRenderer>>::iterator							m_ComRendererStart;
	list<SPTR<CRenderer>>::iterator							m_ComRendererEnd;

public:
	bool IsCheck() { return m_bCheck; }

private:
	void RenderGroupLoopInit();
	void ComGenRenderLoopInit();
	void ComPersRenderLoopInit();
	void ComUIRenderLoopInit();
	void Render();
	void PushRenderer(int _Index, CRenderer* _pRenderer);
	size_t  ActComCount();
	void ReleaseCom();
	const int GetRenderOnComponent();


	Mat GetSceneProj();
	Mat GetSceneView();
	Mat GetSceneProjNoneRatio();
	Mat GetNoneView();
	Mat GetScenePersProj();
	Mat GetScenePersView();

public:
	CSceneRenderMgr(CGameScene* _PairScene);
	~CSceneRenderMgr();
};

