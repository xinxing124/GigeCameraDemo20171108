﻿#pragma once


// CInputDialog 对话框

class CInputDialog : public CDialog
{
	DECLARE_DYNAMIC(CInputDialog)

public:
	CInputDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInputDialog();

// 对话框数据
	enum { IDD = IDD_INPUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_DelayTime;
	afx_msg void OnBnClickedOk();
};
