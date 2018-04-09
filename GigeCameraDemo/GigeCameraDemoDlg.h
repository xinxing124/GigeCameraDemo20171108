// GigeCameraDemoDlg.h : header file
//

#if !defined(AFX_GIGECAMERADEMODLG_H__82BFE149_F01E_11D1_AF74_00A0C91AC0FB__INCLUDED_)
#define AFX_GIGECAMERADEMODLG_H__82BFE149_F01E_11D1_AF74_00A0C91AC0FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "ZLBCommonLib.h"
#include "ZLBImageLib.h"
#include "InputDialog.h"
#include "MessageDialog.h"
#include "afxwin.h"
#include "SplashWnd.h"
#include "SerialPort.h"	// Added by ClassView

#define PI	3.14159265
#define WM_SHOWTASK (WM_USER+10)

#define IMAGE_WIDTH    2048
#define IMAGE_HEIGHT   2048
#define IMAGE_CHANNELS 1
//buffer number for motion jpeg 
#define NUM_BUFF	100

//Size of each buffer loading MJpeg
#define	MAX_BUFF	IMAGE_WIDTH*IMAGE_HEIGHT*IMAGE_CHANNELS    

typedef struct tag_BuffNode {
	int					sizeBuff;
	char				pBuff[MAX_BUFF];
	int                 iImageWidth;
	int                 iImageHeight;
	int                 iImageType;
	ULONGLONG                 iImageCount;
	char				pName[100];
	BOOL				bIsLocked;
}BUFFNODE;

/////////////////////////////////////////////////////////////////////////////
// CGigeCameraDemoDlg dialog

class CGigeCameraDemoDlg : public CDialog, public CImageExWndEventHandler
{
// Construction
public:
	CGigeCameraDemoDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL IsUserAdmin();
	BOOL CreateObjects_1();
	BOOL DestroyObjects_1();
	BOOL CreateObjects_2();
	BOOL DestroyObjects_2();
	void UpdateMenu();
	static void XferCallback_1(SapXferCallbackInfo *pInfo);
	static void XferCallback_2(SapXferCallbackInfo *pInfo);

   void PixelChanged(int x, int y);

// Dialog Data
	//{{AFX_DATA(CGigeCameraDemoDlg)
	enum { IDD = IDD_GIGECAMERADEMO_DIALOG };
	CStatic	m_statusWnd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGigeCameraDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL b_show;
	HICON		m_hIcon;
	CString  m_appTitle;
	CString	m_serverName_1;
	int m_resourceIndex_1 ;
	CString m_configFileName_1;
    CImageExWnd    m_ImageWnd_1;
	SapAcquisition m_Acq_1;		// Internal SapAcquisition object
	SapAcqDevice	*m_AcqDevice_1;
	SapBuffer		*m_Buffers_1;
	SapTransfer		*m_Xfer_1;
	SapView        *m_View_1;

	CString	m_serverName_2;
	int m_resourceIndex_2;
	CString m_configFileName_2;
	CImageExWnd    m_ImageWnd_2;
	SapAcquisition m_Acq_2;		// Internal SapAcquisition object
	SapAcqDevice	*m_AcqDevice_2;
	SapBuffer		*m_Buffers_2;
	SapTransfer		*m_Xfer_2;
	SapView        *m_View_2;

	unsigned int m_TimeCount;
	bool	m_RevFlag;
	HANDLE          g_hThread;
	HANDLE          g_hThread_1;
	HANDLE          g_hThread_2;
	// Generated message map functions
	//{{AFX_MSG(CGigeCameraDemoDlg)

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTest();
	afx_msg void OnGrab();
	afx_msg void OnFreeze();
   afx_msg void OnEndSession(BOOL bEnding);
   afx_msg BOOL OnQueryEndSession();
   afx_msg LRESULT OnReceiveData(WPARAM wParam, LPARAM lParam);
   afx_msg   LRESULT   onShowTask(WPARAM   wParam,LPARAM   lParam); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	/*从配置文件中读取参数*/
	bool ReadParamFromIniFile();
	/*将参数配置写入到配置文件中*/
	bool WriteParamFileIniFile();
	/*将数据缓存进入内存*/
	//bool PushImageToMyBuffer(BUFFNODE listImageBuff[],char *pBuffer,int iBufferLength,int iWidth,int iHeight,int iImageType,int iImageCount);
	/*是否启动贝尔滤波算法*/
	afx_msg void OnCbnSelchangeComboBayer();

public:
	//CSplashWnd *m_pSplashWindow;
	CMessageDialog *pMessageDialog;
	//CSerialPort m_SerialPort;
	CSerialPort m_SerialPortOne;
	CSerialPort m_SerialPortTwo;
	NOTIFYICONDATA nid;
	bool m_bTimerFlag;
	POINT Old;
	CString	m_strReceive;
	
	ULONGLONG m_SaveCount_1;
	ULONGLONG m_SaveCount_2;
	//配置文件句柄
	HANDLE    m_iniHandle;
	//是否开启自动存储功能
	int m_iAutoStorage;
	//存储路径
	CString m_strStoragePath;
	CString m_strLeftPath;
	CString m_strRightPath;
	
	//存储格式
	int m_iStorageFormat;
	//数据格式
	int m_iPixelFormat;
	//图像颜色
	int m_iImageColor;
	//当前计数
	ULONGLONG m_iCurrentCount_1;
	ULONGLONG m_iCurrentCount_2;
	double m_dFreeSpaceG;
	//JPEG压缩因子
	int m_iJpegFac;

	// 是否存储图像控件变量
	CButton m_checkSaveImage;
	// 图像数据存储总数控件变量
	CEdit m_edSaveImageCount;
	// 存储图像路径控件变量
	CEdit m_edSaveImagePath;
	// 存储图像格式控件变量
	CComboBox m_comSaveImageFormat;
	// 像素位深控件变量
	CComboBox m_comImagePixelFormat;
	// 图像通道数
	CComboBox m_comChannles;
	// JPEG压缩因子
	CEdit m_edJpegFac;
	/*贝尔滤波图像*/
	SapColorConversion   *m_Bayer;
	// 是否启动贝尔滤波算法
	CButton m_bBayerEnable;
	/*贝尔滤波算法使能*/
	int  m_iBayerEnable;
	CComboBox m_bayerType;
	int  m_iBayerType;
	char *m_pTimeFileName;

	afx_msg void OnMainitemShow();
	afx_msg void OnMainitemHide();
	afx_msg void OnMainitemExit();
	//virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMainitemTest();
	afx_msg void FitToWindow(SapView *pView);
	afx_msg void OnBnClickedReturn();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void ImageFrom_IDResource(UINT nID, LPCTSTR sTR, CImage *  pImg) ;
public:
	void RefreshUI();

	double m_XiShu[2];
	int m_iDirectionFlag;
	int m_iAddMileageFlag;
	int m_iTempDirection;
	int m_iTempAddMileage;
	BYTE m_cRecvBuf[1024];
	CString m_strInfo;
	CString m_strTempMileage;
	CString m_strCurrMileage;
	double m_dbVoltage_1;
	double m_dbVoltage_2;
	double m_dbSpeed;
	double m_dbOldVoltage_1;
	double m_dbOldVoltage_2;
	double m_dbOldSpeed;
	double m_dbTotalMileage;
	int m_iExitCount;
	int m_iDotInfo;

	CRect m_RectExitShow;
	CRect m_RectDateTimeShow;
	CRect m_RectMileageShow;
	CRect m_RectMeterShow;
	CRect m_RectBatteryShow_1;
	CRect m_RectBatteryShow_2;
	CRect m_RectElectricShow_1;
	CRect m_RectElectricShow_2;
	CRect m_RectSpeedShow;
	CRect m_RectTotalMileageShow;
	CRect m_RectInfoShow;
	CRect m_RectDirectionShow;
	CRect m_RectAddMileageShow;
	CRect m_RectWholeMileageButton;
	CRect m_RectForkStartButton;
	CRect m_RectForkFinishButton;
	CRect m_RectCrossingStartButton;
	CRect m_RectCrossingFinishButton;
	CRect m_RectKey0;
	CRect m_RectKey1;
	CRect m_RectKey2;
	CRect m_RectKey3;
	CRect m_RectKey4;
	CRect m_RectKey5;
	CRect m_RectKey6;
	CRect m_RectKey7;
	CRect m_RectKey8;
	CRect m_RectKey9;
	CRect m_RectKeyAdd;
	CRect m_RectKeySubtract;
	CRect m_RectKeyUp;
	CRect m_RectKeyDwon;
	CRect m_RectKeyOk;
	CRect m_RectKeyBack;
	CRect m_RectKeyClear;
	CRect m_RectKeyDecimal;
	CRect m_RectKeyReturn;
	int m_iWindowMode;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void WritePosInfo(int Value);
	void SetUI(void);
	void HideCursor();
	int m_iFirstFlag;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GIGECAMERADEMODLG_H__82BFE149_F01E_11D1_AF74_00A0C91AC0FB__INCLUDED_)
