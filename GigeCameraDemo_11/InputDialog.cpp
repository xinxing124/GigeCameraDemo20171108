// InputDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GigeCameraDemo.h"
#include "InputDialog.h"
#include "afxdialogex.h"


// CInputDialog 对话框

IMPLEMENT_DYNAMIC(CInputDialog, CDialog)

CInputDialog::CInputDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDialog::IDD, pParent)
{

	m_DelayTime = 1;
}

CInputDialog::~CInputDialog()
{
}

void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIME_EDIT, m_DelayTime);
}


BEGIN_MESSAGE_MAP(CInputDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CInputDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputDialog 消息处理程序


void CInputDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CDialog::OnOK();
}
