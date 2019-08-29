#pragma once

class CKW1281
{
public:
	CKW1281(void);
	~CKW1281(void);

private:
	struct strBlock
	    {
		 UCHAR  Length;
		 UCHAR  Counter;
		 UCHAR  Title;
		 UCHAR  Data[128];
		 UCHAR  Status;
	    };
    struct strTextBlock
        {
		 UCHAR Data[16];
		 UCHAR Length;
		 UCHAR Status;
        };
	struct strParameterGroupBlock
	    {
	     UCHAR ParameterType;
		 UCHAR Data[16];
		 float Value;
		 UCHAR Status;
	    };
	struct strGroupBlock
	    {
	     strParameterGroupBlock Parameter[16];
		 UCHAR Status;
		 UCHAR Enable;
	    };

	HANDLE   hPort;
	HANDLE   hTimerThread;
    UCHAR    ReopenPort;	
	UCHAR    PortStatus;
	strBlock TxBlock;
	strBlock RxBlock;
	UCHAR    BlockNumber;

	USHORT WaitByteNoEcho(USHORT Timeout);
	USHORT ReceiveByte(USHORT Timeout);
	UCHAR  ReceiveKeyWord(void);
    UCHAR  ReceiveBlock(void);

	UCHAR  SendByteNoEcho(UCHAR Data);
	UCHAR  SendByteWithEchoTest(UCHAR Data);
	UCHAR  SendByteWithEchoAndWaitACK(UCHAR Data);
	UCHAR  SendBlock(void);

	UCHAR  ReceiveTextBlock(void);
	UCHAR  ReceiveParameterGroup(void);
	void   AllDataReceive(void);

	void   DisableGroup(UCHAR Group);
	void   Convert(UCHAR Group);
	void   InitData(UCHAR Reason);
	float  ConvertTwoBytesInValue(UCHAR ParameterType, UCHAR Lo, UCHAR Hi);

	UCHAR BitBangCount;
	UCHAR BitBangArray[10];
	UCHAR BitBangNext(void);
	void  BitBangInit(UCHAR Address);

public:
	void TimerThreadInClass(void);
	strTextBlock TextBlock[32];
	strGroupBlock GroupBlock[17];
	UCHAR InitDone; 
	UCHAR Connect(UCHAR Port);
	void Disconnect(void);
};
DWORD TimerThread(LPVOID pParam);
