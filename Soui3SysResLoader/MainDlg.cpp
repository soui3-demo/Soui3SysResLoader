// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
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

void CMainDlg::OnReload()
{
	SAutoRefPtr<IResProvider>   pResProvider;
	BOOL bLoaded = FALSE;
	CreateResProvider(RES_FILE, (IObjRef * *)& pResProvider);
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
	SHostWnd::OnSize(0,rcWnd.Size());
	return 0;
}

