#pragma once

#include "SquidCompositor.h"

class CMainFrame : public CFrameWnd
{
public:
    CMainFrame();

protected:
    DECLARE_DYNAMIC(CMainFrame)

private:
    CxImage m_imgResult;
    bool    m_bHasResult;

    CString GetAppDirectory() const;
    void    LoadAndCompose();
    void    ResizeToImage();

protected:
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};
