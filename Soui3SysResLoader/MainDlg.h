// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#define UM_RECOREATE (WM_USER+100)

class CMainDlg : public SHostWnd
{
	bool m_bFileDemo;
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

protected:
	void OnReloadSys();
	void OnReloadDemo();

	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_ID_COMMAND(R.id.btn_reload_sys,OnReloadSys)
		EVENT_ID_COMMAND(R.id.btn_reload_demo,OnReloadDemo)
	EVENT_MAP_END()
		
	LRESULT OnRecreate(UINT uMsg,WPARAM wp,LPARAM lp);

	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		MESSAGE_HANDLER_EX(UM_RECOREATE,OnRecreate)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
};
