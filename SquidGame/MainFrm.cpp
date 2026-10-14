#include "stdafx.h"
#include "SquidGame.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
    : m_bHasResult(false)
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    LoadAndCompose();
    ResizeToImage();

    return 0;
}

CString CMainFrame::GetAppDirectory() const
{
    TCHAR buf[MAX_PATH];
    GetModuleFileName(NULL, buf, MAX_PATH);
    CString path(buf);
    int pos = path.ReverseFind(_T('\\'));
    if (pos >= 0)
        path = path.Left(pos + 1);
    return path;
}

void CMainFrame::LoadAndCompose()
{
    CString dir = GetAppDirectory();
    CString headPath   = dir + _T("squid_head.png");
    CString bodyPath   = dir + _T("squid_body.png");
    CString pointsPath = dir + _T("squid_points.png");

    CxImage imgHead, imgBody, imgPoints;
    bool ok = imgHead.Load(headPath, CXIMAGE_FORMAT_PNG);
    ok = imgBody.Load(bodyPath, CXIMAGE_FORMAT_PNG) && ok;
    ok = imgPoints.Load(pointsPath, CXIMAGE_FORMAT_PNG) && ok;

    if (!ok)
    {
        m_bHasResult = false;
        return;
    }

    m_bHasResult = ComposeSquidImage(imgHead, imgBody, imgPoints, m_imgResult);
}

void CMainFrame::ResizeToImage()
{
    if (!m_bHasResult)
    {
        SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOZORDER | SWP_NOMOVE);
        return;
    }

    CRect rcWindow(0, 0, (int)m_imgResult.GetWidth(), (int)m_imgResult.GetHeight());
    ::AdjustWindowRect(&rcWindow, GetStyle(), GetMenu() != NULL);

    SetWindowPos(NULL, 0, 0, rcWindow.Width(), rcWindow.Height(), SWP_NOZORDER | SWP_NOMOVE);

    // AdjustWindowRect can be off by a few pixels on high-DPI displays,
    // so grow the window by whatever the client area is still missing.
    CRect rcClient;
    GetClientRect(&rcClient);
    int dx = (int)m_imgResult.GetWidth()  - rcClient.Width();
    int dy = (int)m_imgResult.GetHeight() - rcClient.Height();
    if (dx != 0 || dy != 0)
        SetWindowPos(NULL, 0, 0, rcWindow.Width() + dx, rcWindow.Height() + dy, SWP_NOZORDER | SWP_NOMOVE);
}

void CMainFrame::OnPaint()
{
    CPaintDC dc(this);

    CRect rcClient;
    GetClientRect(&rcClient);

    if (m_bHasResult)
    {
        m_imgResult.Draw(dc.GetSafeHdc(), 0, 0);
    }
    else
    {
        dc.FillSolidRect(&rcClient, RGB(255, 255, 255));
        CString msg;
        msg.Format(_T("squid_head.png, squid_body.png and squid_points.png were not found in:\n%s"),
            (LPCTSTR)GetAppDirectory());
        dc.SetBkMode(TRANSPARENT);
        dc.DrawText(msg, &rcClient, DT_CENTER | DT_WORDBREAK);
    }
}
