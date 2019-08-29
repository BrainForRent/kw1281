// TestKW1281Dlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "KW1281.h"
#include "ComPortSearch.h"
#include "myFont.h"


// CTestKW1281Dlg dialog
class CTestKW1281Dlg : public CDialog
{
// Construction
public:
	CTestKW1281Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TestKW1281_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedGoBtn();
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListCtrl lc_Registers;
	CButton btn_GoOrStop;
	CListBox lb_Diag;

	myFont Font;
	CKW1281 Exchange;
	CComPortSearch ComPortSearch;

	int   nIndex[32];
	float ExValue[32];
	UCHAR ExStatus[32];
	
	UCHAR  SelectedPort;
	UCHAR GoOrStop;
	UINT_PTR Timer;
	
	struct strParemeterRow
	     {
	      CString ParameterName;
	      CString ParameterValue;
	      CString ParameterUnit;
	     };
	struct strGroupRows
	     {
	      strParemeterRow GroupRow[4];
	      CString GroupName;
	     };
	     
	strGroupRows GroupRows[8];     
	
	void RefreshText(void);
	void RefreshGroups(void);
	void CheckboxTranfer(void);

};
