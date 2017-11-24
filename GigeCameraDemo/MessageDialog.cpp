// MessageDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "GigeCameraDemo.h"
#include "MessageDialog.h"

//extern ULONGLONG m_iCountImage_1;
//extern ULONGLONG m_iCountImage_2;

#define DLG_BACKGROUND RGB(24,46,69)
// CMessageDialog 对话框

IMPLEMENT_DYNAMIC(CMessageDialog, CDialog)

CMessageDialog::CMessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDialog::IDD, pParent)
	, m_strInfo(_T(""))
{

}

CMessageDialog::~CMessageDialog()
{
}

void CMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageDialog, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_MSG,Response)//Response是自定义消息响应函数
	ON_BN_CLICKED(IDOK, &CMessageDialog::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CMessageDialog 消息处理程序

LRESULT CMessageDialog::Response(WPARAM wparam,LPARAM lparam)
{
CString m_recv = (LPCTSTR)wparam;
m_strInfo=m_recv.Mid(1,m_recv.Find('$')-1);
::SetDlgItemTextA(this->m_hWnd,IDC_STATIC_1,m_strInfo);
//Invalidate();

return 1;
}

HBRUSH CMessageDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	//return hbr;
	HBRUSH hbr =  CDialog::OnCtlColor(pDC, pWnd, nCtlColor);  
    if ( nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_LISTBOX)  
    {  
		//nCtlColor==CTLCOLOR_STATIC ||
        pDC->SetBkColor(RGB(255, 255, 255));     
		//pDC->SetBkMode(TRANSPARENT);  
		//pDC-> SetTextColor(RGB(255,0,0));                    //前景色为红色 
        return hbr;  
    }  
    else  
    {  
		CFont * cFont=new CFont;   
		cFont->CreateFont(16,0,0,0,FW_SEMIBOLD,FALSE,FALSE,0,  ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH&FF_SWISS,"Arial");  //对特定的控件做修改 
        HBRUSH hBrush =  CreateSolidBrush(DLG_BACKGROUND); //创建背景刷;

	
        pDC->SetBkMode(TRANSPARENT);  
		pWnd->SetFont(cFont);
		pDC-> SetTextColor(RGB(255,255,255));                    //前景色为红色 
        return hBrush;  
    }  
	
	//if(CTLCOLOR_STATIC==nCtlColor && pWnd-> GetDlgCtrlID()==IDC_STRY) //只对IDC_TOOL这一个静态文本控件起作用   
	//{  
	//	pDC-> SetBkColor(RGB(236,233,216));                   //背景色为窗口颜色   
	//	pDC-> SetTextColor(RGB(255,0,0));                    //前景色为红色     
	//	hbr =CreateSolidBrush(RGB(236,233,216));            //其它区域也为窗口颜色
	//}   
	//else
	//	hbr=CDialog::OnCtlColor(pDC,   pWnd,   nCtlColor);    
	//return   hbr; 

	//CFont * cFont=new CFont;   
	//cFont->CreateFont(16,0,0,0,FW_SEMIBOLD,FALSE,FALSE,0,  ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH&FF_SWISS,"Arial");  //对特定的控件做修改 
	//switch(nCtlColor){   
	//case CTLCOLOR_STATIC: //对所有静态文本控件的设置 
	//	{pDC->SetBkMode(TRANSPARENT); //设置背景为透明 
	//	pDC->SetTextColor(RGB(255,255,0)); //设置字体颜色 
	//	pWnd->SetFont(cFont); //设置字体  
	//	HBRUSH B = CreateSolidBrush(RGB(125,125,255)); //创建画刷  
	//	return (HBRUSH) B; //返回画刷句柄  
	//	}   
	//case CTLCOLOR_EDIT: //对所有编辑框的设置 
	//	{pDC->SetBkMode(TRANSPARENT);   
	//	pDC->SetTextColor(RGB(255,255,0));   
	//	pWnd->SetFont(cFont);    
	//	HBRUSH B = CreateSolidBrush(RGB(125,125,255));   
	//	return (HBRUSH) B;   
	//	}   
	//default:
	//	return CDialog::OnCtlColor(pDC,pWnd, nCtlColor);
	//}


}

BOOL CMessageDialog::OnEraseBkgnd(CDC* pDC)
{
	//return CDialog::OnEraseBkgnd(pDC);
     CBrush back(DLG_BACKGROUND);  
     CBrush* pold=pDC->SelectObject(&back);  
     CRect rect;  
     pDC->GetClipBox (&rect);  
     pDC->PatBlt (rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);  
         pDC->SelectObject(pold);  

//int cx, cy;  
//CImage  image;  
////CRect   rect;  
////根据路径载入图片  
////image.Load(_T("C:\\仪表.jpg"));  
//image.Load(_T("C:\\0.png"));  
////获取图片的宽 高度  
//cx  = image.GetWidth();  
//cy  = image.GetHeight();
//rect.top =0;
//rect.left =0;
//rect.bottom=cy;
//rect.right =cx;
//image.Draw(pDC->m_hDC, rect);//将图片绘制到picture
    return TRUE;   
}

int CMessageDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(WS_CAPTION,0,0);//如果不想去掉标题栏，去掉该句。
	SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
	//SetBackgroundColor(RGB(94,94,94),RGB(0,0,0)); 
	ShowWindow(SW_SHOWMAXIMIZED);

	return 0;
}


void CMessageDialog::OnClose()
{
	DestroyWindow();

	CDialog::OnClose();
}


void CMessageDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

//	int cx, cy;  
//CImage  image;  
//CRect   rect;  
//      
////根据路径载入图片  
////image.Load(_T("C:\\QQ图片20171103163038.png"));  
//image.Load(_T("C:\\仪表.jpg"));  
////image.Load(_T("C:\\0.png"));  
////获取图片的宽 高度  
//cx  = image.GetWidth();  
//cy  = image.GetHeight();  
//      
////获取Picture Control控件的大小  
////GetDlgItem(IDC_PICTURE_METER)->GetWindowRect(&rect);  
////将客户区选中到控件表示的矩形区域内  
////ScreenToClient(&rect);  
////窗口移动到控件表示的区域  
////GetDlgItem(IDC_PICTURE_METER)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);  
////CWnd *pWnd = NULL;  
////pWnd    = GetDlgItem(IDC_PICTURE_METER);//获取控件句柄  
////pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小  
////GetClientRect(&rect);//获取句柄指向控件区域的大小  
//
////CRect rect;
//GetClientRect(&rect);
//
//CDC *pDc = NULL;  
////pDc = pWnd->GetDC();//获取picture的DC  
// pDc =    GetDC();
////RECT rect;
//rect.top =0;
//rect.left =0;
//rect.bottom=cy;
//rect.right =cx;
////image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内  
//  
//image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内  
////pDc->SetBkMode(TRANSPARENT); //透明
//////pDc->SelectObject(font2);//换字体
////pDc->SetTextColor(RGB(255,255,255));//白色
//////pDc->DrawText(_T("修改"),CRect(0,0,100,100),DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK);
////pDc->DrawText(m_strInfo,CRect(0,0,100,100),DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK);
//
//ReleaseDC(pDc);  
}


void CMessageDialog::OnBnClickedOk()
{
	//CGigeCameraDemoDlg *pDlg= (CGigeCameraDemoDlg *)::AfxGetApp()->GetMainWnd();
	
	//		pDlg->m_iCurrentCount_1++;

	CDialog::OnOK();
}


BOOL CMessageDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	//SetTimer(3,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CMessageDialog::OnTimer(UINT_PTR nIDEvent)
{

	CDialog::OnTimer(nIDEvent);
}

void CMessageDialog::OnLButtonUp(UINT nFlags, CPoint point)
{

	CDialog::OnLButtonUp(nFlags, point);
}
