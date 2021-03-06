
// TeeChartDlg.h: 头文件
//

#pragma once
#include "afxwin.h"

#include "afxcmn.h"

#include "CheFileData.h"
#include <string>
#include <map>
#include "afxdtctl.h"
using namespace std;
// CTeeChartDlg 对话框
class CTeeChartDlg : public CDialog
{
// 构造
public:
	CTeeChartDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEECHART_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CLyData* GetCurrentData(){return m_pCurData;}
	bool LoadData();
	afx_msg void OnBnClickedButtonOpen();
	DECLARE_EVENTSINK_MAP()
	CString m_temp;

	afx_msg void OnBnClickedButton1();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;

protected:	
	map<string, CLyData*> m_cheFileDataMap;
	CLyData*	m_pCurData;

public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonSelinputfile();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButtonsave();

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CDateTimeCtrl m_dtDateCtrl;
	CDateTimeCtrl m_dtTimeCtrl;
	CDateTimeCtrl m_dtTimeCtrl2;
	afx_msg void OnBnClickedButtonOutdir();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnEnChangeEditName();
};
