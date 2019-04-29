#include "stdafx.h"
#include "SplashWindow.h"


CSplashWindow::CSplashWindow(const tstring& _Name, const tstring& _Title)
	:CGameWindow(), m_SplashImagePath(_T(""))
{
	CObjBase::Name(_Name);
	
	if (_T("") == _Title)
		m_Title = Name();
	
	CreateSplashWindow();
	ShowGameWindow();

	RECT m_WndRect;

	GetClientRect(m_hWnd, &m_WndRect);

	m_hCursor = LoadCursor(0, IDC_ARROW);


}


CSplashWindow::~CSplashWindow()
{
	//SAFE_DELETE(m_SplashImage);
}
void CSplashWindow::SetSplashImage(const tstring& _FolderKey, const tstring& _FileName)
{
	m_SplashImagePath = CFilePathMgr::GetPath(_FolderKey) + _FileName;

	SplashImageRender();
}
void CSplashWindow::SplashImageRender()
{
	if (_T("") == m_SplashImagePath)
		return;

	CImage* SplashImage = new CImage();
	
	HRESULT hr = SplashImage->Load(m_SplashImagePath.c_str());

	if (FAILED(hr))
		return;

	m_ImageSize.x = SplashImage->GetWidth();
	m_ImageSize.y = SplashImage->GetHeight();

	//CImage Rendering
	//m_SplashImage->AlphaBlend(m_hDC, 0, 0, m_WndSize.x, m_WndSize.y, 0, 0, m_ImageSize.x, m_ImageSize.y,m_bt.SourceConstantAlpha,m_bt.BlendOp);
	//m_SplashImage->TransparentBlt(m_hDC, 0, 0, m_WndSize.x, m_WndSize.y, 0, 0, m_ImageSize.x, m_ImageSize.y,RGB(255,255,255));

	SplashImage->StretchBlt(m_hDC, 0, 0, m_WndSize.x, m_WndSize.y, 0, 0, m_ImageSize.x, m_ImageSize.y, SRCCOPY);
	SplashImage->ReleaseDC();
	SAFE_DELETE(SplashImage);
}
bool CSplashWindow::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
	return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
	return TRUE;
	default:
		SplashImageRender();
		break;
	}

	return DefWindowProc(m_hWnd, message, wParam, lParam);;
}