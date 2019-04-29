#include "stdafx.h"
#include "SceneRenderMgr.h"
#include "GameScene.h"
#include "Renderer.h"
#include "SceneBuilder.h"

void CSceneRenderMgr::RENLIST::YSortting() 
{
	if (false == YSort)
		return;
	

	stable_sort(GeneralRenList.begin(), GeneralRenList.end(), RendereYSort);
	stable_sort(PerspectiveRenList.begin(), PerspectiveRenList.end(), RendereYSort);
	stable_sort(UIRenList.begin(), UIRenList.end(), RendereYSort);
}
CSceneRenderMgr::CSceneRenderMgr(CGameScene* _PairScene)
	:m_PairScene(_PairScene)
{

}


CSceneRenderMgr::~CSceneRenderMgr()
{
	m_mapRendererGroup.clear();
}

void CSceneRenderMgr::RenderGroupLoopInit()
{
	m_RenderGroupStart = m_mapRendererGroup.begin();
	m_RenderGroupEnd = m_mapRendererGroup.end();
}
void CSceneRenderMgr::ComGenRenderLoopInit()
{
	m_ComRendererStart = m_RenderGroupStart->second.GeneralRenList.begin();
	m_ComRendererEnd = m_RenderGroupStart->second.GeneralRenList.end();
}
void CSceneRenderMgr::ComPersRenderLoopInit()
{
	m_ComRendererStart = m_RenderGroupStart->second.PerspectiveRenList.begin();
	m_ComRendererEnd = m_RenderGroupStart->second.PerspectiveRenList.end();
}
void CSceneRenderMgr::ComUIRenderLoopInit()
{
	m_ComRendererStart = m_RenderGroupStart->second.UIRenList.begin();
	m_ComRendererEnd = m_RenderGroupStart->second.UIRenList.end();
}
void CSceneRenderMgr::Render()
{
	m_bCheck = false;
	m_PairScene->GetWindow()->DeviceRenderStart();

	RenderGroupLoopInit();

	for (; m_RenderGroupStart != m_RenderGroupEnd; ++m_RenderGroupStart)
	{
		m_RenderGroupStart->second.YSortting();

		ComGenRenderLoopInit();
		for (; m_ComRendererStart != m_ComRendererEnd; ++m_ComRendererStart)
		{
			if (true == (*m_ComRendererStart)->IsUpdate())
			{
				(*m_ComRendererStart)->ComRender();
			}
		}



		Mat ChangeProj;
		Mat ChangeView;

		// 만일 랜더 그룹에 PerspectiveMode Rederer가 1개라도 존재 한다면
		// PersCam의 Proj,View로 랜더링을 적용
		if (0 != m_RenderGroupStart->second.PerspectiveRenList.size())
		{
			ChangeProj = GetScenePersProj();
			ChangeView = GetScenePersView();

			//Perspective 전용 Proj과 View로 전환
			m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_PROJECTION, &ChangeProj);
			m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_VIEW, &ChangeView);

			ComPersRenderLoopInit();

			for (; m_ComRendererStart != m_ComRendererEnd; ++m_ComRendererStart)
			{
				if (true == (*m_ComRendererStart)->IsUpdate())
				{
					(*m_ComRendererStart)->ComRender();
				}
			}
		}
		// 만일 랜더 그룹에 UI 랜더러가 존재한다면 
		// 줌인이 없는 Proj 행렬과 항등행렬인 view행렬로 랜더를 적용한다

		if (0 != m_RenderGroupStart->second.UIRenList.size())
		{
			ChangeProj = GetSceneProjNoneRatio();
			ChangeView = GetNoneView();

			//UI 전용 Proj과 View로 전환
			m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_PROJECTION, &ChangeProj);
			m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_VIEW, &ChangeView);

			ComUIRenderLoopInit();

			for (; m_ComRendererStart != m_ComRendererEnd; ++m_ComRendererStart)
			{
				if (true == (*m_ComRendererStart)->IsUpdate())
				{
					(*m_ComRendererStart)->ComRender();
				}
			}
		}

		//일반 Proj 과 View로 복귀
			
		ChangeProj = GetSceneProj();
		ChangeView = GetSceneView();
		m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_PROJECTION, &ChangeProj);
		m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_VIEW, &ChangeView);
		

	}
	
	/*m_PairScene->FontRender();

	Mat BaseProj;
	Mat BaseView;

	BaseProj = GetSceneProj();
	BaseView = GetSceneView();


	m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_PROJECTION, &BaseProj);
	m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_VIEW, &BaseView);
*/
	m_PairScene->FontRender();




	Mat BaseProj;
	Mat BaseView;

	BaseProj = GetSceneProjNoneRatio();
	BaseView = GetNoneView();

	
	m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_PROJECTION, &BaseProj);
	m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_VIEW, &BaseView);

	m_PairScene->UIFontRender();

	BaseProj = GetSceneProj();
	BaseView = GetSceneView();

	m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_PROJECTION, &BaseProj);
	m_PairScene->GetWindow()->GetLPDevice()->SetTransform(D3DTS_VIEW, &BaseView);

	if (TRUE == m_PairScene->GetDebugMgr()->IsDebugMode())
	{
		m_PairScene->DebugRender();
	}

	m_PairScene->GetBuilder()->SceneRender();


	m_PairScene->GetWindow()->DeviceRenderEnd();
	

}
void CSceneRenderMgr::PushRenderer(int _Index, CRenderer* _pRenderer)
{
	map<int,RENLIST>::iterator FindIter = m_mapRendererGroup.find(_Index);

	if (FindIter == m_mapRendererGroup.end()) 
	{
		RENLIST NewRenList = RENLIST();
		
		if (LAYER_MAPOBJECT == _Index)
			NewRenList.YSort = true;

		if (PERSPECTIVE == _pRenderer->GetProjMode()) 
		{
			NewRenList.PerspectiveRenList.push_back(_pRenderer);
		}
		else if (true == _pRenderer->isUI()) 
		{
			NewRenList.UIRenList.push_back(_pRenderer);
		}
		else 
		{
			NewRenList.GeneralRenList.push_back(_pRenderer);
		}
		m_mapRendererGroup.insert(map<int, RENLIST>::value_type(_Index, NewRenList));
		return;
	}

	if (PERSPECTIVE == _pRenderer->GetProjMode())
	{
		FindIter->second.PerspectiveRenList.push_back(_pRenderer);
	}
	else if (true == _pRenderer->isUI()) 
	{
		FindIter->second.UIRenList.push_back(_pRenderer);
	}
	else 
	{
		FindIter->second.GeneralRenList.push_back(_pRenderer);
	}

	return;

}
size_t  CSceneRenderMgr::ActComCount()
{
	size_t Count = 0;

	RenderGroupLoopInit();

	for (;m_RenderGroupStart != m_RenderGroupEnd; ++m_RenderGroupStart)
	{
		Count += m_RenderGroupStart->second.GeneralRenList.size();
		Count += m_RenderGroupStart->second.UIRenList.size();
	}

	return Count;
}
void CSceneRenderMgr::ReleaseCom()
{
	RenderGroupLoopInit();

	for (; m_RenderGroupStart != m_RenderGroupEnd; ++m_RenderGroupStart)
	{
		ComGenRenderLoopInit();
		
		// 일반적인 Renderer를 List에서 Erase

		for (; m_ComRendererStart != m_ComRendererEnd; )
		{
			if (true == (*m_ComRendererStart)->IsDeath())
			{
				m_ComRendererStart = m_RenderGroupStart->second.GeneralRenList.erase(m_ComRendererStart);
				m_bCheck = true;
			}
			else
			{
				++m_ComRendererStart;
			}
		}

		ComUIRenderLoopInit();
		// UI Renderer를 List에서 Erase
		for (; m_ComRendererStart != m_ComRendererEnd; )
		{
			if (true == (*m_ComRendererStart)->IsDeath())
			{
				m_ComRendererStart = m_RenderGroupStart->second.UIRenList.erase(m_ComRendererStart);
				m_bCheck = true;
			}
			else
			{
				++m_ComRendererStart;
			}
		}

		ComPersRenderLoopInit();
		// PerspectiveMode Renderer를 List에서 Erase
		for (; m_ComRendererStart != m_ComRendererEnd; )
		{
			if (true == (*m_ComRendererStart)->IsDeath())
			{
				m_ComRendererStart = m_RenderGroupStart->second.UIRenList.erase(m_ComRendererStart);
				m_bCheck = true;
			}
			else
			{
				++m_ComRendererStart;
			}
		}

	}
}

Mat CSceneRenderMgr::GetSceneProj()
{
	return m_PairScene->GetMainCamCom()->GetSceneProj();
}
Mat CSceneRenderMgr::GetSceneView()
{
	return m_PairScene->GetMainCamCom()->GetSceneView();
}
Mat CSceneRenderMgr::GetSceneProjNoneRatio()
{
	return m_PairScene->GetMainCamCom()->GetProjNoneRatio();
}
Mat CSceneRenderMgr::GetScenePersProj() 
{
	return m_PairScene->GetMainPersCamCom()->GetSceneProj();
}
Mat CSceneRenderMgr::GetScenePersView()
{
	return m_PairScene->GetMainPersCamCom()->GetSceneView();
}
Mat CSceneRenderMgr::GetNoneView()
{
	Mat NoneView;
	D3DXMatrixIdentity(&NoneView);
	return NoneView;
}
const int CSceneRenderMgr::GetRenderOnComponent()
{
	int ComponentCount = 0;

	RenderGroupLoopInit();

	for (;m_RenderGroupStart!= m_RenderGroupEnd;++m_RenderGroupStart)
	{
		ComGenRenderLoopInit();
		
		for (;m_ComRendererStart!= m_ComRendererEnd; ++m_ComRendererStart)
		{
			if (true == (*m_ComRendererStart)->IsUpdate()) 
			{
				++ComponentCount;
			}
		}

		ComPersRenderLoopInit();

		for (; m_ComRendererStart != m_ComRendererEnd; ++m_ComRendererStart)
		{
			if (true == (*m_ComRendererStart)->IsUpdate())
			{
				++ComponentCount;
			}
		}

		ComUIRenderLoopInit();

		for (; m_ComRendererStart != m_ComRendererEnd; ++m_ComRendererStart)
		{
			if (true == (*m_ComRendererStart)->IsUpdate())
			{
				++ComponentCount;
			}
		}
	}

	return ComponentCount;
}