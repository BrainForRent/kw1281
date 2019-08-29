#include "StdAfx.h"
#include "KW1281.h"
#include "PTC.h"

#define Job_PortOpenWait       0
#define Job_PortReady          1
#define Job_SendAddres         2 
#define Job_WaitKeyWord        3
#define Job_WaitTextBlock      4
#define Job_WaitParameterBlock 5
#define Job_Error              6
#define Job_Timeout            7

#define PortStatus_Open        1
#define PortStatus_Closed      2
#define PortStatus_Error       3




CKW1281::CKW1281(void)
{
     ULONG m_dwID;
     UCHAR uc0;
          for (uc0=0;uc0<17;uc0++)
               {
               GroupBlock[uc0].Enable = 0;
               }
          hTimerThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE) TimerThread,this,0,&m_dwID); 
}

CKW1281::~CKW1281(void)
{
          TerminateThread(hTimerThread,0);
}
UCHAR CKW1281::Connect(UCHAR Port)
{
     DCB dcb;
     COMMTIMEOUTS  ct;    
     char PortName[15];

          ReopenPort = Port;
          sprintf_s(&PortName[0],14,"\\\\.\\COM%u", Port);
          hPort=CreateFileA(PortName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
          if (hPort==INVALID_HANDLE_VALUE)
              {//Something wrong
               PortStatus = PortStatus_Error;
               return 1;
              }
          else
              {//All OK
               GetCommState(hPort,&dcb);
               dcb.ByteSize = 8;             // number of bits/byte, 4-8
               dcb.StopBits = 0;             // 0,1,2 = 1, 1.5, 2
               dcb.Parity = NOPARITY;      // 0-4=no,odd,even,mark,space
               dcb.BaudRate = 10400;
               dcb.fDtrControl =DTR_CONTROL_DISABLE;//DTR_CONTROL_ENABLE;//DTR_CONTROL_HANDSHAKE;
               dcb.fRtsControl =RTS_CONTROL_DISABLE;//RTS_CONTROL_ENABLE;//RTS_CONTROL_HANDSHAKE;
               SetCommState(hPort,&dcb);

               ct.ReadIntervalTimeout         = 1;
               ct.ReadTotalTimeoutMultiplier  = 1;
               ct.ReadTotalTimeoutConstant    = 1;
               ct.WriteTotalTimeoutMultiplier = 0;
               ct.WriteTotalTimeoutConstant   = 0;
               SetCommTimeouts(hPort,&ct);
               //Prepare
               EscapeCommFunction(hPort,SETDTR|SETRTS);	
               PurgeComm(hPort,PURGE_RXABORT|PURGE_TXABORT|PURGE_RXCLEAR|PURGE_TXCLEAR);
               PortStatus = PortStatus_Open;
               return 0;
              }
		
}
void CKW1281::Disconnect(void)
{
          InitData(4);
          PortStatus = PortStatus_Closed;
          CloseHandle(hPort); 
}

DWORD TimerThread(LPVOID pParam)
{
     CKW1281 *KW1281 = (CKW1281 *) pParam;
          //bad code, don't do this in real life
          KW1281->TimerThreadInClass();
     return(0);
}


UCHAR CKW1281::BitBangNext(void)
{
    //Send next bit from bit array.
    UCHAR Result;
          if (BitBangArray[BitBangCount]!=0)				      
               {// high
               ClearCommBreak(hPort);
               }
          else
               {// low
               SetCommBreak(hPort);
               }
          BitBangCount++;
          if (BitBangCount>=10) 
               {//Stop bit rising edge already send
               //Be ready for ECU fast answer
               EscapeCommFunction(hPort,SETDTR|CLRRTS);	
               PurgeComm(hPort,PURGE_RXABORT|PURGE_TXABORT|PURGE_RXCLEAR|PURGE_TXCLEAR);
               Result = 0;
               } 
          else
               {//
               Result = 1;
               }	 
	return(Result);	     
}
void CKW1281::BitBangInit(UCHAR Address)
{
     //Convert Address in bit array, 
     //[0]=Start bit, [1..7]=Address (LSB first), [8]=Parity, [9]=Stop.
     //Bit counter ready for Start via BitBangNext()
     UCHAR Odd, Bit;
     UCHAR uc0;

          Odd = 1;
          BitBangArray[0] = 0;//Start bit
          for(uc0=0;uc0<7;uc0++)
               {
               if ((Address&(1<<uc0))!=0) Bit = 1; else Bit = 0;
               BitBangArray[uc0+1] = Bit;
               Odd^=Bit;
               }
          BitBangArray[8] = Odd; //Parity bit
          BitBangArray[9] = 1;   //Stop bit
          BitBangCount    = 0;
          //Port prepare, maybe not good place for.
          EscapeCommFunction(hPort,SETDTR|SETRTS);	
          PurgeComm(hPort,PURGE_RXABORT|PURGE_TXABORT|PURGE_RXCLEAR|PURGE_TXCLEAR);
}

  
  
void CKW1281::TimerThreadInClass(void)
{
     LONGLONG ExPTC, CurrPTC, Period, Period200ms, Period100ms, Period20ms, Period10ms;  
     extern LONGLONG GetPTC(void);
     extern LONGLONG GetPTCfreq(void);
     UCHAR ErrorTimeout;
     UCHAR Job;
	

          //Init data arrays like after exchange stop
          InitData(4);
         
          //Init job marker
          Job = Job_PortOpenWait;
          InitDone = 0;

          //Calculate performance counter values
          Period      = GetPTCfreq();
          Period200ms = Period/5;
          Period100ms = Period/10;
          Period20ms  = Period/50;
          Period10ms  = Period/100;
          Period      = Period100ms;
          ExPTC       = GetPTC();
          while(1)
               {
               CurrPTC=GetPTC();
               if ((CurrPTC-ExPTC)>Period)
                    {
                    ExPTC = CurrPTC;
                    switch(Job)
                         {
                         case Job_PortOpenWait://Port closed, wait open
                              switch(PortStatus)
                                   {
                                   case PortStatus_Open:
                                        Job = Job_PortReady;
                                        InitData(1);
                                        break;
                                   case PortStatus_Error:
                                        Connect(ReopenPort);
                                        InitData(4);
                                        break;     
                                   }
                              break; 
                         case Job_PortReady://Opened just now
                              Period = Period200ms;
                              BitBangInit(8);
                              Job = Job_SendAddres;
                              InitDone = 0;
                              break;     
                         case Job_SendAddres://1 start bit, 7 data bits, 1 parity, 1 stop bit
                              if (BitBangNext()==0)				      
                                   {//Address sended
                                   Job = Job_WaitKeyWord;
                                   Period = Period10ms;
                                   }
                              break;
                         case Job_WaitKeyWord://KeyWord
                              if (ReceiveKeyWord()==0)
                                   {//KeyWord received
                                   Job = Job_WaitTextBlock;
                                   } 
                              else
                                   {//Нет ключевого слова
                                   Job = Job_Error;
                                   }  
                              break;
                         case Job_WaitTextBlock://Text Block's
                              if (ReceiveTextBlock()==0)
                                   {//Text blocks received
                                   Job = Job_WaitParameterBlock;
                                   InitDone = 1;
                                   Period = Period100ms;
                                   } 
                              else
                                   {//No text blocks
                                   Job = Job_Error;
                                   }  
                              break;
                         case Job_WaitParameterBlock://Parameter Block's
                              if (ReceiveParameterGroup()==0)
                                   {//Parameter blocks received
							    
                                   } 
                              else
                                   {//No parameter blocks
                                   Job = Job_Error;
                                   }  
                              break;
                         case Job_Error://
                              //Ждать 1 сек
                              Period = Period100ms;
                              ErrorTimeout  = 0;
                              Job = Job_Timeout;
                              break;
                         case Job_Timeout://Timeout counter
                              ErrorTimeout++;
                              if (ErrorTimeout>10) 
                                   {Job = Job_PortOpenWait;// one more try
                                   if (PortStatus == PortStatus_Open)
                                        {
                                        CloseHandle(hPort); 
                                        Sleep(200);
                                        Connect(ReopenPort);
                                        InitData(3);
                                        } 
                                   else
                                        {    
                                        InitData(4);
                                        } 
                                   }
                              break;
                         }
                    }
               Sleep(5);
               }  
}
void CKW1281::InitData(UCHAR Reason)
{
/*
     1- clear data after restart
     2- error
     3- clear data after error
     4- clear data after exchange stop
*/   
     UCHAR uc0, uc1, uc2;

          for (uc0=0;uc0<32;uc0++)
               {
               TextBlock[uc0].Length  = 0;
               TextBlock[uc0].Data[0] = 0;
               TextBlock[uc0].Status  = 0xFF;
               }
          for (uc0=0;uc0<16;uc0++)
               {
               GroupBlock[uc0].Status = 0;
               for (uc1=0;uc1<16;uc1++)
                    {
                    GroupBlock[uc0].Parameter[uc1].ParameterType = 0;
                    GroupBlock[uc0].Parameter[uc1].Value = 0;
                    GroupBlock[uc0].Parameter[uc1].Status = Reason;
                    for (uc2=0;uc2<16;uc2++)
                         {
                         GroupBlock[uc0].Parameter[uc1].Data[uc2] = 0;
                         }
                    }
               } 
}

USHORT CKW1281::WaitByteNoEcho(USHORT Timeout)
{
/*
     0x00dd - Byte received
     0x0100 - No data received
     0x0200 - Clear port error
     0x0300 - Read port error
*/  
     UCHAR   In[4];
     DWORD   bc;
     USHORT  Result; 
     USHORT  Time;
     DWORD   lpErrors;
     COMSTAT lpStat;
     UCHAR   mBreak;

	     
          Time = 0;
          mBreak = 1;
          while(mBreak)
               {
               Sleep(1);
               if (ClearCommError(hPort, &lpErrors, &lpStat)!=0)
                    {
                    if (lpStat.cbInQue>0)
                         {
                         if (ReadFile(hPort,&In[0],1,&bc,NULL)!=0)
                              {
                              if (bc>0) 
                                   {//Byte received
                                   mBreak = 0;
                                   Result = (USHORT) In[0];
                                   }
                              }
                         else
                              {//Read port error
                              mBreak = 0;
                              Result = 0x0300;
                              }
                         }
                    }
               else
                    {//Clear port error
                    mBreak = 0;
                    Result = 0x0200;
                    }         
               Time++;
               if (Time>Timeout) 
                    {//No data received error
                    mBreak = 0;
                    Result = 0x0100;
                    }
               }
     return(Result);
}
UCHAR CKW1281::SendByteNoEcho(UCHAR Data)
{
/*
     0x00 - Byte transmited
     0x01 - write file errror
     0x02 - secommmask error
     0x03 - waitcomevent error
*/  
     DWORD bc;
     DWORD dwMask=0;
     UCHAR Result;
	 
          if (SetCommMask(hPort,EV_TXEMPTY)!=0)
               {
               if (WriteFile(hPort,&Data,1,&bc,NULL)!=0)
                    {
                    if (WaitCommEvent(hPort,&dwMask,NULL)!=0)
                         {//Byte transmited
                         Result = 0;
                         }
                    else
                         {//waitcomevent error
                         Result = 3;
                         }     
                    }
               else
                    {//write file errror
                    Result = 1;
                    }    
               }
          else
               {//secommmask error
               Result = 2;
               }    
	     
     return(Result);  
}
UCHAR CKW1281::SendByteWithEchoTest(UCHAR Data)
{
/*
     0x00 - Byte transmited, Echo OK
     0x01 - no echo byte received or echo error
     0x02 - Transmit error
*/
     UCHAR Result;
	 
          if (SendByteNoEcho(Data)==0)
               {
               if (WaitByteNoEcho(200)==Data)
                    {//Echo OK
                    Result = 0;
                    }
               else    
                    {//no echo byte received or echo error
                    Result = 1;
                    }
               }
          else
               {//Transmit error
               Result = 2;
               }    

	return(Result);	     
}
UCHAR CKW1281::SendByteWithEchoAndWaitACK(UCHAR Data)
{
/*
     0x00 - Byte transmited, ACK OK
     0x01 - no ACK byte received or ACKo error
     0x02 - Echo error
*/
     UCHAR Result;
     UCHAR ACK;
          ACK=Data^0xFF;
          if (SendByteWithEchoTest(Data)==0)
               {
               if (WaitByteNoEcho(200)==ACK)
                    {//Byte transmited, ACK OK
                    Result = 0;
                    }
               else    
                    {//no ACK byte received or ACKo error
                    Result = 1;
                    }
               }
          else
               {//Echo error
               Result = 2;
               }         
     return(Result);
}

UCHAR CKW1281::ReceiveKeyWord(void)
{
/*   0= KeyWord ОК
     1= no/wrong echo error
     2= KeyWord error
*/	
     UCHAR Result; 
	
          if (WaitByteNoEcho(200)==0x0055) 
               {
               if (WaitByteNoEcho(200)==0x0001) 
                    {
                    if (WaitByteNoEcho(200)==0x008A) 
                         {//KeyWord received
                         //Send 0x75 with echo test
                         if (SendByteWithEchoTest(0x75)==0)
                              {//KeyWord ОК
                              Result = 0;
                              BlockNumber = 0;
                              }
                         else    
                              {//no/wrong echo error
                              Result = 1;
                              }
                         }
                    else
                         {//KeyWord error
                         Result = 2;
                         }    
                    }
               else
                    {//KeyWord error
                    Result = 2;
                    }    
               }
          else
               {//KeyWord error
               Result = 2;
               }    
     return(Result);
}


USHORT CKW1281::ReceiveByte(USHORT Timeout)
{
/*    
     0х00dd - all OK
     0x0100 - no data received error
     0x0200 - no/wrong echo error
*/
	
     USHORT Result;
     USHORT Received;
     UCHAR  ACK;
          
          Received = WaitByteNoEcho(Timeout);
          if (Received<0x0100)
               {//Byte received
               //return ACK
               ACK = (UCHAR) Received;
               ACK^= 0xFF;
               //Send ACK with echo test
               if (SendByteWithEchoTest(ACK)==0)
                    {//all OK
                    Result = Received;
                    }
               else
                    {//no/wrong echo error
                    Result = 0x0200;
                    }
               }
          else
               {//no data received error
               Result = 0x0100;
               }
     return(Result);
}

UCHAR CKW1281::ReceiveBlock(void)
{
/*
     0      - all OK
     1      - length error
     2..254 - wrong byte number
     255    - block end 0x03 receive error 
*/
     UCHAR BufferCntr;
     UCHAR BufferLimit;
     USHORT Data;
     UCHAR Result;
     UCHAR * ptrData;
     UCHAR mBreak;

          ptrData = &RxBlock.Counter;
          //Wait block length
          Data = ReceiveByte(100);
          Result  = 0;
          if (Data>0x00FF) Result  = 1;//length error
          else
		     {//Length receiver
               RxBlock.Length = (UCHAR) Data;
               BufferLimit = RxBlock.Length-2;
               //Wait all bytes
               mBreak = 1;
               BufferCntr = 0;
               while(mBreak)
                    {
                    Data = ReceiveByte(20);
                    if (Data>0x00FF) 
                         {//Byte receive error
                         Result  = BufferCntr + 2;
                         mBreak = 0;
                         }
                    else
                         {//Byte OK
                         *ptrData++ =  (UCHAR) Data;
                         BufferCntr++;
                         if (BufferCntr>BufferLimit)
                              {//Block received
                              mBreak = 0;
                              //Wait block end 0x03
                              Data = WaitByteNoEcho(100);
                              //Test block end
                              if (Data==0x0003)
                                   {//all OK
                                   Result  = 0;
                                   }
                              else    
                                   {//block end 0x03 receive error 
                                   Result = 255;
                                   }
                              }
                         }   
                    } 
               }
          RxBlock.Status = Result;
     return(Result);
}

UCHAR CKW1281::SendBlock(void)
{
/*
     0      - all OK
     1..254 - wrong byte number
     255    - block end 0x03 send error 
*/
     UCHAR BufferCntr;
     UCHAR BufferLimit;
     UCHAR mBreak;
     UCHAR * ptrData;
     UCHAR Result;

          BufferCntr = 0;
          BufferLimit = TxBlock.Length-1;
          ptrData = &TxBlock.Length;
         
          mBreak = 1;
          while(mBreak)//remember MISRA rule to use break only in switch
               {
               if (SendByteWithEchoAndWaitACK(*ptrData++)!=0)
                    {//Byte transmit error
                    Result  = BufferCntr + 1;
                    mBreak = 0;
                    }
               else
                    {//Byte transmitted
                    BufferCntr++;
                    if (BufferCntr>BufferLimit)
                         {//End of block
                         mBreak = 0;
                         //Send block end 0x03
                         if (SendByteWithEchoTest(0x03)==0)
                              {//all OK
                              Result  = 0;
                              }     
                         else
                              {//block end 0x03 send error
                              Result  = 255;
                              }     
                         }
                    }
               } 
     return(Result);
}

UCHAR CKW1281::ReceiveTextBlock(void)
{
/*
     0     - all OK
     0x0d  - first wrong received block
     0xd0  - first wrong transmitted block
*/    
     UCHAR mBreak;
     UCHAR Result;
     UCHAR Block;
     UCHAR uc0;
    
          mBreak = 1;
          //Text block with 12 characters
          Block = 0;
          while(mBreak!=0)
               {
               TextBlock[Block].Status = ReceiveBlock();
               if (TextBlock[Block].Status==0)
                    {//Block OK
                    TextBlock[Block].Length = RxBlock.Length - 3;//Length without counter and block end
                    for(uc0=0;uc0<16;uc0++) 
                         {
                         TextBlock[Block].Data[uc0] = RxBlock.Data[uc0];
                         }
                    if (RxBlock.Title==0x09)
                         {//all OK 
                         mBreak = 0;   
                         Result = 0;
                         }
                    else
                         {//Next Block
                         Block++;
                         TxBlock.Length  = 0x03;
                         TxBlock.Counter = RxBlock.Counter+1;
                         TxBlock.Title   = 0x09;
                         if (SendBlock()!=0)
                              {//first wrong transmitted block
                              Result = Block << 4; 
                              mBreak = 0;
                              }
                         }    
                    }
               else
                    {//first wrong received block
                    Result = Block + 1; 
                    mBreak = 0;
                    }    
               }
     return(Result);        
}
UCHAR CKW1281::ReceiveParameterGroup(void)
{
/*
     0     - all OK
     0x0d  - first wrong received group
     0xd0  - first wrong transmitted group
*/
	 UCHAR mBreak;
	 UCHAR Group;
	 UCHAR Result;
	
          mBreak = 1;
          Result = 0;
          for (Group=1;Group<9;Group++)
               {
               if (mBreak!=0)
                    {
                    if (GroupBlock[Group].Enable == 1)
                         {//Need to read group
                         TxBlock.Length  = 0x04;
                         TxBlock.Counter = RxBlock.Counter+1;
                         TxBlock.Title   = 0x29;
                         TxBlock.Data[0] = Group;
                         if (SendBlock()==0)
                              {if (ReceiveBlock()==0)
                                   {//all OK
                                   Convert(Group);
                                   }
                              else
                                   {//first wrong received group
                                   Result = Group;
                                   mBreak = 0;
                                   }    
                              }
                         else
                              {//first wrong transmitted group
                              Result = Group << 4;
                              mBreak = 0;
                              }  
                         }
                    else
                         {//Dont need to read group
                         DisableGroup(Group);
                         }   
                    }
               }
     return(Result);        
}

void CKW1281::DisableGroup(UCHAR Group)
{
     UCHAR uc0;
	     
          for (uc0=0;uc0<4;uc0++)
               {
               GroupBlock[Group].Parameter[uc0].Status = 5;
               }
}

void CKW1281::Convert(UCHAR Group)
{
     UCHAR uc0, uc1;
    
          if (GroupBlock[Group].Status==0)
               {//If group received ok convert data in value
               uc1 = 0;
               for (uc0=0;uc0<4;uc0++)
                    {//
                    GroupBlock[Group].Parameter[uc0].ParameterType = RxBlock.Data[uc1++];
                    GroupBlock[Group].Parameter[uc0].Data[0]       = RxBlock.Data[uc1++];
                    GroupBlock[Group].Parameter[uc0].Data[1]       = RxBlock.Data[uc1++];
                    GroupBlock[Group].Parameter[uc0].Value         = ConvertTwoBytesInValue(GroupBlock[Group].Parameter[uc0].ParameterType, GroupBlock[Group].Parameter[uc0].Data[0], GroupBlock[Group].Parameter[uc0].Data[1]);
                    GroupBlock[Group].Parameter[uc0].Status        = 0;
                    }
               }
          else
               {
               GroupBlock[Group].Parameter[0].Status = 1;
               GroupBlock[Group].Parameter[1].Status = 1;
               GroupBlock[Group].Parameter[2].Status = 1;
               GroupBlock[Group].Parameter[3].Status = 1;
               }    
	         
}	         

float CKW1281::ConvertTwoBytesInValue(UCHAR ParameterType, UCHAR Lo, UCHAR Hi)
{
     //No datasheet for this, just blind copy-paste from https://www.blafusel.de/obd/obd2_kw1281.html
    
     float Result;
          switch(ParameterType) 
               {
               case	 1: Result  = (float) 0.2f*Lo*Hi;				break;
               case	 2: Result  = (float) Lo*0.002f*Hi;			break;
               case	 3: Result  = (float) 0.002f*Lo*Hi;			break;
               case	 4: Result  = (float) abs(Hi-127)*0.01f*Lo;		break;
               case	 5: Result  = (float) Lo*(Hi-100)*0.1f;			break;
               case	 6: Result  = (float) 0.001f*Lo*Hi;			break;
               case	 7: Result  = (float) 0.01f*Lo*Hi;				break;
               case	 8: Result  = (float) 0.1f*Lo*Hi;				break;
               case	 9: Result  = (float) (Hi-127)*0.02f*Lo;		break;
               case	11: Result  = (float) 0.0001f*Lo*(Hi-128)+1;	     break;
               case	12: Result  = (float) 0.001f*Lo*Hi;			break;
               case	13: Result  = (float) (Hi-127)*0.001f*Lo;		break;
               case	14: Result  = (float) 0.005f*Lo*Hi;			break; 
               case	15: Result  = (float) 0.01f*Lo*Hi;				break;
               case	18: Result  = (float) 0.04f*Lo*Hi;				break;
               case	19: Result  = (float) Lo*Hi*0.01f;				break;
               case	20: Result  = (float) Lo*(Hi-128)/128;			break;
               case	21: Result  = (float) 0.001f*Lo*Hi;			break;
               case	22: Result  = (float) 0.001f*Lo*Hi;			break;
               case	23: Result  = (float) Hi/256*Lo;				break;
               case	24: Result  = (float) 0.001f*Lo*Hi;			break;
               case	25: Result  = (float) (Hi*1.421f)+(Lo/182);		break;
               case	26: Result  = (float) (Hi-Lo);				break;
               case	27: Result  = (float) abs(Hi-128)*0.01f*Lo;		break;
               case	28: Result  = (float) (Hi-Lo);				break;
               case	30: Result  = (float) Hi/12*Lo;				break;
               case	31: Result  = (float) Hi/2560*Lo;				break;
               case	33: Result  = (float) 100*Hi/Lo ;				break;
               case	34: Result  = (float) (Hi-128)*0.01f*Lo;		break;
               case	35: Result  = (float) 0.01f*Lo*Hi;				break;
               case	36: Result  = (float) (Lo*2560+Hi*10);			break;
               case	38: Result  = (float) (Hi-128)*0.001f*Lo;		break;
               case	39: Result  = (float) Hi/256*Lo;				break;
               case	40: Result  = (float) Hi*0.1f+(25.5f*Lo)-400;	break;
               case	41: Result  = (float) Hi+Lo*255;				break;
               case	42: Result  = (float) Hi*0.1f+(25.5f*Lo)-400;	break;
               case	43: Result  = (float) Hi*0.1f+(25.5f*Lo);		break;
               case	45: Result  = (float) 0.1f*Lo*Hi/100;			break;
               case	46: Result  = (float) (Lo*Hi-3200)*0.0027f;		break;
               case	47: Result  = (float) (Hi-128)*Lo;				break;
               case	48: Result  = (float) Hi+Lo*255;				break;
               case	49: Result  = (float) (Hi/4)*Lo*0.1f;			break;
               case	50: Result  = (float) (Hi-128)/(0.01f*Lo);		break;
               case	51: Result  = (float) ((Hi-128)/255)*Lo;		break;
               case	52: Result  = (float) Hi*0.02f*Lo-Lo;			break;
               case	53: Result  = (float) (Hi-128)*1.4222f+0.006f*Lo; break;
               case	54: Result  = (float) Lo*256+Hi;				break;
               case	55: Result  = (float) Lo*Hi/200;				break;
               case	56: Result  = (float) Lo*256+Hi;				break;
               case	57: Result  = (float) Lo*256+Hi+65536;			break;
               case	59: Result  = (float) (Lo*256+Hi)/32768;		break;
               case	60: Result  = (float) (Lo*256+Hi)*0.01f;		break;
               case	62: Result  = (float) 0.256f*Lo*Hi;			break;
               case	64: Result  = (float) (Lo+Hi);				break;
               case	65: Result  = (float) 0.01f*Lo*(Hi-127);		break;
               case	66: Result  = (float) (Lo*Hi)/511.12f;			break;
               case	67: Result  = (float) (640*Lo)+Hi*2.5f;			break;
               case	68: Result  = (float) (256*Lo+Hi)/7.365f;		break;
               case	69: Result  = (float) (256*Lo +Hi)*0.3254f;		break;
               case	70: Result  = (float) (256*Lo +Hi)*0.192f;		break;
               }  
     return(Result);
}			 
