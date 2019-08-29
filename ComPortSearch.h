#pragma once


// ÑComPortSearch

class CComPortSearch : public CComboBox
{
	DECLARE_DYNAMIC(CComPortSearch)

public:
	CComPortSearch();
	virtual ~CComPortSearch();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnCbnSelchange();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	void SearchPorts(void);
	USHORT RegistrySearch(void);
	USHORT RegistrySearchResult;
	COLORREF TextColor;
	struct strPort
	    {
		 TCHAR * Name;
		 UCHAR Number;
	    };	
	strPort Ports[256];

public:
	

	//Clear ports list and search registry
	void Init(void);
	//
	USHORT GetSelectedPortNumber(void);
	//
	void SetColor(COLORREF Color);
};


