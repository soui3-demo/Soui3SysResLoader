// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND")),m_bFileDemo(false)
{
}

CMainDlg::~CMainDlg()
{
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	return 0;
}
//TODO:消息映射
void CMainDlg::OnClose()
{
	SNativeWnd::DestroyWindow();
	PostQuitMessage(0);
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

#include <helper/SAutoBuf.h>

void CMainDlg::OnReloadSys()
{
	SAutoRefPtr<IResProvider>   pResProvider;
	BOOL bLoaded = FALSE;
	CreateResProvider(RES_FILE, (IObjRef * *)& pResProvider);
	//load sys resource package from current directory.
	bLoaded = pResProvider->Init((LPARAM)_T("soui-sys-resource"), 0);
	SASSERT(bLoaded);
	pugi::xml_document xmlDoc;
	const LPCTSTR pszType = _T("xml");
	const LPCTSTR pszXmlName = _T("SYS_XML_SKIN");
	size_t dwSize=pResProvider->GetRawBufferSize(pszType,pszXmlName);
	if(dwSize==0) return;
	SAutoBuf strXml;
	strXml.Allocate(dwSize);
	pResProvider->GetRawBuffer(pszType,pszXmlName,strXml,dwSize);
	pugi::xml_parse_result result= xmlDoc.load_buffer(strXml,strXml.size(),pugi::parse_default,pugi::encoding_auto);
	if(result)
	{
		SApplication *theApp = SApplication::getSingletonPtr();
		SSkinPool * pBuildSkinPool= SSkinPoolMgr::getSingletonPtr()->GetBuiltinSkinPool();
		pBuildSkinPool->RemoveAll();

		theApp->AddResProvider(pResProvider,NULL);
		pBuildSkinPool->LoadSkins(xmlDoc.child(L"skin"));
		theApp->RemoveResProvider(pResProvider);

		PostMessage(UM_RECOREATE);
	}
}

LRESULT CMainDlg::OnRecreate(UINT uMsg,WPARAM wp,LPARAM lp)
{
	CRect rcWnd;
	SNativeWnd::GetWindowRect(&rcWnd);
	SHostWnd::OnDestroy();
	CREATESTRUCT cs;
	cs.cx = rcWnd.Width();
	cs.cy = rcWnd.Height();
	SHostWnd::OnCreate(&cs);

	if(m_bFileDemo){
		OnReloadDemo();
	}

	SHostWnd::OnSize(0,rcWnd.Size());
	return 0;
}

void CMainDlg::OnReloadDemo()
{
	if(!SApplication::getSingletonPtr()->HasResource(_T("file"),_T("demo.xml")))
	{
		SMessageBox(m_hWnd,_T("在本程序目录下放一个demo.xml来加载自定义布局"),__T("提示"),MB_OK|MB_ICONSTOP);
		return;
	}
	SWindow *pDemo = FindChildByID(R.id.wnd_demo);
	if(pDemo){
		pDemo->DestroyAllChildren();
		pDemo->CreateChildren(L"<include src=\"file:demo.xml\"/>");
		m_bFileDemo = true;
	}
}

