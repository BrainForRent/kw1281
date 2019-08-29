// myFont.cpp : implementation file
//

#include "stdafx.h"
#include "TestKW1281.h"
#include "myFont.h"


// myFont

myFont::myFont()
{
}

myFont::~myFont()
{
}


void myFont::Init(void)
{

          h72.CreateFont(72,0,0,0,FW_HEAVY,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h60.CreateFont(60,0,0,0,FW_HEAVY,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h50.CreateFont(50,0,0,0,FW_HEAVY,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h40.CreateFont(40,0,0,0,FW_HEAVY,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h30.CreateFont(30,0,0,0,FW_HEAVY,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h24.CreateFont(24,0,0,0,FW_ULTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h22.CreateFont(22,0,0,0,FW_HEAVY,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h20.CreateFont(20,0,0,0,FW_ULTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h16.CreateFont(16,0,0,0,FW_ULTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h18.CreateFont(18,0,0,0,FW_ULTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h12.CreateFont(12,0,0,0,FW_ULTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");
          h14.CreateFont(14,0,0,0,FW_ULTRABOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"aaa");

}
void myFont::Destroy(void)
{


          h72.DeleteObject();
          h60.DeleteObject();
          h50.DeleteObject();
          h40.DeleteObject();
          h30.DeleteObject();
          h24.DeleteObject();
          h22.DeleteObject();
          h20.DeleteObject();
          h18.DeleteObject();
          h16.DeleteObject();
          h14.DeleteObject();
          h12.DeleteObject();
}


// myFont member functions
