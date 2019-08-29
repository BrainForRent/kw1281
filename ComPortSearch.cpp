// ÑComPortSearch.cpp : implementation file
//

#include "stdafx.h"
#include "TestKW1281.h"
#include "ComPortSearch.h"


// ÑComPortSearch

IMPLEMENT_DYNAMIC(CComPortSearch, CComboBox)

CComPortSearch::CComPortSearch()
{
	     TextColor = RGB(0,0,0);
}

CComPortSearch::~CComPortSearch()
{
}


BEGIN_MESSAGE_MAP(CComPortSearch, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &CComPortSearch::OnCbnSelchange)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// ÑComPortSearch message handlers



void CComPortSearch::OnCbnSelchange()
{
     // TODO: Add your control notification handler code here
          if (GetCurSel()==RegistrySearchResult) SearchPorts();

}

BOOL CComPortSearch::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
     // TODO: Add your specialized code here and/or call the base class

          Beep(1000,1000);

     return CComboBox::Create(dwStyle, rect, pParentWnd, nID);
}

void CComPortSearch::Init(void)
{
     // TODO: Add your control notification handler code here

     //Clear port list and search in registry all available ports
     USHORT us0;
          for (us0=0;us0<256;us0++)
               {
               Ports[us0].Number = 0;
               }
          SearchPorts();
}

void CComPortSearch::SearchPorts(void)
{
     USHORT us0;
     CString Str0, Str1;

          //Block combo
          InsertString(0, _T("Search COM ports ........"));
          SetCurSel(0);
          EnableWindow(FALSE);
          //Search in registry all available ports
          RegistrySearch();
          //Clear combo
          ResetContent();
          //Fill combo
          for (us0=0;us0<RegistrySearchResult;us0++)
               {
               Str0.Format(_T("COM%03u   "),Ports[us0].Number);
               Str1 = Ports[us0].Name;
               Str0+= Str1;
               InsertString(us0, Str0);
               }
          InsertString(-1, _T("Search COM ports ........"));
          SetCurSel(0);
          //Unblock combo 
          EnableWindow(TRUE);
}
USHORT CComPortSearch::RegistrySearch(void)
{

     LONG lRet;
     HKEY hKey;

     CString Str0;     
     UCHAR uc0;
     USHORT us0,us1,us2;

     TCHAR Charecter;
     DWORD nValues, nMaxValueNameLen, nMaxValueLen;
     DWORD nValueLen, nValueNameLen;
     DWORD dwType;

     TCHAR * ValueName;
     TCHAR * Value;

     strPort Sort;
     UCHAR Temp;

          for (us0=0;us0<256;us0++)
               {
               if (Ports[us0].Number!=0)
                    {
                    delete[] Ports[us0].Name;
                    Ports[us0].Number = 0;
                    }
               }
          //Read available ports quantity
          lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\"), 0, KEY_READ, &hKey);
          lRet = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL,
                                   &nValues, &nMaxValueNameLen, &nMaxValueLen, NULL, NULL);
          //Read each available 
          if (nValues>0)
               {
               nValueNameLen = nMaxValueNameLen + sizeof(TCHAR);
               nValueLen     = nMaxValueLen     + sizeof(TCHAR);
               ValueName     = new TCHAR[nValueNameLen];
               Value         = new TCHAR[nValueLen];
               for (us0=0;us0<nValues;us0++)
                    {
                    dwType        = REG_NONE;   
                    nValueNameLen = nMaxValueNameLen + sizeof(TCHAR);
                    nValueLen     = nMaxValueLen     + sizeof(TCHAR);
                    lRet = RegEnumValue(hKey, us0, 
                                        (LPTSTR) ValueName, &nValueNameLen,
                                        NULL, &dwType, 
                                        (LPBYTE) Value, &nValueLen);
                    //Number
                    Str0='0';
                    for (uc0=0; uc0<nValueLen; uc0++)
                         {
                         Charecter = Value[uc0];
                         if ((Charecter>='0')&&(Charecter<='9'))
                              {
                              Str0+=Charecter;   
                              }
                         }
                    Ports[us0].Number = (UCHAR) atoi(Str0);    
                    //Name
                    Ports[us0].Name     = new TCHAR[nValueNameLen+1];

                    for (uc0=0; uc0<nValueNameLen+1; uc0++)
                         {//brute force search
                         Ports[us0].Name[uc0]=ValueName[uc0];
                         }
                    }
               delete[] ValueName;
               delete[] Value;
               }
          RegistrySearchResult = (USHORT) nValues; 

          //Sort available port list, number increase order
          for (us0=0;us0<RegistrySearchResult;us0++)
               {
               Temp = Ports[us0].Number;
               us2  = us0;
               for(us1=us0+1;us1<RegistrySearchResult;us1++)
                    {//find lower
                    if (Temp>Ports[us1].Number)
                         {
                         Temp = Ports[us1].Number;
                         us2 = us1;
                         }
                    }
               Sort.Number = Ports[us0].Number;
               Sort.Name   = Ports[us0].Name;
               Ports[us0].Number = Ports[us2].Number;
               Ports[us0].Name   = Ports[us2].Name;
               Ports[us2].Number = Sort.Number;
               Ports[us2].Name   = Sort.Name;
               }
     return (USHORT) nValues;            
}
USHORT CComPortSearch::GetSelectedPortNumber(void)
{
     USHORT us0;
     USHORT Result;

          us0    = GetCurSel();
          Result = Ports[us0].Number;

     return(Result);
}




HBRUSH CComPortSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
     HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
     // TODO:  Change any attributes of the DC here

          pDC->SetTextColor( TextColor);

     // TODO:  Return a different brush if the default is not desired
     return hbr;
}

void CComPortSearch::SetColor(COLORREF Color)
{
          TextColor = Color;
          Invalidate(TRUE);
}