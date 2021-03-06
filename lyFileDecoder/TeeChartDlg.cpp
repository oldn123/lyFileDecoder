
// TeeChartDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TeeChart.h"
#include "TeeChartDlg.h"
#include "afxdialogex.h"

#include <vector>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTeeChartDlg 对话框



CTeeChartDlg::CTeeChartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TEECHART_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCurData = NULL;
}

void CTeeChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtDateCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dtTimeCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_dtTimeCtrl2);
}

BEGIN_MESSAGE_MAP(CTeeChartDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CTeeChartDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON1, &CTeeChartDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CTeeChartDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_SELINPUTFILE, &CTeeChartDlg::OnBnClickedButtonSelinputfile)
	ON_BN_CLICKED(IDC_BUTTONSAVE, &CTeeChartDlg::OnBnClickedButtonsave)
	ON_WM_SIZE()
END_MESSAGE_MAP()


CString GetPath(LPCTSTR lpszName)
{
	CString strPath(lpszName);
	int nPos1 = strPath.ReverseFind(_T('\\'));		//先取文件名
	int nPos2 = strPath.ReverseFind(_T('\/'));
	int nPos = nPos2 < nPos1 ? nPos1 : nPos2;
	if (nPos > 0)
		strPath = strPath.Left(nPos);

	return strPath;
}


CString GetFileName(LPCTSTR lpszFullFileName, bool bGetExtName)
{
	CString strFileName(lpszFullFileName);
	CString sName = strFileName;
	int nPos1 = strFileName.ReverseFind(_T('\\'));		//先取文件名
	int nPos2 = strFileName.ReverseFind(_T('\/'));
	int nPos = nPos2 < nPos1 ? nPos1 : nPos2;
	if (nPos > 0)
	{
		sName = strFileName.Right(strFileName.GetLength() - nPos - 1);
	}

	nPos = sName.ReverseFind(_T('.'));

	if (!bGetExtName && nPos > 0)
	{
		CString sNewName = sName.Left(nPos);
		sName = sNewName;
	}

	return sName;
}

bool CTeeChartDlg::LoadData()
{
	CLyData * pData = new CLyData;
	CString sInput;
	GetDlgItemText(IDC_EDIT_INPUTPATH, sInput);
	
	if (!pData->LoadFile(sInput))
	{
		delete pData;
		return false;
	}
	USES_CONVERSION;
	string sKey = T2A(sInput);
	
	if (m_cheFileDataMap.count(sKey))
	{
		delete m_cheFileDataMap[sKey];
	}
	m_cheFileDataMap[sKey] = pData;
	m_pCurData = pData;

	CString sFile2;
	GetDlgItemText(IDC_EDIT_OUTPUTPATH, sFile2);
	bool bNeedSetPath = sFile2.GetLength() < 1;
	sFile2 = sInput.Left(sInput.GetLength() - 3) + L"_New.dat";
	if (bNeedSetPath)
	{	
		SetDlgItemText(IDC_EDIT_OUTPUTPATH, GetPath(sFile2));
	}
	
	SetDlgItemText(IDC_EDIT_NAME, GetFileName(sFile2, true));

	if (m_pCurData->GetData()->nNumber)
	{
		SetDlgItemInt(IDC_EDIT_DATA1, *m_pCurData->GetData()->nNumber);
	}
	GetDlgItem(IDC_EDIT_DATA1)->EnableWindow(m_pCurData->GetData()->nNumber != NULL);

	COleDateTime oleTime1;
	oleTime1.SetDate(*m_pCurData->GetData()->wYear, *m_pCurData->GetData()->byteMonth, *m_pCurData->GetData()->byteDay);
	m_dtDateCtrl.SetTime(oleTime1);
	oleTime1.SetTime(*m_pCurData->GetData()->byteHour, *m_pCurData->GetData()->byteMinute, 0);
	m_dtTimeCtrl.SetTime(oleTime1);

	if (m_pCurData->GetData()->byteHour2)
	{
		oleTime1.SetTime(*m_pCurData->GetData()->byteHour2, *m_pCurData->GetData()->byteMinute2, 0);
		m_dtTimeCtrl2.SetTime(oleTime1);
	}
	
	m_dtTimeCtrl2.EnableWindow(m_pCurData->GetData()->byteHour2 != NULL);
	

	CString sText;
	for (int i = IDC_EDIT_DATA2; i <= IDC_EDIT_DATA24; i++)
	{
		GetDlgItem(i)->EnableWindow(m_pCurData->GetData()->fValues[eData_2 + i - IDC_EDIT_DATA2] != NULL);
		if (i == IDC_EDIT_DATA11)
		{
			GetDlgItem(IDC_DATETIMEPICKER_11)->EnableWindow(m_pCurData->GetData()->fValues[eData_2 + i - IDC_EDIT_DATA2] != NULL);
		}
		if (m_pCurData->GetData()->fValues[eData_2 + i - IDC_EDIT_DATA2] == NULL)
		{
			continue;
		}

		if (i == IDC_EDIT_DATA11)
		{
			int nv = *(int*)m_pCurData->GetData()->fValues[eData_2 + i - IDC_EDIT_DATA2];
			COleDateTimeSpan tms;
			tms.SetDateTimeSpan(0,0,0,nv);
		
			COleDateTime tm;
			tm = tms;
			CDateTimeCtrl * pdtc = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_11); 
			if (pdtc)
			{
				pdtc->SetTime(tm);
			}
		}
		else
		{
			float fv = *m_pCurData->GetData()->fValues[eData_2 + i - IDC_EDIT_DATA2];
			sText.Format(L"%.2f", fv);
		}
		SetDlgItemText(i, sText);
	}

	float fv = m_pCurData->GetData()->fNox;
	sText.Format(L"%.2f", fv);
	SetDlgItemText(IDC_EDIT_DATA25, sText);

	return true;
}


BOOL CTeeChartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。


	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	COleDateTime tm;
	tm.SetTime(0,0,0);
	CDateTimeCtrl * pdtc = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_11); 
	if (pdtc)
	{
		pdtc->SetTime(tm);
	}	
	
	pdtc = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER3); 
	if (pdtc)
	{
		pdtc->SetTime(tm);
	}
	
	pdtc = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER2); 
	if (pdtc)
	{
		pdtc->SetTime(tm);
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTeeChartDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTeeChartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTeeChartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTeeChartDlg::OnBnClickedButtonOpen()
{
	
}


BEGIN_EVENTSINK_MAP(CTeeChartDlg, CDialog)
END_EVENTSINK_MAP()




void CTeeChartDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	LoadData();
}



void CTeeChartDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
}


void CTeeChartDlg::OnBnClickedButtonSelinputfile()
{
	// TODO: Add your control notification handler code here

	CFileDialog filedialog(TRUE,NULL,L"",OFN_HIDEREADONLY, L"All Files(*.ly)|*.ly||",NULL);
	if(filedialog.DoModal() == IDOK)
	{
		CString sFile = filedialog.GetPathName();
		SetDlgItemText(IDC_EDIT_INPUTPATH, sFile);

		LoadData();
	}
}

LPCTSTR g_charCode = L"1234567890.";

BOOL CTeeChartDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int nNotifyCode = HIWORD(wParam);
	int nCtrlId = LOWORD(wParam);
	if (nNotifyCode == EN_CHANGE)
	{
		if (nCtrlId >= IDC_EDIT_DATA1 && nCtrlId <= IDC_EDIT_DATA24)
		{
			CString sInput;
			GetDlgItemText(nCtrlId, sInput);
			int nLen = sInput.GetLength();
			if (nLen > 0)
			{
				bool bHasFloat = false;
				TCHAR sTmpBuf[250] = {0};
				for (int i = 0; i < nLen; i++)
				{
					CString OneStr = sInput.Mid(i,1); 
					if (OneStr == L".")
					{
						if (!bHasFloat)
						{
							if (i != 0)
							{
								bHasFloat = true;
								_tcscat(sTmpBuf, (LPCTSTR)OneStr);
							}
						}
					}
					else
					{
						if(-1 != OneStr.FindOneOf(g_charCode))
						{
							_tcscat(sTmpBuf, (LPCTSTR)OneStr);
						}
					}	
				}

				if (sInput != sTmpBuf)
				{
					sInput = sTmpBuf;
					SetDlgItemText(nCtrlId, sInput);
					CEdit * pE = (CEdit *)GetDlgItem(nCtrlId);
					pE->SetSel(nLen - 1, nLen - 1, FALSE);
					pE->SetFocus();
				}
			}
		}
	}
	else if (nNotifyCode == BN_CLICKED)
	{
		if (nCtrlId == IDC_BUTTONSAVE)
		{
			OnBnClickedButtonsave();
			return true;
		}
		else if (nCtrlId == IDC_BUTTON_OUTDIR)
		{
			OnBnClickedButtonOutdir();
		}
		else if (nCtrlId == IDC_BUTTON_RESET)
		{
			OnBnClickedButtonReset();
		}
	}
	return __super::OnCommand( wParam,  lParam);
}

void CTeeChartDlg::OnBnClickedButtonsave()
{
	// TODO: Add your control notification handler code here
	if (m_pCurData)
	{		
		SYSTEMTIME sysDate;
		SYSTEMTIME sysTime1;
		SYSTEMTIME sysTime2;
		m_dtDateCtrl.GetTime(&sysDate);
		m_dtTimeCtrl.GetTime(&sysTime1);
		m_dtTimeCtrl2.GetTime(&sysTime2);

		*m_pCurData->GetData()->wYear = sysDate.wYear;
		*m_pCurData->GetData()->byteMonth = sysDate.wMonth;
		*m_pCurData->GetData()->byteDay = sysDate.wDay;

		*m_pCurData->GetData()->byteMinute = sysTime1.wMinute;
		*m_pCurData->GetData()->byteHour = sysTime1.wHour;

		if (m_pCurData->GetData()->byteHour2)
		{
			*m_pCurData->GetData()->byteHour2 = sysTime2.wHour;
			*m_pCurData->GetData()->byteMinute2 = sysTime2.wMinute;
		}

		CString sText;
		for (int i = IDC_EDIT_DATA2; i <= IDC_EDIT_DATA24; i++)
		{
			if (m_pCurData->GetData()->fValues[eData_2 + i - IDC_EDIT_DATA2] == NULL)
			{
				continue;
			}

			if (i == IDC_EDIT_DATA11)
			{
				COleDateTimeSpan tms;
				COleDateTime tm;
				CDateTimeCtrl * pdtc = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_11); 
				if (pdtc)
				{
					pdtc->GetTime(tm);
				}
				tms = tm;
				int nv = tms.GetSeconds();
				*(int*)m_pCurData->GetData()->fValues[eData_2 + i - IDC_EDIT_DATA2] = nv;
			}
			else
			{
				GetDlgItemText(i, sText);
				float fv = _ttof(sText);
				*m_pCurData->GetData()->fValues[eData_2 + i - IDC_EDIT_DATA2] = fv;
			}	
		}


		CString sOutPath;
		CString sOutName;
		GetDlgItemText(IDC_EDIT_OUTPUTPATH, sOutPath);
		GetDlgItemText(IDC_EDIT_NAME, sOutName);
		sOutPath += L"\\";
		sOutPath += sOutName;

		if(sOutPath.GetLength() > 0 && m_pCurData->SaveFile(sOutPath))
		{
			MessageBox(L"保存成功",L"提示");
			return;
		}
	}	
	MessageBox(L"保存失败",L"提示");
}


CRect GetCtrlRect(HWND hDlg, HWND hSubWnd)
{
	if (hDlg == NULL || !IsWindow(hDlg))	return CRect(0,0,0,0);

	CRect rtClient;

	HWND hWnd = hSubWnd;
	if (hWnd == NULL || !IsWindow(hWnd))	return CRect(0,0,0,0);
	::GetWindowRect(hWnd, &rtClient);

	CWnd::FromHandle(hDlg)->ScreenToClient(&rtClient);

	return rtClient;
}

CRect GetCtrlRect(HWND hDlg, UINT nID)
{
	if (hDlg == NULL || !IsWindow(hDlg))	return CRect(0,0,0,0);

	HWND hWnd = ::GetDlgItem(hDlg, nID);

	return GetCtrlRect(hDlg, hWnd);
}

void CTeeChartDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

}

void CTeeChartDlg::OnBnClickedButtonOutdir()
{
	// TODO: Add your control notification handler code here

	//选择输出路径
	TCHAR szDir[MAX_PATH];
	BROWSEINFO bi; 
	ITEMIDLIST *pidl; 
	bi.hwndOwner = this->m_hWnd; 
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;//这个是输出缓冲区 
	bi.lpszTitle = _T("选择输出文件夹："); //标题
	bi.ulFlags = BIF_NEWDIALOGSTYLE;//使用新的界面,在win7中效果较好//BIF_RETURNONLYFSDIRS; 
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);//弹出对话框 
	if (pidl == NULL)//点了取消，或者选择了无效的文件夹则返回NULL
		return;

	if(SHGetPathFromIDList(pidl, szDir)) 
	{
		SetDlgItemText(IDC_EDIT_OUTPUTPATH, szDir);
	}

}


void CTeeChartDlg::OnBnClickedButtonReset()
{
	// TODO: Add your control notification handler code here
	if (m_pCurData)
	{
		if (MessageBox(L"确定要恢复原波形数据吗？", L"提示", MB_YESNO) == IDNO)
		{
			return;
		}
		LoadData();
	}
	else
	{
		MessageBox(L"没有找到需要恢复的数据", L"提示", 0);
	}
}

