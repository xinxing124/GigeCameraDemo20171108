#pragma once
//#include "afxwin.h"
//#include "afxext.h"

// CMessageDialog 对话框

class CMessageDialog : public CDialog
{
	DECLARE_DYNAMIC(CMessageDialog)

public:
	CMessageDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMessageDialog();

// 对话框数据
	enum { IDD = IDD_MESSAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg LRESULT Response(WPARAM wparam,LPARAM lparam);

	afx_msg void OnBnClickedOk();
	CString m_strInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
