// InputDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GigeCameraDemo.h"
#include "InputDialog.h"
#include "afxdialogex.h"


// CInputDialog �Ի���

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


// CInputDialog ��Ϣ�������


void CInputDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CDialog::OnOK();
}
