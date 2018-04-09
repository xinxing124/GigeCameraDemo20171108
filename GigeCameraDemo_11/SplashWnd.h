#pragma once
#include "afxwin.h"

#define PI	3.14159265
class CSplashWnd:public CWnd
{
DECLARE_DYNAMIC(CSplashWnd)
private: 
    CDC mMemDC; 
    BITMAP bmBitmap; 
    CBitmap m_Bitmap; 
    CBitmap *Old_Bitmap; 

	HANDLE g_hThread;
public: 
	CImage image;
	long m_lHeight;
	long m_lWidth;
    void CreateSplash(); 
	void DeleteSplash(); 
    void ImageFrom_IDResource(UINT nID, LPCTSTR sTR, CImage *  pImg);
	void SetInfo(CString info);
	void HideCursor();
public:
CSplashWnd();
virtual ~CSplashWnd();

protected:
afx_msg void OnPaint();
DECLARE_MESSAGE_MAP()
public:

	CString m_strInfo;
	afx_msg void OnNcDestroy();
	double m_dbOldSpeed;
};

