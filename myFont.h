#pragma once

// myFont command target

class myFont
{
public:
	myFont();
	virtual ~myFont();
	
	CFont h72,h50,h60,h40,h30,h24,h22,h20,h18,h16,h14,h12;
	
	void Init(void);
	void Destroy(void);
};


