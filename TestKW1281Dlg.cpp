// TestKW1281Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestKW1281.h"
#include "TestKW1281Dlg.h"
#include "ControlCharacterTable.h"
#include "Messages.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestKW1281Dlg dialog




CTestKW1281Dlg::CTestKW1281Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestKW1281Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestKW1281Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REGISTERS_LC, lc_Registers);

	DDX_Control(pDX, IDC_GO_BTN, btn_GoOrStop);
	DDX_Control(pDX, IDC_DIAG_LB, lb_Diag);
	DDX_Control(pDX, IDC_COMBO3, ComPortSearch);
}

BEGIN_MESSAGE_MAP(CTestKW1281Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTestKW1281Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestKW1281Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_GO_BTN, &CTestKW1281Dlg::OnBnClickedGoBtn)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTestKW1281Dlg message handlers

BOOL CTestKW1281Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// TODO: Add extra initialization here
     CString Str;
     UCHAR uc0;
          //Title
          SetWindowText("Test KW1281");
          //Fonts     
          Font.Init();
          //CListBox Init
          lc_Registers.SetFont(&Font.h16,TRUE);
          lc_Registers.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); 
          lc_Registers.InsertColumn(0, ColumnName[0], LVCFMT_CENTER, 55);
          lc_Registers.InsertColumn(1, ColumnName[1], LVCFMT_LEFT, 280);
          lc_Registers.InsertColumn(2, ColumnName[2], LVCFMT_LEFT, 250);
          lc_Registers.InsertColumn(3, ColumnName[3], LVCFMT_CENTER,60);
          lc_Registers.InsertColumn(4, ColumnName[4], LVCFMT_LEFT, 535);
	
          for (uc0=0;uc0<32;uc0++)
               {
               Str.Format("%u / %u",(uc0/4)+1,(uc0%4)+1);
               nIndex[uc0] = lc_Registers.InsertItem(uc0 , Str);
               if ((uc0%4)==0) lc_Registers.SetItemText(nIndex[uc0], 1, Group[uc0/4]);
               lc_Registers.SetItemText(nIndex[uc0], 2, Parameters[uc0]);
               lc_Registers.SetItemText(nIndex[uc0], 4, Units[uc0]);
               //Checkboxes
               lc_Registers.SetCheck(nIndex[uc0]);
               //
               ExValue[uc0] = 12345.6789f;
		     }

          //Init Com port registry search combo 
          ComPortSearch.SetFont(&Font.h22,TRUE);
          ComPortSearch.SetColor(RGB(255,0,0));
          ComPortSearch.Init();
		 
          GoOrStop = 1;
		 
          Timer = SetTimer(1,200,NULL);  

     return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestKW1281Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestKW1281Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestKW1281Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CTestKW1281Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CTestKW1281Dlg::RefreshText(void)
{
     CString Str, Str0;
     UCHAR uc0, uc1, uc2;

          Str = "";
          for (uc0=0; uc0<32; uc0++)
               {
               uc1 = Exchange.TextBlock[uc0].Length;
               if (uc1>0)
                    {
                    Str0.Format("[#%u]= \" ",uc0);
                    Str+=Str0;
                    for(uc2=0;uc2<uc1;uc2++) 
                         {//If control character received show it in <>
                         if (Exchange.TextBlock[uc0].Data[uc2]>=32)
                              {
                              Str+=(char)(Exchange.TextBlock[uc0].Data[uc2]);
                              }
                         else
                              {
                              Str+=ControlCharacter[Exchange.TextBlock[uc0].Data[uc2]];
                              }    
                         }
                    Str+="\";  ";
                    }
               }
          lb_Diag.InsertString(0, Str);
}


void CTestKW1281Dlg::RefreshGroups(void)
{

     CString Str;
     UCHAR uc0, uc1, uc2;
     UCHAR Status[32];
     float Value[32];
     UCHAR UnitRefresh[4] = {0,0,0,0};
	 
	 
          uc2=0;
          for (uc0=1;uc0<9;uc0++)
               {for (uc1=0;uc1<4;uc1++)
                    {
                    Value[uc2] = Exchange.GroupBlock[uc0].Parameter[uc1].Value;
                    Status[uc2]= Exchange.GroupBlock[uc0].Parameter[uc1].Status;
                    uc2++;
                    }
               }     

          for (uc0=0;uc0<32;uc0++)
               {
               //CListCtrl refresh latch for blink prevent
               if ((ExValue[uc0]!=Value[uc0])||
                    (ExStatus[uc0]!=Status[uc0]))
                    {
                    //"Unit" row refresh flags
                    switch(uc0)
                         {
                         case  0: if (Status[0]==0)  UnitRefresh[0] = 1; break;
                         case  1: if (Status[1]==0)  UnitRefresh[1] = 1; break;
                         case  3: if (Status[3]==0)  UnitRefresh[2] = 1; break;
                         case 28: if (Status[28]==0) UnitRefresh[3] = 1; break;
                         }
                    //"Value" row refresh				   
                    switch(Status[uc0])
                         {
                         case 0: Str.Format("%0.2f",Value[uc0]); break;
                         case 1: Str = "wait..."; break;
                         case 2: Str = "ERROR"; break;
                         case 3: Str = "restart"; break;
                         case 4: Str = "OFF"; break;
                         case 5: Str = "n/a"; break;
                         }
                    lc_Registers.SetItemText(nIndex[uc0], 3, Str);
                    //Save data in latch
                    ExValue[uc0]  = Value[uc0];
                    ExStatus[uc0] = Status[uc0];
                    } 
               }
		     
          //"Unit" row refresh
          if (UnitRefresh[0]==1)
               {//"Compressor"		     
               uc0= (UCHAR)(Value[0]+0.5);    
               if (Status[0]==0)
                    { 
                    Str = CompressorStatus[uc0];
                    }
               else
                    {
                    Str = "Unknown";
                    }
               lc_Registers.SetItemText(nIndex[0], 4, Str);        
               }
          if (UnitRefresh[1]==1)
               {//"Engine Speed,recognized"	
               uc0= (UCHAR)(Value[1]+0.5);    
               lc_Registers.SetItemText(nIndex[1], 4, EngineSpeed[uc0]);
               }
          if (UnitRefresh[3]==1)
               {//"Standing Time"	
               uc0= (UCHAR)(Value[3]+0.5);    
               switch(uc0) 
                    {
                    case  250: Str = "Battery disconnected"; break;
                    case  255: Str = "Transfer Malfunction"; break;
                    default: Str = "Minutes"; break;
                    }
               lc_Registers.SetItemText(nIndex[3], 4, Str);
               }
	
          if (UnitRefresh[3]==1)
               {//"Fresh Air Blower,(V2 - specified)"	
               if (Value[28]>11.5) uc0 = 7;
               else if (Value[28]>10.0) uc0 = 6;
                    else if (Value[28]>8.7) uc0 = 5;
                         else if (Value[28]>6.5) uc0 = 4;
                              else if (Value[28]>5.6) uc0 = 3;
                                   else if (Value[28]>4.1) uc0 = 2;
                                        else if (Value[28]>3.6) uc0 = 1;
                                             else uc0 = 0;
               lc_Registers.SetItemText(nIndex[28], 4, FanSpeed[uc0]);
               }
}

void CTestKW1281Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
          RefreshGroups();
          if (Exchange.InitDone==1) 
               {
               RefreshText();
               Exchange.InitDone = 0;
               }
          CheckboxTranfer(); 
	CDialog::OnTimer(nIDEvent);
}

void CTestKW1281Dlg::OnBnClickedGoBtn()
{
	// TODO: Add your control notification handler code here
     CString Str;
          if (GoOrStop!=0)
               {//Open closed port
               GoOrStop = 0;
               btn_GoOrStop.SetWindowText("Stop");
               ComPortSearch.EnableWindow(FALSE);

               SelectedPort = (UCHAR) ComPortSearch.GetSelectedPortNumber();			 
               if (Exchange.Connect(SelectedPort)!=0)
                    {
                    Str.Format("Error opening COM%u", SelectedPort);
                    }
               else
                    {
                    Str.Format("Opened COM%u", SelectedPort);
                    }
               }
          else    
               {//Close opened port
               GoOrStop = 1;
               btn_GoOrStop.SetWindowText("GO");
               ComPortSearch.EnableWindow(TRUE);
               Exchange.Disconnect();
               Str.Format("Closed COM%u", SelectedPort);
               }
          lb_Diag.InsertString(0, Str);


}


void CTestKW1281Dlg::CheckboxTranfer(void)
{
	//changing of one checkbox inside of group causes change of other 3 checkboxes

     UCHAR uc0;
     UCHAR CheckState[4];
     UCHAR One;
     UCHAR NewState;
          for (uc0=0;uc0<8;uc0++)
               {//Test checkboxes inside of group of four
               CheckState[0] = lc_Registers.GetCheck(nIndex[(uc0*4)+0]);
               CheckState[1] = lc_Registers.GetCheck(nIndex[(uc0*4)+1]);
               CheckState[2] = lc_Registers.GetCheck(nIndex[(uc0*4)+2]);
               CheckState[3] = lc_Registers.GetCheck(nIndex[(uc0*4)+3]);
               One = 0;
               if (CheckState[0]!=0) One++;
               if (CheckState[1]!=0) One++;
               if (CheckState[2]!=0) One++;
               if (CheckState[3]!=0) One++;			 
               switch(One)
                    {
                    case 0: NewState = 0; break;
                    case 1: NewState = 1; break;
                    case 2: NewState = 1; break;
                    case 3: NewState = 0; break;
                    case 4: NewState = 1; break;
                    }
               lc_Registers.SetCheck(nIndex[(uc0*4)+0],NewState);
               lc_Registers.SetCheck(nIndex[(uc0*4)+1],NewState);
               lc_Registers.SetCheck(nIndex[(uc0*4)+2],NewState);
               lc_Registers.SetCheck(nIndex[(uc0*4)+3],NewState);
               Exchange.GroupBlock[uc0+1].Enable = NewState;
               }
}