// GigeCameraDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinSvc.h"
#include "GigeCameraDemo.h"
#include "GigeCameraDemoDlg.h"
#include "Gdiplus.h"  
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PIC_WIDTH    2048
#define PIC_HEIGHT   2048
#define	PIC_BUFF	PIC_WIDTH*PIC_HEIGHT
char*	pImageBuff_1;
//char	pImageBuff_1[PIC_BUFF];
char*	pImageBuff_2;

//CList<char *,char*&> listBuff_1;
//CList<char*,char*&> listBuff_2;
//
//struct tag_ImageBuff {
//	char				pBuff[MAX_BUFF];
//	int                 iImageCount;
//	struct tag_ImageBuff *next;          //单链表的指针域   
//};
//tag_ImageBuff *tag_head,*tag_curr,*tag_tail,*tag_tmp;

WINDOWPLACEMENT m_wp;
//装载图像数据的环形缓冲数组
BUFFNODE		listImageBuff_1[NUM_BUFF];
static int				listImageWriteIndex_1=-1;
static int				listImageReadIndex_1=-1;
BUFFNODE		listImageBuff_2[NUM_BUFF];	
static int				listImageWriteIndex_2=-1;
static int				listImageReadIndex_2=-1;
ULONGLONG GetPathUseSpace(const char *szPath);
ULONGLONG GetDirFileCount(CString strPath);
double ReadFreeSpace(CString strPath);
//存储图片数据
CString m_strStorageFormat;
char* m_cSavePath_1;
char* m_cSavePath_2;
SapBuffer		*m_Buffers_1;
SapBuffer		*m_Buffers_2;
bool m_bDirExit_1=false;
bool m_bDirExit_2=false;
bool PushImageToMyBuffer(BUFFNODE listImageBuff[],char *pBuffer,int iBufferLength,int iWidth,int iHeight,int iImageType,int iImageCount);
bool PushImageToMyBuffer_1(BUFFNODE listImageBuff[],char *pBuffer,int iBufferLength,int iWidth,int iHeight,int iImageType,int iImageCount);
bool PushImageToMyBuffer_2(BUFFNODE listImageBuff[],char *pBuffer,int iBufferLength,int iWidth,int iHeight,int iImageType,int iImageCount);
DWORD WINAPI   SaveImageData(LPVOID pParam);
DWORD WINAPI   SaveImageData_1(LPVOID pParam);
DWORD WINAPI   SaveImageData_2(LPVOID pParam);
//存储RGB24的图像数据指针
static char    *m_pRgb24Buffer;
static SapBayer::Align m_align[4];
static FILE *m_pfLog_1;
static FILE *m_pfLog_2;
static ULONGLONG m_iCountImage_1;
static ULONGLONG m_iCountImage_2;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGigeCameraDemoDlg dialog

CGigeCameraDemoDlg::CGigeCameraDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGigeCameraDemoDlg::IDD, pParent)
	,m_strCurrMileage("")
	,m_strTempMileage(_T(""))
	,m_iDirectionFlag(0)
	,m_iAddMileageFlag(0)
	, m_dbVoltage_1(0)
	, m_dbVoltage_2(0)
	, m_dbSpeed(0)
	,m_iExitCount(0)
	, m_dbTotalMileage(0)
	, m_strInfo(_T(""))
	, m_iTempDirection(0)
	, m_iTempAddMileage(0)
	, m_iDotInfo(0)
	, m_iWindowMode(0)
	, m_iFirstFlag(0)
	, m_dbCurrMileage(0.0)
	, m_dbLastMileage(0.0)
{
	//{{AFX_DATA_INIT(CGigeCameraDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_AcqDevice_1			= NULL;
	m_Buffers_1			= NULL;
	m_Xfer_1				= NULL;
	m_View_1              = NULL;
	m_SaveCount_1			=0;
	m_AcqDevice_2			= NULL;
	m_Buffers_2			= NULL;
	m_Xfer_2				= NULL;
	m_View_2              = NULL;
	m_SaveCount_2		=0;

	m_Bayer             = NULL;
	m_iBayerEnable      = 0;
	m_pRgb24Buffer      = NULL;
	m_align[0]          = SapBayer::AlignGBRG;
	m_align[1]          = SapBayer::AlignBGGR;
	m_align[2]          = SapBayer::AlignRGGB;
	m_align[3]          = SapBayer::AlignGRBG;
	m_iBayerType        = 2;
	m_pfLog_1             = NULL;
	m_pfLog_2             = NULL;
	m_iCountImage_1       = 3;
	m_iCountImage_2       = 0;
	m_iCurrentCount_1 = 0;
	m_iCurrentCount_2 = 0;
	listImageWriteIndex_1=-1;
	listImageReadIndex_1=-1;
	listImageWriteIndex_2=-1;
	listImageReadIndex_2=-1;

	m_pTimeFileName= NULL;
	m_TimeCount=0;
	m_strReceive = _T("");
	m_bTimerFlag=false;
	m_RevFlag=false;
}

void CGigeCameraDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGigeCameraDemoDlg)
	DDX_Control(pDX, IDC_STATUS, m_statusWnd);
	DDX_Control(pDX, IDC_VIEW_WND1, m_ImageWnd_1);
	DDX_Control(pDX, IDC_VIEW_WND2, m_ImageWnd_2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGigeCameraDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CGigeCameraDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_BN_CLICKED(IDC_GRAB, &CGigeCameraDemoDlg::OnGrab)
	ON_BN_CLICKED(IDC_FREEZE, &CGigeCameraDemoDlg::OnFreeze)
   ON_WM_ENDSESSION()
   ON_WM_QUERYENDSESSION()
   ON_MESSAGE(WM_SHOWTASK,onShowTask)
   ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveData)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MAINITEM_SHOW, &CGigeCameraDemoDlg::OnMainitemShow)
	ON_COMMAND(ID_MAINITEM_HIDE, &CGigeCameraDemoDlg::OnMainitemHide)
	ON_COMMAND(ID_MAINITEM_EXIT, &CGigeCameraDemoDlg::OnMainitemExit)
	ON_COMMAND(ID_MAINITEM_TEST, &CGigeCameraDemoDlg::OnMainitemTest)
	ON_WM_NCPAINT()
	ON_WM_TIMER()
	
	ON_BN_CLICKED(IDC_RETURN, &CGigeCameraDemoDlg::OnBnClickedReturn)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGigeCameraDemoDlg message handlers

LRESULT   CGigeCameraDemoDlg::onShowTask(WPARAM   wParam,LPARAM   lParam) 
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
{ 
    if(wParam!=IDR_MAINFRAME) 
    return   1; 
    switch(lParam) 
    { 
            case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个"关闭" 
            { 
                LPPOINT   lpoint=new   tagPOINT; 
                ::GetCursorPos(lpoint);//得到鼠标位置 
				CMenu menu,*popmenu;
				menu.LoadMenu(IDR_MENU1);
				popmenu = menu.GetSubMenu(0);

				#if !_DEBUG
					popmenu->RemoveMenu(3,MF_BYPOSITION); 
				#endif

				CPoint myPoint;
				GetCursorPos(&myPoint);  //鼠标位置
				SetForegroundWindow(); 
				popmenu->TrackPopupMenu(TPM_LEFTALIGN,myPoint.x,myPoint.y,this);
				PostMessage(WM_NULL,0,0);
				popmenu->DestroyMenu(); 
                delete   lpoint; 
            } 
            break; 
        } 
        return   0; 
    } 

//将数据缓存进入内存 BUFFNODE		listImageBuff_1[NUM_BUFF];
bool PushImageToMyBuffer(BUFFNODE listImageBuff[],char *pBuffer,int iBufferLength,int iWidth,int iHeight,int iImageType,int iImageCount)
{
/*
	int index = 0;
	int ui8tmp = 0;
	int iBreak = FALSE;
	static int ui8BufFlag = -1;

	//ui8BufFlag=(ui8BufFlag==NUM_BUFF-1?0:ui8BufFlag+1);


	if(ui8BufFlag==NUM_BUFF-1)
	{
		ui8tmp=0;
	}
	else
	{
		ui8tmp=ui8BufFlag+1;
	}
		
	for( index=ui8tmp; index<ui8tmp+NUM_BUFF; index++ )
	{
	  if( index>NUM_BUFF-1 )
	  {
		 if( listImageBuff[index-NUM_BUFF].bIsLocked  == FALSE)
		 {
			ui8BufFlag = index-NUM_BUFF;
			iBreak = 1;
			break;
		 }
	   }
	   else
	   {
			if( listImageBuff[index].bIsLocked == FALSE)
			{
				ui8BufFlag = index;
				iBreak = 1;
				break;
			}
	   }
	} 

	if(iBreak == 0)
	{
		return false;
	}

	memcpy(listImageBuff[ui8BufFlag].pBuff,pBuffer,iBufferLength);
	listImageBuff[ui8BufFlag].sizeBuff  = iBufferLength;
	listImageBuff[ui8BufFlag].iImageType = iImageType;
	listImageBuff[ui8BufFlag].iImageWidth = iWidth;
	listImageBuff[ui8BufFlag].iImageHeight = iHeight;
	listImageBuff[ui8BufFlag].iImageCount = iImageCount;
	listImageBuff[ui8BufFlag].bIsLocked = TRUE;
	*/
	return true;
}

bool PushImageToMyBuffer_1(BUFFNODE listImageBuff[],char *pBuffer,int iBufferLength,int iWidth,int iHeight,int iImageType,int iImageCount)
{
	int index = 0;
	int ui8tmp = 0;
	int iBreak = 0;
	static int ui8BufFlag_1 = -1;

	if(ui8BufFlag_1==NUM_BUFF-1)
	{
		ui8tmp=0;
	}
	else
	{
		ui8tmp=ui8BufFlag_1+1;
	}
		
	for( index=ui8tmp; index<ui8tmp+NUM_BUFF; index++ )
	{
	  if( index>NUM_BUFF-1 )
	  {
		 if( listImageBuff[index-NUM_BUFF].bIsLocked  == FALSE)
		 {
			ui8BufFlag_1 = index-NUM_BUFF;
			iBreak = 1;
			break;
		 }
	   }
	   else
	   {
			if( listImageBuff[index].bIsLocked == FALSE)
			{
				ui8BufFlag_1 = index;
				iBreak = 1;
				break;
			}
	   }
	} 

	if(iBreak == 0)
	{
		return false;
	}

	memcpy(listImageBuff[ui8BufFlag_1].pBuff,pBuffer,iBufferLength);
	listImageBuff[ui8BufFlag_1].sizeBuff  = iBufferLength;
	listImageBuff[ui8BufFlag_1].iImageType = iImageType;
	listImageBuff[ui8BufFlag_1].iImageWidth = iWidth;
	listImageBuff[ui8BufFlag_1].iImageHeight = iHeight;
	listImageBuff[ui8BufFlag_1].iImageCount = iImageCount;
	listImageBuff[ui8BufFlag_1].bIsLocked = TRUE;

	return true;
}

bool PushImageToMyBuffer_2(BUFFNODE listImageBuff[],char *pBuffer,int iBufferLength,int iWidth,int iHeight,int iImageType,int iImageCount)
{
	int index = 0;
	int ui8tmp = 0;
	int iBreak = 0;
	static int ui8BufFlag_2 = -1;


	if(ui8BufFlag_2==NUM_BUFF-1)
	{
		ui8tmp=0;
	}
	else
	{
		ui8tmp=ui8BufFlag_2+1;
	}
		
	for( index=ui8tmp; index<ui8tmp+NUM_BUFF; index++ )
	{
	  if( index>NUM_BUFF-1 )
	  {
		 if( listImageBuff[index-NUM_BUFF].bIsLocked  == FALSE)
		 {
			ui8BufFlag_2 = index-NUM_BUFF;
			iBreak = 1;
			break;
		 }
	   }
	   else
	   {
			if( listImageBuff[index].bIsLocked == FALSE)
			{
				ui8BufFlag_2 = index;
				iBreak = 1;
				break;
			}
	   }
	} 

	if(iBreak == 0)
	{
		return false;
	}

	memcpy(listImageBuff[ui8BufFlag_2].pBuff,pBuffer,iBufferLength);
	listImageBuff[ui8BufFlag_2].sizeBuff  = iBufferLength;
	listImageBuff[ui8BufFlag_2].iImageType = iImageType;
	listImageBuff[ui8BufFlag_2].iImageWidth = iWidth;
	listImageBuff[ui8BufFlag_2].iImageHeight = iHeight;
	listImageBuff[ui8BufFlag_2].iImageCount = iImageCount;
	listImageBuff[ui8BufFlag_2].bIsLocked = TRUE;

	return true;
}

BOOL CreateFile2(CString fileName, CString content, BOOL canBeEmptyFile)
{
    if(content.GetLength() > 0 || canBeEmptyFile)
    {

        CFile outFile;              //  注：CStdioFile是CFile的子类，这里用这个类也是可以的
        outFile.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
        /*
        上面用Open函数时：
        1) 参数只用modeCreate的话，则只会创建文件(如果文件不存在则创建，否则清空内容)

        2) 如果只用modeWrite的话，如果文件不存在，则打开失败，否则打开，并添加内容到文件头部

        3) 如果在参数最后加了CFile::typeText的话，不管文件是否存在及前面是否其他参数，都会失败，不知道为什么？？

        4) 用CFile::typeBinary的话，不管文件是否存在，都会成功！
        */


        outFile.Write(content, content.GetLength());
        outFile.Close();
        return TRUE;
    }
    return FALSE;
}

DWORD WINAPI   SaveImageData(LPVOID pParam)
{
	CGigeCameraDemoDlg *pDlg = (CGigeCameraDemoDlg *)pParam;
	/*
    int index = 0;
	int ui8tmp_1 = 0;
	int iBreak_1 = FALSE;
	static int ui8BufIndex_1 = -1;
	int ui8tmp_2 = 0;
	int iBreak_2 = FALSE;
	static int ui8BufIndex_2 = -1;

	SYSTEMTIME time;
	char ch_ImageFileName_1[1024];
	char ch_ImageFileName_2[1024];

   while(1)
   {
	   if(pDlg->m_dFreeSpaceG<1)
		   continue;
	   iBreak_1 = 0;
	   iBreak_2 = 0;
		if(ui8BufIndex_1==NUM_BUFF-1)
		{
			    ui8tmp_1=0;
		}
		else
		{
			ui8tmp_1=ui8BufIndex_1+1;
		}
			
		for( index=ui8tmp_1; index<ui8tmp_1+NUM_BUFF; index++ )
		{
			if( index>NUM_BUFF-1 )
			{
				if( listImageBuff_1[index-NUM_BUFF].bIsLocked  == TRUE)
				{
					ui8BufIndex_1 = index-NUM_BUFF;
					iBreak_1 = 1;
					break;
				}
			}
			else
			{
				if( listImageBuff_1[index].bIsLocked == TRUE)
				{
					ui8BufIndex_1 = index;
					iBreak_1 = 1;
					break;
				}
			}
		} 
		
		if(ui8BufIndex_2==NUM_BUFF-1)
		{
			    ui8tmp_2=0;
		}
		else
		{
			ui8tmp_2=ui8BufIndex_2+1;
		}
			
		for( index=ui8tmp_2; index<ui8tmp_2+NUM_BUFF; index++ )
		{
			if( index>NUM_BUFF-1 )
			{
				if( listImageBuff_2[index-NUM_BUFF].bIsLocked  == TRUE)
				{
					ui8BufIndex_2 = index-NUM_BUFF;
					iBreak_2 = 1;
					break;
				}
			}
			else
			{
				if( listImageBuff_2[index].bIsLocked == TRUE)
				{
					ui8BufIndex_2 = index;
					iBreak_2 = 1;
					break;
				}
			}
		} 

		if(iBreak_1 == 0)
		{
			Sleep(10);
			continue;
		}
		if(iBreak_2 == 0)
		{
			Sleep(10);
			continue;
		}
		// ::GetLocalTime(&time);
	    //m_pTimeFileName = ZLBGetFormatTime(time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
		if(!m_bDirExit_1)
		{
			//判断目录是否存在,如果不存在就创建目录
			ZLBCreateDirectory(m_cSavePath_1);
			ZLBCreateDirectory(m_cSavePath_2);
			m_bDirExit_1=true;
		}
		//if(pDlg->m_iStorageFormat == 0)
		//{
		//	sprintf(ch_ImageFileName_1,"%s\\%d.bmp",m_cSavePath_1,listImageBuff_1[ui8BufIndex_1].iImageCount);
		//	sprintf(ch_ImageFileName_2,"%s\\%d.bmp",m_cSavePath_2,listImageBuff_2[ui8BufIndex_2].iImageCount);
		//}
		//else if(pDlg->m_iStorageFormat == 1)
		//{
		//	sprintf(ch_ImageFileName_1,"%s\\%d.jpg",m_cSavePath_1,listImageBuff_1[ui8BufIndex_1].iImageCount);
		//	sprintf(ch_ImageFileName_2,"%s\\%d.jpg",m_cSavePath_2,listImageBuff_2[ui8BufIndex_2].iImageCount);
		//}

		//if(pDlg->m_iPixelFormat == 0)
		//{
		//	//pDlg->m_SaveCount_1=listImageBuff_1[ui8BufIndex_1].iImageCount;
		//	//pDlg->m_SaveCount_2=listImageBuff_2[ui8BufIndex_2].iImageCount;
		//	ZLBSaveImage(ch_ImageFileName_1,(char*)listImageBuff_1[ui8BufIndex_1].pBuff,listImageBuff_1[ui8BufIndex_1].iImageWidth,listImageBuff_1[ui8BufIndex_1].iImageHeight,8,pDlg->m_iImageColor,pDlg->m_iJpegFac);
		//	ZLBSaveImage(ch_ImageFileName_2,(char*)listImageBuff_2[ui8BufIndex_2].pBuff,listImageBuff_2[ui8BufIndex_2].iImageWidth,listImageBuff_2[ui8BufIndex_2].iImageHeight,8,pDlg->m_iImageColor,pDlg->m_iJpegFac);
		//	//CreateFile2(ch_ImageFileName_1,"XX",true);
		//	//CreateFile2(ch_ImageFileName_2,"XX",true);
		//}

		sprintf(ch_ImageFileName_1,"%s\\%d.bmp",m_cSavePath_1,listImageBuff_1[ui8BufIndex_1].iImageCount);
		sprintf(ch_ImageFileName_2,"%s\\%d.bmp",m_cSavePath_2,listImageBuff_2[ui8BufIndex_2].iImageCount);
		ZLBSaveImage(ch_ImageFileName_1,(char*)listImageBuff_1[ui8BufIndex_1].pBuff,listImageBuff_1[ui8BufIndex_1].iImageWidth,listImageBuff_1[ui8BufIndex_1].iImageHeight,8,1,80);
		ZLBSaveImage(ch_ImageFileName_2,(char*)listImageBuff_2[ui8BufIndex_2].pBuff,listImageBuff_2[ui8BufIndex_2].iImageWidth,listImageBuff_2[ui8BufIndex_2].iImageHeight,8,1,80);

		listImageBuff_1[ui8BufIndex_1].bIsLocked = FALSE;
		listImageBuff_2[ui8BufIndex_2].bIsLocked = FALSE;
   }
   */
   return TRUE;
}

DWORD WINAPI   SaveImageData_1(LPVOID pParam)
{
	CGigeCameraDemoDlg *pDlg = (CGigeCameraDemoDlg *)pParam;
 //   int index = 0;
	//int ui8tmp_1 = 0;
	//int iBreak_1 = FALSE;
	//char ch_ImageDir_1[1024];
	//char*data;
	//static int ui8BufIndex_1 = -1;
	time_t start;
	time_t end;
	double timeSpace;

	SYSTEMTIME time;
	char ch_ImageFileName_1[1024];
	
   while(1)
   {
	   if(pDlg->m_dFreeSpaceG<1)
		   continue;
	   if(m_iCountImage_1<1)
		   continue;

	   if(!m_bDirExit_1)
		{
			//判断目录是否存在,如果不存在就创建目录
			m_bDirExit_1=true;
			ZLBCreateDirectory(m_cSavePath_1);
		}
		
		start = clock();
	    listImageReadIndex_1=(listImageReadIndex_1>=NUM_BUFF-1?0:listImageReadIndex_1+1);
		if(listImageBuff_1[listImageReadIndex_1].bIsLocked == TRUE)
		{
			sprintf_s(ch_ImageFileName_1,"%s\\%08d.%s",m_cSavePath_1,listImageBuff_1[listImageReadIndex_1].iImageCount,m_strStorageFormat);
			ZLBSaveImage(ch_ImageFileName_1,(char*)listImageBuff_1[listImageReadIndex_1].pBuff,listImageBuff_1[listImageReadIndex_1].iImageWidth,listImageBuff_1[listImageReadIndex_1].iImageHeight,8,1,80);
			//memcpy(&P_dest[i], &dest, sizeof(dest));
			listImageBuff_1[listImageReadIndex_1].bIsLocked = FALSE;
		}
		end = clock();
		timeSpace = end - start;
		if(m_pfLog_1 != NULL)
		{
			pDlg->m_iCurrentCount_1++;
			SYSTEMTIME time;
			::GetLocalTime(&time);
			fprintf_s(m_pfLog_1,"LEFT:%d-%d-%d %d:%d:%d.%d tcount=%d acount=%d time=%f\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_1,pDlg->m_iCurrentCount_1,timeSpace);
		}

	 //  iBreak_1 = 0;

	 //  start = clock();
		//if(ui8BufIndex_1==NUM_BUFF-1)
		//{
		//	    ui8tmp_1=0;
		//}
		//else
		//{
		//	ui8tmp_1=ui8BufIndex_1+1;
		//}
		//	
		//for( index=ui8tmp_1; index<ui8tmp_1+NUM_BUFF; index++ )
		//{
		//	if( index>NUM_BUFF-1 )
		//	{
		//		if( listImageBuff_1[index-NUM_BUFF].bIsLocked  == TRUE)
		//		{
		//			ui8BufIndex_1 = index-NUM_BUFF;
		//			iBreak_1 = 1;
		//			break;
		//		}
		//	}
		//	else
		//	{
		//		if( listImageBuff_1[index].bIsLocked == TRUE)
		//		{
		//			ui8BufIndex_1 = index;
		//			iBreak_1 = 1;
		//			break;
		//		}
		//	}
		//} 
		//
		//if(iBreak_1 == 0)
		//{
		//	Sleep(10);
		//	continue;
		//}

	 //  //if(tag_head==NULL)
		//  // continue;
	 // 

	 // 

	 ////  if(listBuff_1.IsEmpty())
		////    continue;;
	 ////  
	 ////  data=new char[PIC_BUFF];
	 ////  //data=listBuff_1.GetHead();
	 ////  memcpy(data,listBuff_1.GetHead(),PIC_BUFF);
		//if(!m_bDirExit_1)
		//{
		//	//判断目录是否存在,如果不存在就创建目录
		//	m_bDirExit_1=true;
		//	ZLBCreateDirectory(m_cSavePath_1);
		//}

		////sprintf(ch_ImageFileName_1,"%s\\%d.bmp",m_cSavePath_1,m_iCountImage_1++);
		////ZLBSaveImage(ch_ImageFileName_1,(char*)tag_head->pBuff,PIC_WIDTH,PIC_HEIGHT,8,1,80);
		////tag_curr=tag_head->next;
		////free(tag_head);
		////tag_head=tag_curr;
		//		
		////if((listImageBuff_1[ui8BufIndex_1].iImageCount-1)%100==0)
		////{
		////	sprintf(ch_ImageDir_1,"%s\\%d_%d",m_cSavePath_1,listImageBuff_1[ui8BufIndex_1].iImageCount,listImageBuff_1[ui8BufIndex_1].iImageCount+99);
		////	ZLBCreateDirectory(ch_ImageDir_1);
		////}
		//int ncount=listImageBuff_1[ui8BufIndex_1].iImageCount;
		//sprintf(ch_ImageFileName_1,"%s\\%d.bmp",m_cSavePath_1,ncount);
		//ZLBSaveImage(ch_ImageFileName_1,(char*)listImageBuff_1[ui8BufIndex_1].pBuff,listImageBuff_1[ui8BufIndex_1].iImageWidth,listImageBuff_1[ui8BufIndex_1].iImageHeight,8,1,80);
		//
		////if(m_pfLog != NULL)
	 ////  {
		////	pDlg->m_iCurrentCount_1++;
		////	::GetLocalTime(&time);
		////	fprintf(m_pfLog,"LEFT SAVE:%d-%d-%d %d:%d:%d.%d count = %d m_iCurrentCount = %d index = %d %d%d%d%d%d\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,listImageBuff_1[ui8BufIndex_1].iImageCount,pDlg->m_iCurrentCount_1,ui8BufIndex_1,listImageBuff_1[0].bIsLocked,listImageBuff_1[1].bIsLocked,listImageBuff_1[2].bIsLocked,listImageBuff_1[3].bIsLocked,listImageBuff_1[4].bIsLocked);
		////}
		//end = clock();
		//timeSpace = end - start;
		//if(m_pfLog_1 != NULL)
		//{
		//	pDlg->m_iCurrentCount_1++;
		//	SYSTEMTIME time;
		//	::GetLocalTime(&time);
		//	fprintf(m_pfLog_1,"LEFT SAVE:%d-%d-%d %d:%d:%d.%d count = %d m_iCountImage = %d convert spent time %f\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,listImageBuff_1[ui8BufIndex_1].iImageCount,pDlg->m_iCurrentCount_1,timeSpace);
		//}
		//listImageBuff_1[ui8BufIndex_1].bIsLocked = FALSE;
   }
   return TRUE;
}

DWORD WINAPI   SaveImageData_2(LPVOID pParam)
{
	CGigeCameraDemoDlg *pDlg = (CGigeCameraDemoDlg *)pParam;
 //   int index = 0;
	//int ui8tmp_2 = 0;
	//int iBreak_2 = FALSE;
	//static int ui8BufIndex_2 = -1;
	//char ch_ImageDir_2[1024];
	time_t start;
	time_t end;
	double timeSpace;

	SYSTEMTIME time;
	char ch_ImageFileName_2[1024];
	
   while(1)
   {
	   if(pDlg->m_dFreeSpaceG<1)
		   continue;
	   if(m_iCountImage_2<1)
		   continue;

	   if(!m_bDirExit_2)
		{
			//CString temp;
			//temp.Format("#Path:%s$\r\n",m_cSavePath_2);
			//pDlg->m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
			//判断目录是否存在,如果不存在就创建目录
			m_bDirExit_2=true;
			ZLBCreateDirectory(m_cSavePath_2);
			
		}
		
		start = clock();
		listImageReadIndex_2=(listImageReadIndex_2>=NUM_BUFF-1?0:listImageReadIndex_2+1);
		if(listImageBuff_2[listImageReadIndex_2].bIsLocked == TRUE)
		{
			sprintf_s(ch_ImageFileName_2,"%s\\%08d.%s",m_cSavePath_2,listImageBuff_2[listImageReadIndex_2].iImageCount,m_strStorageFormat);
			ZLBSaveImage(ch_ImageFileName_2,(char*)listImageBuff_2[listImageReadIndex_2].pBuff,listImageBuff_2[listImageReadIndex_2].iImageWidth,listImageBuff_2[listImageReadIndex_2].iImageHeight,8,1,80);
			//memcpy(&P_dest[i], &dest, sizeof(dest));
			listImageBuff_2[listImageReadIndex_2].bIsLocked = FALSE;
		}
		end = clock();
		timeSpace = end - start;
		if(m_pfLog_2 != NULL)
		{
			pDlg->m_iCurrentCount_2++;
			SYSTEMTIME time;
			::GetLocalTime(&time);
			fprintf(m_pfLog_2,"RIGHT:%d-%d-%d %d:%d:%d.%d tcount=%d acount=%d time=%f\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_2,pDlg->m_iCurrentCount_2,timeSpace);
		}

	 //  iBreak_2 = 0;
	 //  start = clock();
		//if(ui8BufIndex_2==NUM_BUFF-1)
		//{
		//	    ui8tmp_2=0;
		//}
		//else
		//{
		//	ui8tmp_2=ui8BufIndex_2+1;
		//}
		//	
		//for( index=ui8tmp_2; index<ui8tmp_2+NUM_BUFF; index++ )
		//{
		//	if( index>NUM_BUFF-1 )
		//	{
		//		if( listImageBuff_2[index-NUM_BUFF].bIsLocked  == TRUE)
		//		{
		//			ui8BufIndex_2 = index-NUM_BUFF;
		//			iBreak_2 = 1;
		//			break;
		//		}
		//	}
		//	else
		//	{
		//		if( listImageBuff_2[index].bIsLocked == TRUE)
		//		{
		//			ui8BufIndex_2 = index;
		//			iBreak_2 = 1;
		//			break;
		//		}
		//	}
		//} 

		//if(iBreak_2 == 0)
		//{
		//	Sleep(10);
		//	continue;
		//}
		//if(!m_bDirExit_2)
		//{
		//	//判断目录是否存在,如果不存在就创建目录
		//	m_bDirExit_2=true;
		//	ZLBCreateDirectory(m_cSavePath_2);
		//}
		////if((listImageBuff_2[ui8BufIndex_2].iImageCount-1)%100==0)
		////{
		////	sprintf(ch_ImageDir_2,"%s\\%d_%d",m_cSavePath_2,listImageBuff_2[ui8BufIndex_2].iImageCount,listImageBuff_2[ui8BufIndex_2].iImageCount+99);
		////	ZLBCreateDirectory(ch_ImageDir_2);
		////}
		//int ncount=listImageBuff_2[ui8BufIndex_2].iImageCount;
		//sprintf(ch_ImageFileName_2,"%s\\%d.bmp",m_cSavePath_2,ncount);
		////sprintf(ch_ImageFileName_2,"%s\\%d.bmp",m_cSavePath_2,listImageBuff_2[ui8BufIndex_2].iImageCount);
		//ZLBSaveImage(ch_ImageFileName_2,(char*)listImageBuff_2[ui8BufIndex_2].pBuff,listImageBuff_2[ui8BufIndex_2].iImageWidth,listImageBuff_2[ui8BufIndex_2].iImageHeight,8,1,80);
		////memcpy(&P_dest[i], &dest, sizeof(dest));
		////if(m_pfLog != NULL)
	 ////  {
		////pDlg->m_iCurrentCount_2++;
		////::GetLocalTime(&time);
		////fprintf(m_pfLog,"RIGHT SAVE:%d-%d-%d %d:%d:%d.%d count = %d m_iCurrentCount = %d index = %d %d%d%d%d%d\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,listImageBuff_2[ui8BufIndex_2].iImageCount,pDlg->m_iCurrentCount_2,ui8BufIndex_2,listImageBuff_2[0].bIsLocked,listImageBuff_2[1].bIsLocked,listImageBuff_2[2].bIsLocked,listImageBuff_2[3].bIsLocked,listImageBuff_2[4].bIsLocked);
		////}
		//end = clock();
		//timeSpace = end - start;
	 //  if(m_pfLog_2 != NULL)
	 //  {
		//   pDlg->m_iCurrentCount_2++;
		//   SYSTEMTIME time;
		//   ::GetLocalTime(&time);
		//   fprintf(m_pfLog_2,"RIGHT SAVE:%d-%d-%d %d:%d:%d.%d count = %d m_iCountImage = %d convert spent time %f\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,listImageBuff_2[ui8BufIndex_2].iImageCount,pDlg->m_iCurrentCount_2,timeSpace);
	 //  }

		//listImageBuff_2[ui8BufIndex_2].bIsLocked = FALSE;
   }
   return TRUE;
}

void CGigeCameraDemoDlg::XferCallback_1(SapXferCallbackInfo *pInfo)
{
	int iImageType = 0;
	int iCounter,iWidth,iHeight = 0;
    void *pImageData_1 = NULL;
    BOOL iSucceed = 0;
    SapBuffer *pBuffer_1 = NULL;
	char ch_ImageFileName_1[1024];
	char ch_tmp[100];
	int iCount = 0;
	time_t start;
	time_t end;
	double timeSpace;
	
	CGigeCameraDemoDlg *pDlg= (CGigeCameraDemoDlg *) pInfo->GetContext();
	start = clock();

	iImageType = 1;
    //pDlg->m_View_1->Show();

	pBuffer_1 = pDlg->m_Buffers_1;

	iWidth = pBuffer_1->GetWidth();
	iHeight = pBuffer_1->GetHeight();

   iCount =  pInfo->GetEventCount();
   //m_iCountImage_1=iCount;
   m_iCountImage_1++;
   iSucceed = (pBuffer_1->GetAddress(&pImageData_1));
   if(iSucceed)
   {
	   if(!m_bDirExit_1)
		{
			//判断目录是否存在,如果不存在就创建目录
			m_bDirExit_1=true;
			ZLBCreateDirectory(m_cSavePath_1);
		}
		
		start = clock();
		sprintf(ch_ImageFileName_1,"%s\\%08d.bmp",m_cSavePath_1,m_iCountImage_1);
		pBuffer_1->Save(ch_ImageFileName_1,"-format bmp");
		end = clock();
		timeSpace = end - start;
		if(m_pfLog_1 != NULL)
		{
			pDlg->m_iCurrentCount_1++;
			SYSTEMTIME time;
			::GetLocalTime(&time);
			fprintf(m_pfLog_1,"LEFT:%d-%d-%d %d:%d:%d.%d tcount=%d acount=%d time=%f\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_1,pDlg->m_iCurrentCount_1,timeSpace);
		}

		pBuffer_1->ReleaseAddress(&pImageData_1);
	   //end = clock();
	   //timeSpace = end - start;
   }
}

void CGigeCameraDemoDlg::XferCallback_2(SapXferCallbackInfo *pInfo)
{
	int iImageType = 0;
	int iCounter,iWidth,iHeight = 0;
    void *pImageData_2 = NULL;
    BOOL iSucceed = 0;
    SapBuffer *pBuffer_2 = NULL;
	char ch_ImageFileName_2[1024];
	int iCount = 0;
	time_t start;
	time_t end;
	double timeSpace;
	
	CGigeCameraDemoDlg *pDlg= (CGigeCameraDemoDlg *) pInfo->GetContext();
	start = clock();

	iImageType = 1;
    //pDlg->m_View_1->Show();


	pBuffer_2 = pDlg->m_Buffers_2;

	iWidth = pBuffer_2->GetWidth();
	iHeight = pBuffer_2->GetHeight();

   iCount =  pInfo->GetEventCount();
   //m_iCountImage_2=iCount;
   m_iCountImage_2++;
   iSucceed = (pBuffer_2->GetAddress(&pImageData_2));
   if(iSucceed)
   {
	   if(!m_bDirExit_2)
		{
			//CString temp;
			//temp.Format("#Path:%s$\r\n",m_cSavePath_2);
			//pDlg->m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
			//判断目录是否存在,如果不存在就创建目录
			m_bDirExit_2=true;
			ZLBCreateDirectory(m_cSavePath_2);
			
		}
		
		start = clock();
		sprintf(ch_ImageFileName_2,"%s\\%08d.bmp",m_cSavePath_2,m_iCountImage_2);
		pBuffer_2->Save(ch_ImageFileName_2,"-format bmp");
		end = clock();
		timeSpace = end - start;
		if(m_pfLog_2 != NULL)
		{
			pDlg->m_iCurrentCount_2++;
			SYSTEMTIME time;
			::GetLocalTime(&time);
			fprintf(m_pfLog_2,"RIGHT:%d-%d-%d %d:%d:%d.%d tcount=%d acount=%d time=%f\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_2,pDlg->m_iCurrentCount_2,timeSpace);
		}
	 //  	pImageBuff_2=new char[PIC_BUFF];
		//memcpy(pImageBuff_2,(char*)pImageData_2,iWidth*iHeight);
		//listBuff_2.AddTail(pImageBuff_2);
		//free(pImageBuff_2);
		//pBuffer_2->ReleaseAddress(&pImageData_2);

	 //  if(!m_bDirExit)
		//{
		//	//判断目录是否存在,如果不存在就创建目录
		//	ZLBCreateDirectory(m_cSavePath_1);
		//	ZLBCreateDirectory(m_cSavePath_2);
		//	m_bDirExit=true;
		//}
		//sprintf(ch_ImageFileName_2,"%s\\%d.bmp",m_cSavePath_2,iCount);
		//ZLBSaveImage(ch_ImageFileName_2,(char*)pImageData_2,iWidth,iHeight,8,1,80);
		////pBuffer->Save(ch_ImageFileName,"-format bmp");

		//pBuffer_2->ReleaseAddress(&pImageData_2);

		//if(PushImageToMyBuffer_2(listImageBuff_2,(char*)pImageData_2,iWidth*iHeight,iWidth,iHeight,iImageType,iCount))
		//{
		//	pBuffer_2->ReleaseAddress(&pImageData_2);
		//}	

	   	listImageWriteIndex_2=(listImageWriteIndex_2>=NUM_BUFF-1?0:listImageWriteIndex_2+1);
		if(listImageBuff_2[listImageWriteIndex_2].bIsLocked == FALSE)
		{
			memcpy(listImageBuff_2[listImageWriteIndex_2].pBuff,(char*)pImageData_2,iWidth*iHeight);
			listImageBuff_2[listImageWriteIndex_2].sizeBuff  = iWidth*iHeight;
			listImageBuff_2[listImageWriteIndex_2].iImageType = iImageType;
			listImageBuff_2[listImageWriteIndex_2].iImageWidth = iWidth;
			listImageBuff_2[listImageWriteIndex_2].iImageHeight = iHeight;
			listImageBuff_2[listImageWriteIndex_2].iImageCount =m_iCountImage_2;//*100+ iCount;
			listImageBuff_2[listImageWriteIndex_2].bIsLocked = TRUE;
			//m_iCountImage_2++;
			
		}
		pBuffer_2->ReleaseAddress(&pImageData_2);
	   //end = clock();
	   //timeSpace = end - start;
   }
}

void CGigeCameraDemoDlg::PixelChanged(int x, int y)
{
   CString str = m_appTitle;
   str += "  " + m_ImageWnd_1.GetPixelString(CPoint(x,y));
   str += "  " + m_ImageWnd_2.GetPixelString(CPoint(x,y));
   SetWindowText(str);
}

ULONGLONG GetPathUseSpace(const char *szPath)
{
	ASSERT(szPath!=NULL);
	unsigned int nLen=strlen(szPath);
	if(nLen==0) return 0;
	ULONGLONG Result=0;
	if(nLen==3)
	{
		ULARGE_INTEGER nFreeBytesAvailable;
		ULARGE_INTEGER nTotalNumberOfBytes;
		ULARGE_INTEGER nTotalNumberOfFreeBytes;
		if(GetDiskFreeSpaceEx(szPath,&nFreeBytesAvailable,&nTotalNumberOfBytes,&nTotalNumberOfFreeBytes))
		{
			Result=nTotalNumberOfBytes.QuadPart-nFreeBytesAvailable.QuadPart;
		}
	}
	return 0;
}

ULONGLONG GetDirFileCount(CString strPath)
{
	ULONGLONG count=0;
	CFileFind finder;
	BOOL working=finder.FindFile(strPath+"\\*.*");
	while(working)
	{
		working=finder.FindNextFileA();
		if(finder.IsDots())
			continue;
		if(!finder.IsDirectory())
			count++;
	}
	return count;
}

double ReadFreeSpace(CString strPath)
{
	HANDLE hFile=NULL;
	char szVolumeFile[10];
	double dRet=-1.0;
	DWORD dwSectPerClust,dwBytePerSect,dwNumberOfFreeClusters;
	sprintf_s(szVolumeFile,"%s:\\",strPath.Left(1));
	if(!GetDiskFreeSpace(szVolumeFile,&dwSectPerClust,&dwBytePerSect,&dwNumberOfFreeClusters,NULL))
	{
		dRet=-1.0;
	}
	else
	{
		dRet=(DWORD64)dwSectPerClust*(DWORD64)dwBytePerSect*(DWORD64)dwNumberOfFreeClusters/1024.0/1024.0/1024.0;
		//sprintf(szVolumeFile,"free size:%.02f",dRet);
	}
	return dRet;
}

LRESULT CGigeCameraDemoDlg::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	static BYTE RBufOne[25]={0};
	static int RPosOne=0;
	static BYTE RBufTwo[50]={0};
	static int RPosTwo=0;
	UINT16 revint16[4];
	UINT32 rint32[8];
	int iTemp;
	m_RevFlag=true;
	switch((UINT)lParam)
	{
	case 1:
		RBufOne[RPosOne++]=(UINT)wParam;
		if(RPosOne>12){
			for(int i=3,j=0;i<11;j++,i+=2)
				revint16[j] =RBufOne[i] * 256+ RBufOne[i + 1];
			iTemp=(int)(revint16[0]/100.0);
			m_dbVoltage_1=iTemp/10.0;
			//iTemp=(int)(revint16[1]/100.0);
			//pThis->m_dbSpeed =iTemp/10.0;
			iTemp=(int)(revint16[1]/100.0);
			m_dbVoltage_2 =iTemp/10.0;
			RPosOne=0;
		}
		break;
	case 2:
		RBufTwo[RPosTwo++]=(UINT)wParam;
		//if(RPosTwo>21){
		//	for(int m=3,n=0;m<19;n++,m+=2)
		//		rint16[n] =RBufTwo[m] * 256+ RBufTwo[m + 1];
		//	iTemp=(int)(rint16[4]+rint16[5]);
		//	m_dbSpeed =iTemp/1.0;
		//	RPosTwo=0;
		//}
		if(RPosTwo>36){
			for(int m=3,n=0;m<35;n++,m+=4)
				rint32[n] =(RBufTwo[m]<<24)+(RBufTwo[m+1]<<16)+(RBufTwo[m+2]<<8)+RBufTwo[m+3];
			m_dbSpeed=rint32[5]/100.0*60.0*m_XiShu[1]/1000.0;
			//m_dbCurrMileage=rint32[0]*0.3/1024.0/1000.0;
			m_dbCurrMileage=m_dbLastMileage+rint32[0]*m_XiShu[3]/1000.0;//0.3/1024
			//iTemp=(int)(rint32[4]+rint32[5]);
			//m_dbSpeed =iTemp/1.0;
			RPosTwo=0;
		}
		break;
	}
	return 0;
}

//LRESULT CGigeCameraDemoDlg::OnReceiveData(WPARAM wParam, LPARAM lParam)
//{
//	m_RevFlag=true;
//
//	m_strReceive.AppendChar((UINT)wParam);
//	if(m_strReceive.GetAt(0)=='#'&&m_strReceive.GetAt(m_strReceive.GetLength()-1)=='$')
//	{
//		CString strMsg=m_strReceive.Mid(1,m_strReceive.GetLength()-2);
//		m_strReceive="";
//		if(strMsg.Find("Remote Close")>-1)
//		{
//			EndDialog(TRUE);
//			return 0;
//		}
//		CString strRec;
//		SYSTEMTIME time;
//		::GetLocalTime(&time);
//
//		strRec.Format("%d-%02d-%02d %02d:%02d:%02d.%03d Num=%ld Info=%s",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_1,strMsg);
//		//str.Format(_T("Frames acquired in trash buffer: %s||"), (LPSTR)wParam);
//		m_statusWnd.SetWindowText(strRec);
//		CString str;
//		str.Format("%s\\%s\\Record.rec",m_strStoragePath.GetBuffer(),m_pTimeFileName);
//		ZLBCreateDirectory(str.GetBuffer());
//		CStdioFile Rec;
//		Rec.Open(str,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
//		Rec.SeekToEnd();
//		Rec.WriteString(strRec+"\r\n");
//		Rec.Close();
//		
//	}
//	m_RevFlag=false;
//	
//
//	return 0;
//}

void us_Delay(ULONG ulMicroSeconds)  
{ 
    LARGE_INTEGER  timeStop;   
    LARGE_INTEGER  timeStart;   
    LARGE_INTEGER  Freq;   
    LONGLONG  ulTimeToWait;  
    MSG msg;
    if(!QueryPerformanceFrequency( &Freq ))  
        return;  
  
    QueryPerformanceCounter ( &timeStart );  
  
    ulTimeToWait = Freq.QuadPart * ulMicroSeconds / 1000 / 1000;   
      
    timeStop = timeStart;   
      
    while((timeStop.QuadPart - timeStart.QuadPart) < ulTimeToWait )   
    {   
        QueryPerformanceCounter( &timeStop );  
		//if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		//{
		//	TranslateMessage(&msg);
		//	DispatchMessage(&msg);
		//}         
    }  
}  


BOOL CheckMessageQueue() 
    { 
     MSG msg; 
     
     while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){ 
     if(msg.message==WM_QUIT) 
     return FALSE; 
     TranslateMessage(&msg); 
     DispatchMessage(&msg); 
     } 
     return TRUE; 
    } 

void delay_time(DWORD msecond)
{
DWORD start=0;
DWORD nowTime=0;
start=clock();
do
{
MSG msg;
if (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
{
/* 	if (msg.message ==WM_QUIT) return;
 ::TranslateMessage(&msg);
 ::DispatchMessage(&msg);*/

 if (!AfxGetApp()->PumpMessage())
 {
::PostQuitMessage(0);
return;
 }
 }
 	nowTime=clock();

} while ((nowTime-start)<msecond);
} 
//***********************************************************************************
// Initialize Demo Dialog based application
//***********************************************************************************
BOOL CGigeCameraDemoDlg::OnInitDialog()
{
	CRect rect;

	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}

		pSysMenu->EnableMenuItem(SC_MAXIMIZE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		pSysMenu->EnableMenuItem(SC_SIZE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//WriteParamFileIniFile();
	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, FALSE);	// Set small icon
	SetIcon(m_hIcon, TRUE);		// Set big icon

	//::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	// 隐藏TitleBar
	//ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
	DWORD dwStyle = GetStyle();//获取旧样式  
	DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE| WS_SYSMENU |WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;  
	dwNewStyle&=dwStyle;//按位与将旧样式加上 
	SetWindowLong(m_hWnd,GWL_STYLE,dwNewStyle);//设置成新的样式  

	//ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED); 
	//ModifyStyle(WS_CAPTION,0,0);//如果不想去掉标题栏，去掉该句。
	//SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
	//
	//SetWindowPos(GetDlgItem(IDC_VIEW_WND3),0,0, 1024,768,SWP_NOMOVE|SWP_NOSIZE);
	GetDlgItem(IDC_VIEW_WND3)->MoveWindow(0,0,1024,768);
	GetDlgItem(IDC_VIEW_WND3)->ShowWindow(SW_SHOW);
	this->ShowWindow(SW_SHOWMAXIMIZED);
	m_ImageWnd_1.ShowWindow(SW_HIDE);
	m_ImageWnd_2.ShowWindow(SW_HIDE);
	m_statusWnd.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_GRAB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FREEZE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TEST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RETURN)->ShowWindow(SW_HIDE);
	
	SetUI();
	RefreshUI ();

	m_iWindowMode=1;
	//if(m_SerialPort.InitPort(this,1,9600,'N',8,1,EV_RXCHAR|EV_CTS,512))
	//{
	//	m_SerialPort.StartMonitoring();
	//}

	if(m_SerialPortOne.InitPort(this,1,9600,'N',8,1,EV_RXCHAR|EV_CTS,512))
	{
		m_SerialPortOne.StartMonitoring();
	}
	
	if(m_SerialPortTwo.InitPort(this,2,9600,'N',8,1,EV_RXCHAR|EV_CTS,512))
	{
		m_SerialPortTwo.StartMonitoring();
	}
	CString strCommand=AfxGetApp()->m_lpCmdLine;
	BOOL bb=IsUserAdmin();

	for (int nBuff=0; nBuff<NUM_BUFF; nBuff++) {
		listImageBuff_1[nBuff].sizeBuff = 0;
		listImageBuff_1[nBuff].iImageType = 0;
		listImageBuff_1[nBuff].bIsLocked = FALSE;

		listImageBuff_2[nBuff].sizeBuff = 0;
		listImageBuff_2[nBuff].iImageType = 0;
		listImageBuff_2[nBuff].bIsLocked = FALSE;
	}

	m_iCurrentCount_1 = 0;
	m_iCurrentCount_2 = 0;
	m_iCountImage_1 = 0;
	m_iCountImage_2 = 0;
	ReadParamFromIniFile();

	SYSTEMTIME time;
	::GetLocalTime(&time);
	m_pTimeFileName = ZLBGetFormatTime(time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
	m_strLeftPath.Format("%s\\%s\\Left",m_strStoragePath.GetBuffer(),m_pTimeFileName);
	m_strRightPath.Format("%s\\%s\\Right",m_strStoragePath.GetBuffer(),m_pTimeFileName);
	m_cSavePath_1=m_strLeftPath.GetBuffer();
	m_cSavePath_2=m_strRightPath.GetBuffer();

	m_dFreeSpaceG=ReadFreeSpace(m_strStoragePath);

	CString temp;
	temp.Format("#FreeSize:%.02fGB,Ready...$\r\n",m_dFreeSpaceG);
	//temp.Format("#FreeSize:%.02fGB,Ready...$",m_dFreeSpaceG);
	//m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
	//::SendMessage(pMessageDialog->m_hWnd,WM_MSG,(WPARAM)(LPCTSTR)temp,16);
	
	if(m_iWindowMode==1)
	{
		m_strInfo=temp;
		InvalidateRect(&m_RectInfoShow);
	}

	#if _DEBUG
	//CString str;
	//if(m_pfLog_1 == NULL)
	//{	
	//	str.Format("L%d%02d%02d%02d%02d%02d%03d.log",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
	//	m_pfLog_1 = _tfopen(str,_T("wb"));
	//	//m_pfLog_1 = fopen("D:\\Leftlog.log","wb");
	//}
	//if(m_pfLog_2 == NULL)
	//{
	//	str.Format("R%d%02d%02d%02d%02d%02d%03d.log",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
	//	m_pfLog_2 = _tfopen(str,_T("wb"));
	//	//m_pfLog_2 = fopen("D:\\Leftlog.log","wb");
	//}
	#endif

	g_hThread = NULL;
	g_hThread_1 = NULL;
	g_hThread_2 = NULL;

	//CRect rect;    
    GetClientRect(&rect);     //取客户区大小  
    Old.x=rect.right-rect.left;
    Old.y=rect.bottom-rect.top;

	//NOTIFYICONDATA nid;
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this-> m_hWnd; 
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP; 
	nid.uCallbackMessage=(WM_USER+10);//自定义的消息名称 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy_s(nid.szTip, "采集任务 ");//信息提示条为"计划任务提醒" 
	Shell_NotifyIcon(NIM_ADD,&nid);//在托盘区添加图标 
	//ShowWindow(SW_HIDE);//隐藏主窗口 

	//WINDOWPLACEMENT m_wp;
	//GetWindowPlacement(&m_wp); //恢复时用
	//WINDOWPLACEMENT wp;
	//wp.length=sizeof(WINDOWPLACEMENT);
	//wp.flags=WPF_RESTORETOMAXIMIZED;
	//wp.showCmd=SW_HIDE;
	//SetWindowPlacement(&wp);

   // Select acquisition server from dialog
   // Only the servers that support "AcqDevice" resource are listed
	
	int Num=0;
	for(Num=0;Num<30;Num++)	
	{
		if (SapManager::GetServerCount()<3)
		{
			delay_time(2000);
			//Sleep(2000);
			//CheckMessageQueue();
		}
		else
		{
			CString temp="#Both sides of the camera are normal$\r\n";
			//CString temp="#Both sides of the camera are normal$";
			//m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
			if(m_iWindowMode==1)
			{
				m_strInfo=temp;
				InvalidateRect(&m_RectInfoShow);
			}
			//m_pSplashWindow->SetInfo(temp);
			break;
		}
	}
	
	if(Num>=30)
	{
		CString temp="#No camera found or lack of camera$\r\n";
		//CString temp="#No camera found or lack of camera$";
		//m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
		if(m_iWindowMode==1)
		{
			m_strInfo=temp;
			InvalidateRect(&m_RectInfoShow);
		}
		//m_pSplashWindow->SetInfo(temp);
		::MessageBox(NULL,_T("No camera found or lack of camera"),_T("Message"),MB_ICONHAND);
		EndDialog(TRUE);
		return FALSE;
	}
	if(m_iFirstFlag!=1)
	{
		//m_pSplashWindow->DeleteSplash();
		m_iFirstFlag=1;
		//m_pSplashWindow->DestroyWindow();
		//RefreshUI();
	}
		SetTimer(3,1000,NULL);



#if 1
	// Define objects
	//if (SapManager::GetServerCount()<3)
 //   {
 //      MessageBox(_T("No cameras found or lack of cameras"));
 //      EndDialog(TRUE);
 //      return FALSE;
 //   }
	
	//SapLocation.
	char s[1024];

		//SapManager::GetServerName(1,s);
	//SapManager::GetResourceIndex("Linea_M2048-7um_1",SapManager::ResourceAcqDevice,"12080127");
	// Define objects 1

	struct DeviceList
	{
		int Index;
		char SN[128];
	};
	DeviceList dl[3];
	//char SN1[128],SN2[128];
	//int SIndex1,SIndex2;

	int iServerCount=SapManager::GetServerCount();
	SapManager::DetectAllServers(SapManager::DetectServerAll);
	for(int i=0;i<iServerCount;i++)
	{
		int iAcqDeviceNum=SapManager::GetResourceCount(i,SapManager::ResourceAcqDevice);
		int iAcqNum=SapManager::GetResourceCount(i,SapManager::ResourceAcq);
		if(iAcqDeviceNum>0||iAcqNum>0)
		{
			char ServerName[128];
			char DeviceName[128];
			memset(ServerName,0x00,128);
			//SapManager::GetServerName(i,ServerName,sizeof(ServerName));
			//SapManager::GetServerSerialNumber(i,ServerName);
			SapManager::GetServerSerialNumber(i,dl[i].SN);
			dl[i].Index=i;
		}
	}
	if(strcmp(dl[1].SN,dl[2].SN)>0)
	{
		m_AcqDevice_1	= new SapAcqDevice(SapLocation(dl[2].Index,0),m_configFileName_1);
		m_AcqDevice_2	= new SapAcqDevice(SapLocation(dl[1].Index,0),m_configFileName_2);
	}
	else
	{
		
		m_AcqDevice_1	= new SapAcqDevice(SapLocation(dl[1].Index,0),m_configFileName_1);
		m_AcqDevice_2	= new SapAcqDevice(SapLocation(dl[2].Index,0),m_configFileName_2);
	}
	//m_AcqDevice_1	= new SapAcqDevice(SapLocation(m_serverName_1,m_resourceIndex_1),m_configFileName_1);
	m_Buffers_1	= new SapBufferWithTrash(2, m_AcqDevice_1);
	//m_Bayer     = new SapColorConversion(m_Buffers_1);
	m_Xfer_1		= new SapAcqDeviceToBuf(m_AcqDevice_1, m_Buffers_1, XferCallback_1, this);
	m_View_1      = new SapView(m_Buffers_1);

   // Attach sapview to image viewer 1
   m_ImageWnd_1.AttachSapView(m_View_1);

   if (!CreateObjects_1()) { EndDialog(TRUE); return FALSE; }

   	// Define objects 2
	//m_AcqDevice_2	= new SapAcqDevice(SapLocation(m_serverName_2,m_resourceIndex_2),m_configFileName_2);

	m_Buffers_2	= new SapBufferWithTrash(4, m_AcqDevice_2);

	//m_Bayer     = new SapColorConversion(m_Buffers_2);
	m_Xfer_2		= new SapAcqDeviceToBuf(m_AcqDevice_2, m_Buffers_2, XferCallback_2, this);
	m_View_2      = new SapView(m_Buffers_2);

   // Attach sapview to image viewer 2
   m_ImageWnd_2.AttachSapView(m_View_2);

	// Create all objects
	if (!CreateObjects_2()) { EndDialog(TRUE); return FALSE; }

   //m_ImageWnd_1.AttachEventHandler(this);
   //m_ImageWnd_1.CenterImage();
   m_ImageWnd_1.Reset();

   //m_ImageWnd_2.AttachEventHandler(this);
   //m_ImageWnd_2.CenterImage();
   m_ImageWnd_2.Reset();

	FitToWindow(m_View_1);
	FitToWindow(m_View_2);
#endif

   if(strCommand.IsEmpty())
   {
		DWORD dwStart = GetTickCount();
		 DWORD dwEnd   = dwStart;
		 do
		 {
		 dwEnd = GetTickCount() - dwStart;
		 }while(dwEnd <500);
 

		OnGrab();
   }
   else
   {
	   //MessageBox("F");
   }
   UpdateMenu();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CGigeCameraDemoDlg::IsUserAdmin()
{
	//BOOL b;
	//SID_IDENTIFIER_AUTHORITY NtAuthority=SECURITY_NT_AUTHORITY;
	//PSID AdministratorsGroup;
	//b=AllocateAndInitializeSid(&NtAuthority,2,SECURITY_BUILTIN_DOMAIN_RID,DOMAIN_ALIAS_RID_ADMINS,0,0,0,0,0,0,&AdministratorsGroup);
	//if(b)
	//{
	//	if(!CheckTokenMembership(NULL,AdministratorsGroup,&b))
	//	{
	//		b=FALSE;
	//	}
	//	FreeSid(AdministratorsGroup);
	//}
	//return(b);
	BOOL b;
	SC_HANDLE schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE); 
	if (schSCManager == NULL) 
	{
		//非管理员
		b=FALSE;
	}
	else
		b=TRUE;
	return(b);
}
BOOL CGigeCameraDemoDlg::CreateObjects_1()
{
	CWaitCursor wait;

	// Create acquisition object
	if (m_AcqDevice_1 && !*m_AcqDevice_1 && !m_AcqDevice_1->Create())
   {
      DestroyObjects_1();
      return FALSE;
   }

	// Create buffer object
	if ((m_Buffers_1 && !*m_Buffers_1))
	{
		if( !m_Buffers_1->Create())
		{
			DestroyObjects_1();
			return FALSE;
		}
		// Clear all buffers
		m_Buffers_1->Clear();
		
	}
	//if(m_iBayerEnable == 1)
	//{
	//	bool bSet = m_Bayer->SetOutputFormat(SapFormatRGB888);
	//	if(bSet == FALSE)
	//		return false;

	//	bSet = m_Bayer->Enable(1, 0);
	//	if(bSet == FALSE)
	//		return false;
	//	
	//	/*创建贝尔滤波*/
	//	if(!m_Bayer->Create())
	//	{
	//		return false;
	//	}

	//	SapBuffer* bayerBuffer = m_Bayer->GetOutputBuffer();
 //       if (bayerBuffer && *bayerBuffer)
	//		m_View_1->SetBuffer(bayerBuffer);
 //       else
	//		m_View_1->SetBuffer(m_Buffers_1);

	//	// Set alignment 
	//	m_Bayer->SetAlign(m_align[m_iBayerType]);

	//	// Set interpolation method
	//	m_Bayer->SetMethod(SapBayer::Method1);
	//}

	// Create view object
	if (m_View_1 && !*m_View_1 && !m_View_1->Create())
   {
      DestroyObjects_1();
      return FALSE;
   }

   // Set next empty with trash cycle mode for transfer
   if (m_Xfer_1 && m_Xfer_1->GetPair(0))
   {
	   //if (!m_Xfer_1->GetPair(0)->SetCycleMode(SapXferPair::EventEndOfFrame))
	  if (!m_Xfer_1->GetPair(0)->SetCycleMode(SapXferPair::CycleNextWithTrash))
      {
         DestroyObjects_1();
         return FALSE;
      }
   }

	// Create transfer object
	if (m_Xfer_1 && !*m_Xfer_1 && !m_Xfer_1->Create())
   {
      DestroyObjects_1();
      return FALSE;
   }

	return TRUE;
}

BOOL CGigeCameraDemoDlg::CreateObjects_2()
{
	CWaitCursor wait;

	// Create acquisition object
	if (m_AcqDevice_2 && !*m_AcqDevice_2 && !m_AcqDevice_2->Create())
   {
      DestroyObjects_2();
      return FALSE;
   }
	//// Create buffer object
	//if ((m_Buffers_1 && !*m_Buffers_1)&&(m_Buffers_2 && !*m_Buffers_2))
	//{
	//	if( !m_Buffers_1->Create()&&!m_Buffers_2->Create())
	//	{
	//		DestroyObjects();
	//		return FALSE;
	//	}
	//	// Clear all buffers
	//	m_Buffers_1->Clear();
	//	m_Buffers_2->Clear();
	//}
	// Create buffer object
	if (m_Buffers_2 && !*m_Buffers_2)
	{
		if(!m_Buffers_2->Create())
		{
			DestroyObjects_2();
			return FALSE;
		}
		// Clear all buffers
		
		m_Buffers_2->Clear();
	}
	//if(m_iBayerEnable == 1)
	//{
	//	bool bSet = m_Bayer->SetOutputFormat(SapFormatRGB888);
	//	if(bSet == FALSE)
	//		return false;

	//	bSet = m_Bayer->Enable(1, 0);
	//	if(bSet == FALSE)
	//		return false;
	//	
	//	/*创建贝尔滤波*/
	//	if(!m_Bayer->Create())
	//	{
	//		return false;
	//	}

	//	SapBuffer* bayerBuffer = m_Bayer->GetOutputBuffer();
 //       if (bayerBuffer && *bayerBuffer)
	//		m_View_1->SetBuffer(bayerBuffer);
 //       else
	//		m_View_1->SetBuffer(m_Buffers_1);

	//	// Set alignment 
	//	m_Bayer->SetAlign(m_align[m_iBayerType]);

	//	// Set interpolation method
	//	m_Bayer->SetMethod(SapBayer::Method1);
	//}

	// Create view object
	if (m_View_2 && !*m_View_2 && !m_View_2->Create())
   {
      DestroyObjects_2();
      return FALSE;
   }

   // Set next empty with trash cycle mode for transfer
   if (m_Xfer_2 && m_Xfer_2->GetPair(0))
   {
	  //if (!m_Xfer_2->GetPair(0)->SetCycleMode(SapXferPair::EventEndOfFrame))
	   if (!m_Xfer_2->GetPair(0)->SetCycleMode(SapXferPair::CycleNextWithTrash))
      {
         DestroyObjects_2();
         return FALSE;
      }
   }

	// Create transfer object
	if (m_Xfer_2 && !*m_Xfer_2 && !m_Xfer_2->Create())
   {
      DestroyObjects_2();
      return FALSE;
   }

	return TRUE;
}

BOOL CGigeCameraDemoDlg::DestroyObjects_1()
{
	// Destroy transfer object
	if (m_Xfer_1 && *m_Xfer_1) m_Xfer_1->Destroy();

	// Destroy view object
	if (m_View_1 && *m_View_1) m_View_1->Destroy();

	//if(m_iBayerEnable == 1)
	//{
	//	if (m_Bayer && *m_Bayer) m_Bayer->Destroy();
	//}

	// Destroy buffer object
	if (m_Buffers_1 && *m_Buffers_1) m_Buffers_1->Destroy();

	// Destroy acquisition object
	if (m_AcqDevice_1 && *m_AcqDevice_1) m_AcqDevice_1->Destroy();

	return TRUE;
}

BOOL CGigeCameraDemoDlg::DestroyObjects_2()
{
	// Destroy transfer object
	if (m_Xfer_2 && *m_Xfer_2) m_Xfer_2->Destroy();

	// Destroy view object
	if (m_View_2 && *m_View_2) m_View_2->Destroy();

	//if(m_iBayerEnable == 1)
	//{
	//	if (m_Bayer && *m_Bayer) m_Bayer->Destroy();
	//}

	// Destroy buffer object
	if (m_Buffers_2 && *m_Buffers_2) m_Buffers_2->Destroy();

	// Destroy acquisition object
	if (m_AcqDevice_2 && *m_AcqDevice_2) m_AcqDevice_2->Destroy();

	return TRUE;
}

//**********************************************************************************
//
//				Window related functions
//
//**********************************************************************************
void CGigeCameraDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(( nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CGigeCameraDemoDlg::OnPaint() 
{
	if( IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT32 cxIcon = GetSystemMetrics(SM_CXICON);
		INT32 cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT32 x = (rect.Width() - cxIcon + 1) / 2;
		INT32 y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	//if(m_iFirstFlag!=1)
	//{
		//m_pSplashWindow->DeleteSplash();
		m_iFirstFlag=1;
		RefreshUI();
	//}
}

void CGigeCameraDemoDlg::OnDestroy() 
{
	//delete pMessageDialog;  

	CDialog::OnDestroy();
	Shell_NotifyIcon(NIM_DELETE,&nid);//在托盘区添加图标 
	// Destroy all objects
	DestroyObjects_1();
	DestroyObjects_2();
	// Delete all objects
   if (m_View_1)			delete m_View_1;
	if (m_Xfer_1)			delete m_Xfer_1;
	if(m_Bayer)         delete m_Bayer;
	if (m_Buffers_1)		delete m_Buffers_1;
	if (m_AcqDevice_1)	delete m_AcqDevice_1;

	if (m_View_2)			delete m_View_2;
	if (m_Xfer_2)			delete m_Xfer_2;
	//if(m_Bayer)         delete m_Bayer;
	if (m_Buffers_2)		delete m_Buffers_2;
	if (m_AcqDevice_2)	delete m_AcqDevice_2;

	if(m_pfLog_1)
	{
		fclose(m_pfLog_1);
		m_pfLog_1 = NULL;
	}	
	if(m_pfLog_2)
	{
		fclose(m_pfLog_2);
		m_pfLog_2 = NULL;
	}	
	CString temp="#Finish...$\r\n";
	//CString temp="#Finish...$";
	//m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
	if(m_iWindowMode==1)
	{
		m_strInfo=temp;
		InvalidateRect(&m_RectInfoShow);
		//m_pSplashWindow->SetInfo(temp);
	}
}


void CGigeCameraDemoDlg::OnSize(UINT nType, int cx, int cy) 
{
   CDialog::OnSize(nType, cx, cy); 
   
   if(nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)
    {
       float fsp[2];
    POINT Newp; //获取现在对话框的大小
    CRect recta;    
    GetClientRect(&recta);     //取客户区大小  
    Newp.x=recta.right-recta.left;
    Newp.y=recta.bottom-recta.top;
    fsp[0]=(float)Newp.x/Old.x;
    fsp[1]=(float)Newp.y/Old.y;
    CRect Rect;
    int woc;
    CPoint OldTLPoint,TLPoint; //左上角
    CPoint OldBRPoint,BRPoint; //右下角
    HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件  
    while(hwndChild)    
    {    
        woc=::GetDlgCtrlID(hwndChild);//取得ID
        GetDlgItem(woc)->GetWindowRect(Rect);  
        ScreenToClient(Rect);  
        OldTLPoint = Rect.TopLeft();  
        TLPoint.x = long(OldTLPoint.x*fsp[0]);  
        TLPoint.y = long(OldTLPoint.y*fsp[1]);  
        OldBRPoint = Rect.BottomRight();  
        BRPoint.x = long(OldBRPoint.x *fsp[0]);  
        BRPoint.y = long(OldBRPoint.y *fsp[1]);  
        Rect.SetRect(TLPoint,BRPoint);  
        GetDlgItem(woc)->MoveWindow(Rect,TRUE);
        hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);    
    }
    Old=Newp;

    }
   else if(nType==SIZE_MINIMIZED)
   {
	 //  NOTIFYICONDATA nid;
	 //  nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
		//nid.hWnd=this-> m_hWnd; 
		//nid.uID=IDR_MAINFRAME; 
		//nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP   ; 
		//nid.uCallbackMessage=(WM_USER+10);//自定义的消息名称 
		//nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
		//strcpy(nid.szTip, "计划任务提醒 ");//信息提示条为"计划任务提醒" 
		//Shell_NotifyIcon(NIM_ADD,&nid);//在托盘区添加图标 
	    Shell_NotifyIcon(NIM_MODIFY,&nid);//在托盘区添加图标 
		ShowWindow(SW_HIDE);//隐藏主窗口 
   }

   //CRect rClient;
   //GetClientRect(rClient);
   //
   //// resize image viewer
   //if (m_ImageWnd_1.GetSafeHwnd())
   //{
   //   CRect rWnd; 
   //   m_ImageWnd_1.GetWindowRect(rWnd);
   //   ScreenToClient(rWnd);
   //   rWnd.right = rClient.right - 5;
   //   rWnd.bottom = rClient.bottom - 5;
   //   m_ImageWnd_1.MoveWindow(rWnd);
   //}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGigeCameraDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGigeCameraDemoDlg::OnEndSession(BOOL bEnding)
{
   CDialog::OnEndSession(bEnding);

   if( bEnding)
   {
      // If ending the session, free the resources.
      OnDestroy(); 
   }
}

BOOL CGigeCameraDemoDlg::OnQueryEndSession()
{
   if (!CDialog::OnQueryEndSession())
      return FALSE;

   return TRUE;
}

//**************************************************************************************
// Updates the menu items enabling/disabling the proper items depending on the state
//  of the application
//**************************************************************************************
void CGigeCameraDemoDlg::UpdateMenu( void)
{
	BOOL bAcqNoGrab	= m_Xfer_1 && *m_Xfer_1 && !m_Xfer_1->IsGrabbing();
	BOOL bAcqGrab		= m_Xfer_1 && *m_Xfer_1 && m_Xfer_1->IsGrabbing();
	BOOL bNoGrab		= !m_Xfer_1 || !m_Xfer_1->IsGrabbing();

	// Acquisition Control
	GetDlgItem(IDC_GRAB)->EnableWindow(bAcqNoGrab);
	//GetDlgItem(IDC_SNAP)->EnableWindow(bAcqNoGrab);
	GetDlgItem(IDC_FREEZE)->EnableWindow(bAcqGrab);

	// Acquisition Options
	//GetDlgItem(IDC_LOAD_ACQ_CONFIG)->EnableWindow(m_Xfer_1 && !m_Xfer_1->IsGrabbing());

	// General Options
	//GetDlgItem(IDC_BUFFER_OPTIONS)->EnableWindow(bNoGrab);

	// If last control was disabled, set default focus
	//if (!GetFocus())
		//GetDlgItem(IDC_EXIT)->SetFocus();
}

//*****************************************************************************************
//
//					General Options
//
//*****************************************************************************************
bool DoEvents()
{
 MSG msg;
 while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
 {
	 //fprintf(m_pfLog_2,"message:%d wParam:%d lParam:%d\r\n",msg.message,msg.wParam,msg.lParam); 

  if (msg.message == WM_COMMAND&&msg.wParam ==32924 )
  {
	     return false;
  }

  if (!AfxGetApp()->PreTranslateMessage(&msg))
  {
   ::TranslateMessage(&msg);
   ::DispatchMessage(&msg);
  }
 }
 return true;
}
void CGigeCameraDemoDlg::OnTest() 
{
	CViewDlg dlg1(this, m_View_1);
	if( dlg1.DoModal() == IDOK)
      m_ImageWnd_1.Refresh();
		CViewDlg dlg2(this, m_View_2);
	if( dlg2.DoModal() == IDOK)
      m_ImageWnd_2.Refresh();

	//CString temp="#200.20#";
	//m_SerialPort.WriteToPort(temp.GetBuffer(temp.GetLength()),temp.GetLength());
	//m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
	//m_bTimerFlag=false;
	//SetTimer(2,60000,NULL);
	//20K行频30分钟也就是20000/2048*30*60=17578.125
	//DWORD dt;
	//DWORD md;
	//CString temp;
	//SYSTEMTIME time;
	//CInputDialog dlg;
	//INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: Place code here to handle when the dialog is
	//	//  dismissed with OK
	//	
	//	md=dlg.m_DelayTime;
	//	dt=(1000*60)*md;//5分钟
	//	::GetLocalTime(&time);	

	//	//temp.Format("#%d-%02d-%02d %02d:%02d:%02d.%03d Test %d Minute,20K Line Frequency 20002.134/2048*60*%d=%f$\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,int(dt/(1000*60)),int(dt/(1000*60)),double(20002.134/2048*60*(dt/(1000*60))));
	//	temp.Format("#%02d:%02d:%02d.%03d Test %d Minute,20K Line Frequency 20002.134/2048*60*%d=%f$\r\n",time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,int(dt/(1000*60)),int(dt/(1000*60)),double(20002.134/2048*60*(dt/(1000*60))));
	//	m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
	//	Sleep(1);
	//	OnGrab();
	//	DWORD dw = ::GetTickCount();
	//	while( (::GetTickCount()- dw ) <= dt) //1毫秒
	//	{
	//		//do something
	//		if(!DoEvents())
	//			break;
	//	} 
	//	OnFreeze();
	//	::GetLocalTime(&time);
	//	//temp.Format("#%d-%02d-%02d %02d:%02d:%02d.%03d Finish TLeft:%ld TRight:%ld$\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_1,m_iCountImage_2);;
	//	temp.Format("#%02d:%02d:%02d.%03d Finish TLeft:%ld TRight:%ld$\r\n",time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_1,m_iCountImage_2);
	//	m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
	//}
	//CTime t1=CTime::GetCurrentTime();
	//CTime t2;
	//CTimeSpan tspan;
	//OnGrab();
	//do
	//{
	//	MSG msg;
	//	GetMessage(&msg,NULL,0,0);
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//	t2=CTime::GetCurrentTime();
	//	tspan=t2-t1;
	//}while(tspan.GetTotalMinutes()<1);
	//OnFreeze();
}
//*****************************************************************************************
//
//					File Options
//
//*****************************************************************************************

bool CGigeCameraDemoDlg::ReadParamFromIniFile()
{
	 //获取配置文件所在目录
	int iCameraCount=0;
	char chPath[512];
	char *pImageSavePath = NULL;
	char *pRecordSavePath = NULL;
	char *pConfigPath = NULL;
	char* pString = NULL;
	CString strIni;
    bool bExist = false;
	int  iValue = -1;
	char *pTmp = NULL;
	strIni.Format("%s","Config.ini");

	//pConfigPath = ZLBGetConfigPath();
	//ZLBCreateDirectory(pConfigPath);
	pConfigPath =ZLBGetCurrentPath();

	memset(chPath,0x00,512);
	sprintf_s(chPath,"%s\\%s",pConfigPath,strIni.GetBuffer(0));
	m_iniHandle = ZLBIniFile(chPath);
    
	//读取基本配置键值
	m_iAutoStorage = ZLBIniReadInt("Basic","AutoStorage",0,m_iniHandle);
	pTmp= ZLBIniReadString("Basic","StoragePath","D:\\Image",m_iniHandle);
	m_strStoragePath.Format("%s",pTmp);
	m_iStorageFormat= ZLBIniReadInt("Basic","StorageFormat",0,m_iniHandle);
	m_iPixelFormat= ZLBIniReadInt("Basic","PixelFormat",0,m_iniHandle);
	m_iImageColor= ZLBIniReadInt("Basic","ImageColor",1,m_iniHandle);
	m_strStorageFormat=(m_iStorageFormat==0?"bmp":"jpg");

	//m_XiShu[0]= ZLBIniReadDouble("Basic","VoltageParam_1",0.0,m_iniHandle);
	m_XiShu[1]= ZLBIniReadDouble("Basic","SpeedParam",1.0,m_iniHandle);
	//m_XiShu[2]= ZLBIniReadDouble("Basic","VoltageParam_2",0.0,m_iniHandle);
	m_XiShu[3]= ZLBIniReadDouble("Basic","MileageParam",1.0,m_iniHandle);//X*0.3/1024
	m_dbLastMileage= ZLBIniReadDouble("Basic","LastMileage",1.0,m_iniHandle);//X*0.3/1024
	//判断目录是否存在,如果不存在就创建目录
	//CString strDirvePath=m_strStoragePath.Left(2);
	//GetDriveType(strDirvePath);
	bool bb=ZLBIsPathExist(m_strStoragePath.Left(2).GetBuffer());
	if(bb)
	{
		ZLBCreateDirectory(m_strStoragePath.GetBuffer());
	}
	else
	{
		CString temp;
		temp.Format("#The drive does not exist,Reboot...$\r\n");
		//m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
		//m_pSplashWindow->SetInfo(temp);
		this->MessageBox("The drive does not exist,Reboot...","error");
		exit(0);
	}
	//读取相机配置键值
	//iCameraCount = ZLBIniReadInt("Camera","CameraCount",0,m_iniHandle);
	pTmp= ZLBIniReadString("Camera","ServerItem01","Linea_M2048-7um_1",m_iniHandle);
	m_serverName_1.Format("%s",pTmp);
	pTmp= ZLBIniReadString("Camera","FileNameItem01","T_Linea_M2048-7um_L_12080124.ccf",m_iniHandle);
	m_configFileName_1.Format("%s",pTmp);
	m_resourceIndex_1= ZLBIniReadInt("Camera","ResourceItem01",0,m_iniHandle);
	pTmp= ZLBIniReadString("Camera","ServerItem02","Linea_M2048-7um_1",m_iniHandle);
	m_serverName_2.Format("%s",pTmp);
	pTmp= ZLBIniReadString("Camera","FileNameItem02","T_Linea_M2048-7um_L_12080124.ccf",m_iniHandle);
	m_configFileName_2.Format("%s",pTmp);
	m_resourceIndex_2= ZLBIniReadInt("Camera","ResourceItem02",1,m_iniHandle);

	return true;
}

/*将参数配置写入到配置文件中*/
bool CGigeCameraDemoDlg::WriteParamFileIniFile()
{
	/*获取配置文件所在目录*/
	char chPath[512];
	char *pImageSavePath = NULL;
	char *pRecordSavePath = NULL;
	char *pConfigPath = NULL;
	char* pString = NULL;
	CString strIni;
    bool bExist = false;
	int  iValue = -1;
	char *pDir = NULL;
	strIni.Format("%s","Config.ini");

	pConfigPath =ZLBGetCurrentPath();
	memset(chPath,0x00,512);
	sprintf(chPath,"%s\\%s",pConfigPath,strIni.GetBuffer(0));
	m_iniHandle = ZLBIniFile(chPath);  

	ZLBIniWriteDouble("Basic","LastMileage",m_dbLastMileage,m_iniHandle);
	//ZLBIniWriteInt("相机配置","是否开启存储图像(1-开启 0-关闭)",m_iStartImage,m_iniHandle);
	//ZLBIniWriteInt("相机配置","存储图像总的张数",m_iSaveImageCount,m_iniHandle);
	//ZLBIniWriteString("相机配置","存储图像的目录",m_strStoragePath.GetBuffer(0),m_iniHandle);
	//ZLBIniWriteInt("相机配置","存储图像采集格式0-BMP 1-JPEG",m_iStorageFormat,m_iniHandle);
	//ZLBIniWriteInt("相机配置","采集数据格式Mono8 0-8bit 1-10bit",m_iPixelFormat,m_iniHandle);
	//ZLBIniWriteInt("相机配置","图像颜色1-Grey 3-Color",m_iImageColor,m_iniHandle);

	return true;
}

void CGigeCameraDemoDlg::OnMainitemShow()
{
	// TODO: 在此添加命令处理程序代码
	//SIZE_RESTORED
	//b_show=true;
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	ShowWindow(SW_SHOW||SW_RESTORE);//简单的显示主窗口完事儿 
	
}


void CGigeCameraDemoDlg::OnMainitemHide()
{
	// TODO: 在此添加命令处理程序代码
	//b_show=false;
	ShowWindow(SW_HIDE);

}


void CGigeCameraDemoDlg::OnMainitemExit()
{
	// TODO: 在此添加命令处理程序代码
	EndDialog(TRUE);
}

//*****************************************************************************************
//
//					Acquisition Control
//
//*****************************************************************************************

void CGigeCameraDemoDlg::FitToWindow(SapView *pView)
{

	int viewWidth;
	int viewHeight;

	
	BOOL	m_bLockAspectRatio=false;
	DWORD	scaleHeight;
	DWORD	scaleWidth;
	float	scaleHeightFactor;
	float	scaleWidthFactor;

	pView->GetViewArea( &viewWidth, &viewHeight);

	scaleWidthFactor = 100.0f * viewWidth/pView->GetBuffer()->GetWidth();
	scaleHeightFactor= 100.0f * viewHeight/pView->GetBuffer()->GetHeight();

	if( m_bLockAspectRatio)
	{
		if( scaleWidthFactor < scaleHeightFactor)
		{
			scaleHeightFactor= scaleWidthFactor;
		}
		else
		{
			scaleWidthFactor= scaleHeightFactor;
		}
	}

	scaleWidth = (DWORD)floor(pView->GetBuffer()->GetWidth()  * scaleWidthFactor/100 + 0.5f);
	scaleHeight= (DWORD)floor(pView->GetBuffer()->GetHeight() * scaleHeightFactor/100 + 0.5f);

	scaleWidthFactor = 100.0f * scaleWidth / pView->GetBuffer()->GetWidth();
	scaleHeightFactor= 100.0f * scaleHeight / pView->GetBuffer()->GetHeight();
	scaleWidthFactor=23.584;
	scaleHeightFactor=25.5859;
	pView->SetScalingMode( scaleWidthFactor/100.0f, scaleHeightFactor/100.0f);
	UpdateData( FALSE);

}
void CGigeCameraDemoDlg::OnGrab()
{
	// TODO: 在此添加控件通知处理程序代码
	   DWORD	word;
	   DWORD	word_1;
	   DWORD	word_2;
	   SYSTEM_INFO systeminfo;
	   ::GetSystemInfo(&systeminfo);
	   int n=systeminfo.dwNumberOfProcessors;
	m_statusWnd.SetWindowText(_T(""));

	SetTimer(1,100,NULL);
	 
	for (int nBuff=0; nBuff<NUM_BUFF; nBuff++) {
		listImageBuff_1[nBuff].sizeBuff = 0;
		listImageBuff_1[nBuff].iImageType = 0;
		listImageBuff_1[nBuff].bIsLocked = FALSE;

		listImageBuff_2[nBuff].sizeBuff = 0;
		listImageBuff_2[nBuff].iImageType = 0;
		listImageBuff_2[nBuff].bIsLocked = FALSE;
	}
    m_iCountImage_1 = 0;
	m_iCountImage_2 = 0;
	m_iCurrentCount_1 = 0;
	m_iCurrentCount_2 = 0;
	listImageWriteIndex_1=-1;
	listImageReadIndex_1=-1;
	listImageWriteIndex_2=-1;
	listImageReadIndex_2=-1;

	//g_hThread = NULL;
	if(g_hThread_1!=NULL)
	{
		CloseHandle(g_hThread_1);
	}
	if(g_hThread_2!=NULL)
	{
		CloseHandle(g_hThread_2);
	}
	g_hThread_1 = NULL;
	g_hThread_2 = NULL;

	//SYSTEMTIME time;
	//::GetLocalTime(&time);
	//m_pTimeFileName = ZLBGetFormatTime(time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
	//m_strLeftPath.Format("%s\\%s\\Left",m_strStoragePath.GetBuffer(),m_pTimeFileName);
	//m_strRightPath.Format("%s\\%s\\Right",m_strStoragePath.GetBuffer(),m_pTimeFileName);
	//m_cSavePath_1=m_strLeftPath.GetBuffer();
	//m_cSavePath_2=m_strRightPath.GetBuffer();
	m_bDirExit_1=false;
	m_bDirExit_2=false;

	//m_dFreeSpaceG=ReadFreeSpace(m_strStoragePath);

	CString temp;
	//temp.Format("#FreeSize:%.02fGB,Start...$\r\n",m_dFreeSpaceG);
	temp.Format("#Start...$\r\n");
	//temp.Format("#Start...$");
	//m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
	if(m_iWindowMode==1)
	{
		m_strInfo=temp;
		InvalidateRect(&m_RectInfoShow);
	}

	//CString temp;
	//temp.Format("#FreeSize:%.02fGB,Ready...$\r\n",m_dFreeSpaceG);
	//m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());

		//g_hThread      = CreateThread(0,NULL,SaveImageData,this,0,&word);

   	//g_hThread_1      = CreateThread(0,NULL,SaveImageData_1,this,0,&word_1);
	//::SetThreadAffinityMask(g_hThread_2,3);
	//g_hThread_2      = CreateThread(0,NULL,SaveImageData_2,this,0,&word_2);
	//::SetThreadAffinityMask(g_hThread_2,4);

	if( m_Xfer_1->Grab()&&m_Xfer_2->Grab())
	{
		UpdateMenu();	
	}
	//if( m_Xfer_1->Grab())
	//{
	//	UpdateMenu();	
	//}
}


void CGigeCameraDemoDlg::OnFreeze()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_Xfer_1->Freeze()&&m_Xfer_2->Freeze())
	{
		if (CAbortDlg(this, m_Xfer_1).DoModal() != IDOK||CAbortDlg(this, m_Xfer_2).DoModal() != IDOK) 
		{
			m_Xfer_1->Abort();
			m_Xfer_2->Abort();
		}
		UpdateMenu();
	}

	//	if( m_Xfer_1->Freeze())
	//{
	//	if (CAbortDlg(this, m_Xfer_1).DoModal() != IDOK) 
	//	{
	//		m_Xfer_1->Abort();
	//	}
	//	UpdateMenu();
	//}
}


//LRESULT CGigeCameraDemoDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	if(message==WM_NCPAINT)
//	{
//		if(b_show==false)
//			ShowWindow(SW_HIDE);
//		else
//			ShowWindow(SW_SHOW);
//	}
//	return __super::DefWindowProc(message, wParam, lParam);
//}


void CGigeCameraDemoDlg::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnNcPaint()
	//static int i_first_show=1;
	//if(i_first_show>0)
	//{
	//	i_first_show--;
	//	ShowWindow(SW_HIDE);
	//}
	//else
		CDialog::OnNcPaint();
}


void CGigeCameraDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString temp;
	SYSTEMTIME time;
	ULONGLONG left,right;
	if(nIDEvent==3)
	{
		if(m_iWindowMode==1)
		{
			InvalidateRect(&m_RectDateTimeShow);
			InvalidateRect(&m_RectMeterShow);
			InvalidateRect(&m_RectInfoShow);
			InvalidateRect(&m_RectBatteryShow_1);
			InvalidateRect(&m_RectBatteryShow_2);
			InvalidateRect(&m_RectElectricShow_1);
			InvalidateRect(&m_RectElectricShow_2);
			InvalidateRect(&m_RectSpeedShow);
		}
	
		byte sendvoltage[] = {0x01,0x03,0x02,0x58,0x00,0x04,0xC4,0x62};
		//byte sendfrequency[]={0x02,0x03,0x00,0x22,0x00,0x01,0x24,0x33};
		//byte sendfrequency[]={0x01,0x03,0x00,0x00,0x00,0x08,0x44,0x0C};
		byte sendfrequency[]={0x01,0x03,0x00,0x70,0x00,0x10,0x45,0xDD};
		m_SerialPortOne.WriteToPort((char*)sendvoltage,8);
		m_SerialPortTwo.WriteToPort((char*)sendfrequency,8);
		WriteParamFileIniFile();
	}
	if(nIDEvent==2)
	{
		if(!m_bTimerFlag)
		{
			m_bTimerFlag=true;
			OnGrab();
		}
		else
		{
			KillTimer(2);
			OnFreeze();
		}
	}
	if(nIDEvent==1)
	{
		m_TimeCount++;
		m_View_1->Show();
		m_View_2->Show();
		m_dFreeSpaceG=ReadFreeSpace(m_strStoragePath);
		left=GetDirFileCount(m_strLeftPath);
		right=GetDirFileCount(m_strRightPath);
		if(m_TimeCount>4)
		{
			m_TimeCount=0;
			if(m_RevFlag)
			{
				::GetLocalTime(&time);
				temp="";
				//temp.Format("#%d-%02d-%02d %02d:%02d:%02d.%03d TLeft:%ld TRight:%ld ALeft:%ld ARight:%ld FreeSize:%.02fGB$\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_1,m_iCountImage_2,left,right,m_dFreeSpaceG);
				//temp.Format("#%02d:%02d:%02d.%03d TL:%ld TR:%ld AL:%ld AR:%ld FS:%.02fG$\r\n",time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_1,m_iCountImage_2,left,right,m_dFreeSpaceG);
				//temp.Format("#%03d T:%ld-%ld A:%ld-%ld S:%.02f$\r\n",time.wMilliseconds,m_iCountImage_1,m_iCountImage_2,left,right,m_dFreeSpaceG);
				temp.Format("#%03d L:%ld-%ld R:%ld-%ld S:%.02f$\r\n",time.wMilliseconds,m_iCountImage_1,left,m_iCountImage_2,right,m_dFreeSpaceG);
				//m_SerialPort.WriteToPort(temp.GetBuffer(),temp.GetLength());
				if(m_iWindowMode==1)
				{
					m_strInfo=temp;
					InvalidateRect(&m_RectInfoShow);
				}
			}
		}
		if(this!=GetForegroundWindow())
		{
			ShowWindow(SW_HIDE);
		}
		
		::GetLocalTime(&time);
		temp="";
		temp.Format("%d-%02d-%02d %02d:%02d:%02d.%03d TLeft:%ld TRight:%ld ALeft:%ld ARight:%ld FreeSize:%.02fGB$",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_1,m_iCountImage_2,left,right,m_dFreeSpaceG);
		m_statusWnd.SetWindowText(temp);
		temp="";
		temp.Format("%d-%02d-%02d %02d:%02d:%02d.%03d FS:%.02fG\r\nTL:%ld TR:%ld AL:%ld AR:%ld",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_dFreeSpaceG,m_iCountImage_1,m_iCountImage_2,left,right);
		strcpy_s(nid.szTip, temp);//信息提示条为"计划任务提醒" 
		Shell_NotifyIcon(NIM_MODIFY,&nid);//在托盘区添加图标 
		
	}
	__super::OnTimer(nIDEvent);
}


void CGigeCameraDemoDlg::OnMainitemTest()
{
	// TODO: 在此添加命令处理程序代码
	OnTest();
}


void CGigeCameraDemoDlg::OnBnClickedReturn()
{
	m_iWindowMode=1;
	// 隐藏TitleBar
	ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);


	//ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED); 
	//ModifyStyle(WS_CAPTION,0,0);//如果不想去掉标题栏，去掉该句。
	//SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
	//this->SetBackgroundColor(RGB(94,94,94),RGB(0,0,0)); 
	ShowWindow(SW_SHOWMAXIMIZED);


	//// 显示TitleBar
	//ModifyStyle(0, WS_CAPTION, SWP_FRAMECHANGED);
	////ModifyStyle(WS_CAPTION,0,1);//如果不想去掉标题栏，去掉该句。
	// //删除标题栏风格 
	////ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED); 
	//ShowWindow(SW_NORMAL);

	m_ImageWnd_1.ShowWindow(SW_HIDE);
	m_ImageWnd_2.ShowWindow(SW_HIDE);
	m_statusWnd.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_GRAB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FREEZE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TEST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RETURN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_VIEW_WND3)->MoveWindow(0,0,1024,768);
	GetDlgItem(IDC_VIEW_WND3)->ShowWindow(SW_SHOW);

	//GetDlgItem(IDC_VIEW_WND3)->MoveWindow(0,0,1024,768);
	//GetDlgItem(IDC_VIEW_WND3)->ShowWindow(SW_HIDE);
	//m_ImageWnd_1.ShowWindow(SW_SHOW);
	//m_ImageWnd_2.ShowWindow(SW_SHOW);
	//m_statusWnd.ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_GRAB)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_FREEZE)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_TEST)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_RETURN)->ShowWindow(SW_SHOW);


}

void CGigeCameraDemoDlg::ImageFrom_IDResource(UINT nID, LPCTSTR sTR, CImage *  pImg) 
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

void CGigeCameraDemoDlg::RefreshUI()
{
		//CPaintDC dc(this);
int iInfo;
CString str = "36.00V";
double SpeedVal=0.0;
CDC   hMemDC; //加载背景图片的内存DC
int cx, cy;  
CImage  image;
CRect   rect;
CDC *pDc = NULL;  
CWnd *pWnd = NULL;  
pWnd    = GetDlgItem(IDC_VIEW_WND3);//获取控件句柄  
//pWnd->MoveWindow(0,0,1024,768);
pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
pDc = pWnd->GetDC();//获取picture的DC  

//pDc =GetDC();//获取picture的DC  
//CPaintDC dc(this);
CDC  * pMemDc=NULL; //加载背景图片的内存DC
pMemDc=new CDC;
pMemDc->CreateCompatibleDC(pDc);
CBitmap bmp;//这里的Bitmap是必须的，否则当心弄出一个大黑块哦。
bmp.CreateCompatibleBitmap(pDc,rect.Width(),rect.Height());
pMemDc->SelectObject(&bmp);
 
ImageFrom_IDResource(IDB_PNG57,"PNG",&image);//背景图
rect.top =0;
rect.left =0;
rect.bottom=image.GetHeight();   
rect.right =image.GetWidth();  
image.Draw(pMemDc->m_hDC, rect);//将图片绘制到picture表示的区域内  
image.Detach();
if(m_iTempDirection==0)
{
	ImageFrom_IDResource(IDB_PNG47,"PNG",&image);//上行
}
else
{
	ImageFrom_IDResource(IDB_PNG48,"PNG",&image);//下行
}
image.Draw(pMemDc->m_hDC, m_RectDirectionShow);//将图片绘制到picture表示的区域内  
image.Detach();

if(m_iTempAddMileage==0)
{
	ImageFrom_IDResource(IDB_PNG49,"PNG",&image);//里程增加
}
else
{
	ImageFrom_IDResource(IDB_PNG50,"PNG",&image);//里程减少
}
image.Draw(pMemDc->m_hDC, m_RectAddMileageShow);//将图片绘制到picture表示的区域内 
image.Detach();

//m_dbVoltage_1=55.0;
//m_dbVoltage_2=55.0;
if(m_dbVoltage_1>=52.0)
{
	ImageFrom_IDResource(IDB_PNG51,"PNG",&image);
}else if(m_dbVoltage_1>=50.0)
{
	ImageFrom_IDResource(IDB_PNG52,"PNG",&image);
}else if(m_dbVoltage_1>=48.0)
{
	ImageFrom_IDResource(IDB_PNG53,"PNG",&image); 
}else if(m_dbVoltage_1>=46.0)
{
	ImageFrom_IDResource(IDB_PNG54,"PNG",&image); 
}else if(m_dbVoltage_1>=43.0)
{
	ImageFrom_IDResource(IDB_PNG55,"PNG",&image);
}else if(m_dbVoltage_1<43.0)
{
	ImageFrom_IDResource(IDB_PNG56,"PNG",&image); 
}
image.Draw(pMemDc->m_hDC, m_RectBatteryShow_1);//将图片绘制到picture表示的区域内
image.Detach();

if(m_dbVoltage_2>=52.0)
{
	ImageFrom_IDResource(IDB_PNG51,"PNG",&image); 
}else if(m_dbVoltage_2>=50.0)
{
	ImageFrom_IDResource(IDB_PNG52,"PNG",&image);
}else if(m_dbVoltage_2>=48.0)
{
	ImageFrom_IDResource(IDB_PNG53,"PNG",&image); 
}else if(m_dbVoltage_2>=46.0)
{
	ImageFrom_IDResource(IDB_PNG54,"PNG",&image);
}else if(m_dbVoltage_2>=43.0)
{
	ImageFrom_IDResource(IDB_PNG55,"PNG",&image);
}else if(m_dbVoltage_2<43.0)
{
	ImageFrom_IDResource(IDB_PNG56,"PNG",&image);
}
image.Draw(pMemDc->m_hDC, m_RectBatteryShow_2);//将图片绘制到picture表示的区域内 
image.Detach();

	CFont font;
	//font.CreateFont(15, 0, 0, 0, FW_BOLD,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"Times New Roman");
	font.CreateFont(40, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
	pMemDc->SetBkMode(TRANSPARENT);
	pMemDc->SetTextColor(RGB(255,255,255));    //Static控件1的字体颜色-红色
	pMemDc->SelectObject(&font); 

	//日期时间区域
	CTime time = CTime::GetCurrentTime();       
	str.Format("%04d-%02d-%02d %02d:%02d:%02d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());  
    pMemDc->DrawText(str,m_RectDateTimeShow, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
	//里程区域
	//str.Format(L"2017-03-01 16:32:10");
    pMemDc->DrawText(m_strTempMileage,m_RectMileageShow, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//电池1区域
	str.Format("%.2fV",m_dbVoltage_1);
    pMemDc->DrawText(str,m_RectElectricShow_1, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//电池2区域
	str.Format("%.2fV",m_dbVoltage_2);
    pMemDc->DrawText(str,m_RectElectricShow_2, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//速度区域
	SpeedVal=m_dbSpeed*m_XiShu[1];
	str.Format("%.2fKm/h",SpeedVal);
    pMemDc->DrawText(str,m_RectSpeedShow, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	////总里程区域
	//str.Format(L"%.2fKm",m_dbTotalMileage);
 //   dc.DrawText(str,m_RectTotalMileageShow, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
	//CRect rect;
	//rect.left=m_RectTotalMileageShow.left-3; 
	//rect.bottom=m_RectTotalMileageShow.bottom+3;
	//rect.top =m_RectTotalMileageShow.top-3;
	//rect.right=m_RectTotalMileageShow.right+3;
 //   dc.FillSolidRect(&rect,RGB(255,0,0));//红色填充
	font.DeleteObject(); 

	CPen RectPen(PS_SOLID,5,RGB(255,0,0));
	pMemDc->SelectObject(&RectPen); 
	pMemDc->MoveTo(313,282);
	pMemDc->LineTo((int)(313+cos((360-(180-(0+SpeedVal*3.38/5*180/27)))*PI/180)*200),(int)(282+sin((360-(180-(0+SpeedVal*3.38/5*180/27)))*PI/180)*200));
	RectPen.DeleteObject();

	//font.CreateFont(15, 0, 0, 0, FW_BOLD,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"Times New Roman");
	font.CreateFont(28, 0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
	pMemDc->SetBkMode(TRANSPARENT);
	pMemDc->SetTextColor(RGB(255,255,255));    //Static控件1的字体颜色-红色
	pMemDc->SelectObject(&font); 

	//信息区域
	//str.Format(L"2017-03-01 16:32:10.333 T:9999999-9999999 A:9999999-9999999 F:200.345");
	//str.Format(L"333 T:9999999-9999999 A:9999999-9999999 F:200.345");
	//DT_CALCRECT | DT_WORDBREAK  |  DT_EDITCONTROL|  DT_NOPREFIX | DT_EXTERNALLEADING
	//dc.DrawText(str,m_RectInfoShow,  DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK );
	//m_strInfo="2017-03-01 16:32:10.333 T:9999999-9999999 A:9999999-9999999 F:200.345";
	
	if((iInfo=m_strInfo.Find('$'))>1)
		m_strInfo=m_strInfo.Mid(1,iInfo-1);
    pMemDc->DrawText(m_strInfo,m_RectInfoShow, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);

	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
	pDc->BitBlt(0,0,rect.Width(),rect.Height(),pMemDc,0,0,SRCCOPY);

    font.DeleteObject(); 
	bmp.DeleteObject();
	pMemDc->DeleteDC();
}

void CGigeCameraDemoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd *pWnd = NULL;  
	pWnd    = GetDlgItem(IDC_VIEW_WND3);//获取控件句柄  
	if(m_iWindowMode!=1)return;

	if(m_RectExitShow.PtInRect(point)){
		//m_iExitCount=(m_iExitCount>1?1:m_iExitCount+1);
		m_iExitCount++;
		if(m_iExitCount>2)
		{
			m_iExitCount=0;
			m_iWindowMode=2;
			// 显示TitleBar
			ModifyStyle(0, WS_CAPTION, SWP_FRAMECHANGED);
			//DWORD dwExStyle = GetExStyle();//获取旧扩展样式  
			//DWORD dwNewExStyle = WS_EX_LEFT |WS_EX_LTRREADING |WS_EX_RIGHTSCROLLBAR;  
			//dwNewExStyle&=dwExStyle;//按位与将旧扩展样式加上 
			//SetWindowLong(m_hWnd,GWL_EXSTYLE,dwNewExStyle);//设置新的扩展样式   
			//SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);//窗口位置和大小保持原来不变！

			//ModifyStyle(WS_CAPTION,0,1);//如果不想去掉标题栏，去掉该句。
			//删除标题栏风格 
			//ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED); 
		
			//ShowWindow(SW_SHOWMAXIMIZED);
			GetDlgItem(IDC_VIEW_WND3)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_VIEW_WND3)->MoveWindow(0,0,10,10);
			m_ImageWnd_1.ShowWindow(SW_SHOW);
			m_ImageWnd_2.ShowWindow(SW_SHOW);
			m_statusWnd.ShowWindow(SW_SHOW);
			GetDlgItem(IDC_GRAB)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_FREEZE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_TEST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_RETURN)->ShowWindow(SW_SHOW);
			ShowWindow(SW_NORMAL);
		}
	}else if(m_RectWholeMileageButton.PtInRect(point)){//整里程校正
		m_iDotInfo=1;
		WritePosInfo(m_iDotInfo);
	}else if(m_RectForkStartButton.PtInRect(point)){//岔起
		m_iDotInfo=2;
		WritePosInfo(m_iDotInfo);
	}else if(m_RectForkFinishButton.PtInRect(point)){//岔终
		m_iDotInfo=3;
		WritePosInfo(m_iDotInfo);
	}else if(m_RectCrossingStartButton.PtInRect(point)){//道口起
		m_iDotInfo=4;
		WritePosInfo(m_iDotInfo);
	}else if(m_RectCrossingFinishButton.PtInRect(point)){//道口终
		m_iDotInfo=5;
		WritePosInfo(m_iDotInfo);
	}else if(m_RectKeyBack.PtInRect(point)){//退格
		m_strTempMileage.Delete(m_strTempMileage.GetLength()-1,1);
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKeyClear.PtInRect(point)){//清除
		m_strTempMileage="";
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKeyOk.PtInRect(point)){//确定
	MessageBox("确定!");
	}else if(m_RectKey0.PtInRect(point)){//0
		m_strTempMileage.AppendChar('0');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKey1.PtInRect(point)){//1
		m_strTempMileage.AppendChar('1');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKey2.PtInRect(point)){//2
		m_strTempMileage.AppendChar('2');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKey3.PtInRect(point)){//3
		m_strTempMileage.AppendChar('3');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKey4.PtInRect(point)){//4
		m_strTempMileage.AppendChar('4');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKey5.PtInRect(point)){//5
		m_strTempMileage.AppendChar('5');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKey6.PtInRect(point)){//6
		m_strTempMileage.AppendChar('6');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKey7.PtInRect(point)){//7
		m_strTempMileage.AppendChar('7');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKey8.PtInRect(point)){//8
		m_strTempMileage.AppendChar('8');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKey9.PtInRect(point)){//9
		m_strTempMileage.AppendChar('9');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKeyDecimal.PtInRect(point)){//.
		m_strTempMileage.AppendChar('.');
		InvalidateRect(&m_RectMileageShow);
	}else if(m_RectKeyUp.PtInRect(point)){//上行
		m_iTempDirection=0;
		InvalidateRect(&m_RectDirectionShow);
	}else if(m_RectKeyDwon.PtInRect(point)){//下行
		m_iTempDirection=1;
		InvalidateRect(&m_RectDirectionShow);
	}else if(m_RectKeyAdd.PtInRect(point)){//里程增加
		m_iTempAddMileage=0;
		InvalidateRect(&m_RectAddMileageShow);
	}else if(m_RectKeySubtract.PtInRect(point)){//里程减少
		m_iTempAddMileage=1;
		InvalidateRect(&m_RectAddMileageShow);
	}
	__super::OnLButtonDown(nFlags, point);
}


BOOL CGigeCameraDemoDlg::OnEraseBkgnd(CDC* pDC)
{
	if(m_iWindowMode==1)
	{
		if(m_iFirstFlag==0)
		{
			//CImage  image;
			//CRect   rect;
			//CDC *pDc = NULL;  
			//CWnd *pWnd = NULL;  
			//pWnd    = GetDlgItem(IDC_VIEW_WND3);//获取控件句柄  
			//pWnd->MoveWindow(0,0,1024,768);
			//pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
			//pDc = pWnd->GetDC();//获取picture的DC  
			//ImageFrom_IDResource(IDB_PNG57,"PNG",&image);//背景图
			//rect.top =0;
			//rect.left =0;
			//rect.bottom=image.GetHeight();   
			//rect.right =image.GetWidth();  
			//image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内  
			//image.Detach();
		}
		return true;
	}
	else
	{
		//HBRUSH hBrush =  CreateSolidBrush(DLG_BACKGROUND); //创建背景刷;

	
       // pDC->SetBkMode(TRANSPARENT);  
		//pDC->SetBkColor(RGB(212,208,200));
		return __super::OnEraseBkgnd(pDC);
	}
}


void CGigeCameraDemoDlg::WritePosInfo(int Value)
{
	CString strRec;
	SYSTEMTIME time;
	::GetLocalTime(&time);

	strRec.Format("%d-%02d-%02d %02d:%02d:%02d.%03d Num=%ld Info=%d",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,m_iCountImage_1,Value);
	if(m_iWindowMode==1){
		m_strInfo=strRec;
		InvalidateRect(&m_RectInfoShow);
	}else{
		m_statusWnd.SetWindowText(strRec);
	}
	//str.Format(_T("Frames acquired in trash buffer: %s||"), (LPSTR)wParam);
	CString str;
	str.Format("%s\\%s\\Record.rec",m_strStoragePath.GetBuffer(),m_pTimeFileName);
	ZLBCreateDirectory(str.GetBuffer());
	CStdioFile Rec;
	Rec.Open(str,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	Rec.SeekToEnd();
	Rec.WriteString(strRec+"\r\n");
	Rec.Close();
}


void CGigeCameraDemoDlg::SetUI(void)
{
	m_RectExitShow.SetRect(32,19,96,92);
	m_RectDateTimeShow.SetRect(642,48,1000,86);//日期时间
	m_RectMeterShow.SetRect(82,51,543,288);//表盘区域包含速度区
	m_RectMileageShow.SetRect(22,326,450,358);//输入里程
	m_RectInfoShow.SetRect(22,728,1000,760);//信息区域
	m_RectSpeedShow.SetRect(218,210,399,244);//速度区域

	
	m_RectBatteryShow_1.SetRect(710,163,741,230);//电池1区域
	m_RectBatteryShow_2.SetRect(901,163,932,230);//电池2区域
	m_RectElectricShow_1.SetRect(667,240,784,277);//电量1区域
	m_RectElectricShow_2.SetRect(857,240,976,277);//电量2区域

	m_RectDirectionShow.SetRect(468,325,537,363);//上下行区域
	m_RectAddMileageShow.SetRect(542,325,610,363);//加减里程区域

	m_RectWholeMileageButton.SetRect(646,369,1003,482);//整里程校正按钮
	m_RectForkStartButton.SetRect(646,489,818,601);//道岔起点按钮
	m_RectForkFinishButton.SetRect(831,488,1003,601);//道岔终点按钮
	m_RectCrossingStartButton.SetRect(646,607,818,720);//道口起点按钮
	m_RectCrossingFinishButton.SetRect(831,607,1003,720);//道口终点按钮
	
	

	m_RectKey7.SetRect(18,368,130,453);//KEY7按钮
	m_RectKey8.SetRect(137,368,252,453);//KEY8按钮
	m_RectKey9.SetRect(259,368,370,453);//KEY9按钮
	m_RectKeyUp.SetRect(380,368,492,453);//UP按钮
	m_RectKeyBack.SetRect(501,368,614,453);//BACK按钮

	m_RectKey4.SetRect(18,456,130,542);//KEY4按钮
	m_RectKey5.SetRect(139,456,251,542);//KEY5按钮
	m_RectKey6.SetRect(258,456,372,542);//KEY6按钮
	m_RectKeyDwon.SetRect(379,456,492,542);//DOWN按钮
	m_RectKeyClear.SetRect(501,456,613,542);//CLEAR按钮

	m_RectKey1.SetRect(18,545,130,630);//KEY1按钮
	m_RectKey2.SetRect(138,545,251,630);//KEY2按钮
	m_RectKey3.SetRect(258,545,372,630);//KEY3按钮
	m_RectKeyAdd.SetRect(380,545,492,630);//ADD按钮


	m_RectKey0.SetRect(18,634,250,720);//KEY0按钮
	m_RectKeyDecimal.SetRect(259,634,372,720);//DECIMAL按钮
	m_RectKeySubtract.SetRect(380,634,492,720);//SUBTRACT按钮
	
	
	m_RectKeyOk.SetRect(500,545,613,720);//OK按钮

}


int CGigeCameraDemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_pSplashWindow= new CSplashWnd();
    //m_pSplashWindow->CreateSplash(); 
	//HideCursor();
	return 0;
}

void CGigeCameraDemoDlg::HideCursor()//隐藏光标
{
 CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
 SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}