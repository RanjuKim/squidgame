#include "stdafx.h"
#include "SquidGame.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSquidGameApp, CWinApp)
    ON_COMMAND(ID_APP_EXIT, &CWinApp::OnAppExit)
END_MESSAGE_MAP()

CSquidGameApp theApp;

BOOL CSquidGameApp::InitInstance()
{
    CWinApp::InitInstance();

    CMainFrame* pFrame = new CMainFrame;
    m_pMainWnd = pFrame;

    if (!pFrame->LoadFrame(IDR_MAINFRAME))
        return FALSE;

    pFrame->ShowWindow(m_nCmdShow);
    pFrame->UpdateWindow();

    return TRUE;
}
