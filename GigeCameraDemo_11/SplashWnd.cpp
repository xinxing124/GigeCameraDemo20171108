#include "stdafx.h"
#include "math.h"
#include "resource.h"
#include "SplashWnd.h"


// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

bool ExitFlag;

DWORD WINAPI  On_Timer(LPVOID pParam);

CSplashWnd::CSplashWnd()
: m_strInfo(_T("Initializing..."))
, m_dbOldSpeed(-1)
{
	ImageFrom_IDResource(IDB_PNG57,"PNG",&image);
	//m_Bitmap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP1)); //Load Bitmap 
	//m_Bitmap.GetBitmap(&bmBitmap);        //Get Bitmap Info 
	//m_lWidth=bmBitmap.bmWidth,  //Bitmap Width = Splash Window Width 
	//m_lHeight=bmBitmap.bmHeight, //Bitmap Height = Splash Window Height 
	m_lWidth=image.GetWidth();  
	m_lHeight=image.GetHeight();
}

CSplashWnd::~CSplashWnd()
{
	if(g_hThread!=NULL)
	{
		CloseHandle(g_hThread);
	}

}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
ON_WM_PAINT() 
ON_WM_NCDESTROY()
END_MESSAGE_MAP()



// CSplashWnd 消息处理程序

void CSplashWnd::DeleteSplash() 
{
	ExitFlag=true;
}
void CSplashWnd::CreateSplash() 
{ 
//Create Splash Window 
//CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
 //SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

CWnd::CreateEx(0, 
  AfxRegisterWndClass( 
  0, 
  //AfxGetApp()->LoadStandardCursor(IDC_UPARROW)), 
  //AfxGetApp()->LoadStandardCursor(IDC_ARROW)), 
  //AfxGetApp()->LoadCursorA(IDC_WAIT),// LoadStandardCursor(IDC_ARROW)), 
  AfxGetApp()->LoadStandardCursor(IDC_WAIT)), 
  
  NULL, 
  //            "SplashWindow Sample", 
  WS_POPUP, 
  0, 
  0, 
  m_lWidth,  //Bitmap Width = Splash Window Width 
  m_lHeight, //Bitmap Height = Splash Window Height 
  NULL, 
  NULL, 
  NULL); 

this->CenterWindow(); 
this->ShowWindow(SW_SHOW);
this->UpdateWindow(); 
//HideCursor();
ExitFlag=false;
g_hThread= CreateThread(0,NULL,On_Timer,(LPTHREAD_START_ROUTINE)this,0,NULL);
} 

void CSplashWnd::OnPaint() 
{ 

    CPaintDC dc(this); 

    //CBrush brush; 
    //brush.CreateSolidBrush(RGB(64,64,255)); 
    //dc.SelectObject(&brush); 
    //dc.Rectangle(0,0,bmBitmap.bmWidth,bmBitmap.bmHeight); 

    //mMemDC.CreateCompatibleDC(NULL); //Create Memory DC 
    //Old_Bitmap = mMemDC.SelectObject(&m_Bitmap); //Select DC 
    //int num = bmBitmap.bmWidth/40; 
    //dc.StretchBlt(0,0,bmBitmap.bmWidth,bmBitmap.bmHeight,&mMemDC,0,0,bmBitmap.bmWidth,bmBitmap.bmHeight,SRCCOPY); 
	//mMemDC.SelectObject(Old_Bitmap); //Select Bitmap 
	CRect rect; 
	rect.top =0;
	rect.left =0;
	rect.bottom=m_lHeight;   
	rect.right =m_lWidth; 
    image.Draw(dc.m_hDC, rect);//将图片绘制到picture表示的区域内  

	CFont font;
	font.CreateFont(28, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));    //Static控件1的字体颜色-红色
	dc.SelectObject(&font); 
	CRect m_RectInfoShow(22,728,1000,760);//信息区域

    dc.DrawText(m_strInfo,m_RectInfoShow, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
    font.DeleteObject(); 

} 

void CSplashWnd::ImageFrom_IDResource(UINT nID, LPCTSTR sTR, CImage *  pImg) 
{
	HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE(nID), _T(sTR));  
    if(hrsrc)  
    {  
      HGLOBAL hglb = LoadResource(NULL, hrsrc);  
      LPVOID lpsz = LockResource(hglb);  
      ULARGE_INTEGER srcSize;  
      srcSize.QuadPart = SizeofResource(NULL, hrsrc);  
        
      IStream * pStream = NULL;  
      CreateStreamOnHGlobal(NULL, TRUE, &pStream);   
      if(pStream)  
      {  
        pStream->SetSize(srcSize);  
        DWORD cbWritten = 0;  
        pStream->Write(lpsz, srcSize.LowPart, &cbWritten);  
          
		pImg->Load(pStream);  
        //VERIFY( m_BkImage.Load(pStream) == S_OK );  
        pStream->Release();  
      }  
    }  
}
void CSplashWnd::SetInfo(CString info)
{
	m_strInfo=info;
}
//void CSplashWnd::OnTimer(UINT_PTR nIDEvent)
DWORD WINAPI On_Timer(LPVOID pParam)
{
	CSplashWnd *pDlg = (CSplashWnd *)pParam;
	int iInfo;
	static int nDisplay=-1;
	while(!ExitFlag)
   {
		if(pDlg->m_strInfo.Find("Initializing")>-1) 
		{
			nDisplay=nDisplay>3?0:nDisplay+1;   
			switch(nDisplay)   
			{   
			case 0: pDlg->m_strInfo="Initializing"; break;   
			case 1: pDlg->m_strInfo="Initializing."; break;   
			case 2: pDlg->m_strInfo="Initializing.."; break;   
			case 3: pDlg->m_strInfo="Initializing..."; break;   
			}          
		}
		if((iInfo=pDlg->m_strInfo.Find('$'))>1)
			pDlg->m_strInfo=pDlg->m_strInfo.Mid(1,iInfo-1);
		CDC *pDc;
		pDc=pDlg->GetDC();
		CDC  * pMemDc=NULL; //加载背景图片的内存DC
		CRect rect; 
		rect.top =0;
		rect.left =0;
		rect.bottom= pDlg->m_lHeight;   
		rect.right = pDlg->m_lWidth; 

		pMemDc=new CDC;
		pMemDc->CreateCompatibleDC(pDc);
		CBitmap bmp;//这里的Bitmap是必须的，否则当心弄出一个大黑块哦。
		bmp.CreateCompatibleBitmap(pDc,rect.Width(),rect.Height());
		pMemDc->SelectObject(&bmp);

		
		pDlg->image.Draw(pMemDc->m_hDC, rect);//将图片绘制到picture表示的区域内  

		CFont font;
		//font.CreateFont(15, 0, 0, 0, FW_BOLD,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"Times New Roman");
		font.CreateFont(40, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
		pMemDc->SetBkMode(TRANSPARENT);
		pMemDc->SetTextColor(RGB(255,255,255));    //Static控件1的字体颜色-红色
		pMemDc->SelectObject(&font); 

		CString str;
		CRect m_RectDateTimeShow(642,48,1000,86);//日期时间
		CTime time = CTime::GetCurrentTime();       
		str.Format("%04d-%02d-%02d %02d:%02d:%02d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());  
		pMemDc->DrawText(str,m_RectDateTimeShow, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
		font.DeleteObject(); 

		font.CreateFont(28, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
		pMemDc->SetBkMode(TRANSPARENT);
		pMemDc->SetTextColor(RGB(255,255,255));    //Static控件1的字体颜色-红色
		pMemDc->SelectObject(&font); 
		CRect m_RectInfoShow(22,728,1000,760);//信息区域
		pMemDc->DrawText( pDlg->m_strInfo,m_RectInfoShow, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
		font.DeleteObject(); 

		double SpeedVal=0.0;
		pDlg->m_dbOldSpeed=SpeedVal;
		CPen RectPen(PS_SOLID,5,RGB(255,0,0));
		pMemDc->SelectObject(&RectPen); 
		pMemDc->MoveTo(313,282);
		pMemDc->LineTo((int)(313+cos((360-(180-(0+SpeedVal*3.38/5*180/27)))*PI/180)*200),(int)(282+sin((360-(180-(0+SpeedVal*3.38/5*180/27)))*PI/180)*200));
		RectPen.DeleteObject();
		pDc->BitBlt(0,0,pDlg->m_lWidth,pDlg->m_lHeight,pMemDc,0,0,SRCCOPY);
		bmp.DeleteObject();
		pMemDc->DeleteDC();
		pDc->DeleteDC();

		Sleep(500);
	}
	//CWnd::OnTimer(nIDEvent);
	return true;
}

void CSplashWnd::OnNcDestroy()
{
	CWnd::OnNcDestroy();

	::PostQuitMessage(0);//为了使线程自动退出. 
}

void CSplashWnd::HideCursor()//隐藏光标
{
 //CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
 //SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
  HCURSOR  hc;

hc=LoadCursor(NULL,IDC_CROSS);

            SetCursor(hc);
}